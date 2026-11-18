// Compares the C++ field arithmetic with the x86_64 assembly for operands below 4p: every pair of values at the
// modulus boundaries, and random pairs whose limbs are random or sit at carry and borrow boundaries.
#include <array>
#include <cstdint>
#include <functional>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

#include <bls12-381/bls12-381.hpp>

#include "portable_arithmetic.hpp"

using namespace std;
using namespace bls12_381;

typedef portable_arithmetic::limbs limbs;

namespace
{

const uint64_t M = numeric_limits<uint64_t>::max();

// Seed of the generator the random operands come from.
const uint64_t seed = 0x5eed;

// Random operand pairs compared for every operation.
const size_t randomPairs = 200000;

// Mismatches printed before the rest are only counted.
const size_t printedMismatches = 10;

limbs Add384(const limbs& x, const limbs& y)
{
    limbs z;
    uint128_t t = 0;
    for(size_t i = 0; i < 6; i++)
    {
        t += static_cast<uint128_t>(x[i]) + y[i];
        z[i] = static_cast<uint64_t>(t);
        t >>= 64;
    }
    return z;
}

limbs Subtract384(const limbs& x, const limbs& y)
{
    limbs z;
    uint64_t borrow = 0;
    for(size_t i = 0; i < 6; i++)
    {
        uint128_t t = static_cast<uint128_t>(x[i]) - y[i] - borrow;
        z[i] = static_cast<uint64_t>(t);
        borrow = static_cast<uint64_t>(t >> 64) & 1;
    }
    return z;
}

bool Less(const limbs& x, const limbs& y)
{
    for(size_t i = 6; i-- > 0;)
    {
        if(x[i] != y[i])
        {
            return x[i] < y[i];
        }
    }
    return false;
}

// A value below bound whose limbs are random or sit at carry and borrow boundaries.
limbs RandomOperand(mt19937_64& rng, const limbs& bound)
{
    static const array<uint64_t, 7> boundaryLimbs = {0, 1, 2, M - 1, M, uint64_t{1} << 63, (uint64_t{1} << 63) - 1};
    const array<uint64_t, 6> topLimbs = {0, 1, fp::MODULUS.d[5] - 1, fp::MODULUS.d[5], bound[5] - 1, bound[5]};
    for(;;)
    {
        limbs v;
        for(size_t i = 0; i < 5; i++)
        {
            v[i] = rng() % 2 ? rng() : boundaryLimbs[rng() % boundaryLimbs.size()];
        }
        v[5] = rng() % 2 ? rng() % (bound[5] + 1) : topLimbs[rng() % topLimbs.size()];
        if(Less(v, bound))
        {
            return v;
        }
    }
}

struct Operation
{
    const char* name;
    function<limbs(const limbs&, const limbs&)> assembly;
    function<limbs(const limbs&, const limbs&)> portable;
};

const vector<Operation> operations = {
    {"add",
     [](const limbs& x, const limbs& y) { fp z, a(x), b(y); _add(&z, &a, &b); return z.d; },
     [](const limbs& x, const limbs& y) { return portable_arithmetic::add(x, y); }},
    {"lazy add",
     [](const limbs& x, const limbs& y) { fp z, a(x), b(y); _ladd(&z, &a, &b); return z.d; },
     [](const limbs& x, const limbs& y) { return portable_arithmetic::ladd(x, y); }},
    {"double",
     [](const limbs& x, const limbs&) { fp z, a(x); _double(&z, &a); return z.d; },
     [](const limbs& x, const limbs&) { return portable_arithmetic::dbl(x); }},
    {"lazy double",
     [](const limbs& x, const limbs&) { fp z, a(x); _ldouble(&z, &a); return z.d; },
     [](const limbs& x, const limbs&) { return portable_arithmetic::ldbl(x); }},
    {"subtract",
     [](const limbs& x, const limbs& y) { fp z, a(x), b(y); _subtract(&z, &a, &b); return z.d; },
     [](const limbs& x, const limbs& y) { return portable_arithmetic::subtract(x, y); }},
    {"lazy subtract",
     [](const limbs& x, const limbs& y) { fp z, a(x), b(y); _lsubtract(&z, &a, &b); return z.d; },
     [](const limbs& x, const limbs& y) { return portable_arithmetic::lsubtract(x, y); }},
    {"negate",
     [](const limbs& x, const limbs&) { fp z, a(x); _negate(&z, &a); return z.d; },
     [](const limbs& x, const limbs&) { return portable_arithmetic::negate(x); }},
    {"multiply",
     [](const limbs& x, const limbs& y) { fp z, a(x), b(y); _multiply(&z, &a, &b); return z.d; },
     [](const limbs& x, const limbs& y) { return portable_arithmetic::multiply(x, y); }},
    {"square",
     [](const limbs& x, const limbs&) { fp z, a(x); _square(&z, &a); return z.d; },
     [](const limbs& x, const limbs&) { return portable_arithmetic::square(x); }},
};

void Print(const char* label, const limbs& v)
{
    cout << "  " << label << hex;
    for(size_t i = 6; i-- > 0;)
    {
        cout << " " << v[i];
    }
    cout << dec << endl;
}

} // anonymous namespace

int main()
{
    const limbs p = fp::MODULUS.d;
    const limbs one = {1, 0, 0, 0, 0, 0};
    const limbs twoP = Add384(p, p);
    const limbs fourP = Add384(twoP, twoP);

    vector<limbs> boundaryValues = {
        {}, one, Add384(one, one),
        Subtract384(p, Add384(one, one)), Subtract384(p, one), p, Add384(p, one),
        Subtract384(twoP, one), twoP, Add384(twoP, one), Subtract384(fourP, one)
    };

    vector<pair<limbs, limbs>> operands;
    for(const limbs& x : boundaryValues)
    {
        for(const limbs& y : boundaryValues)
        {
            operands.emplace_back(x, y);
        }
    }
    mt19937_64 rng(seed);
    for(size_t i = 0; i < randomPairs; i++)
    {
        limbs x = RandomOperand(rng, fourP);
        limbs y = RandomOperand(rng, fourP);
        operands.emplace_back(x, y);
    }

    size_t mismatches = 0;
    for(const Operation& operation : operations)
    {
        size_t operationMismatches = 0;
        for(const auto& [x, y] : operands)
        {
            const limbs expected = operation.assembly(x, y);
            const limbs actual = operation.portable(x, y);
            if(expected != actual)
            {
                if(mismatches++ < printedMismatches)
                {
                    cout << operation.name << " differs:" << endl;
                    Print("x        ", x);
                    Print("y        ", y);
                    Print("assembly ", expected);
                    Print("portable ", actual);
                }
                operationMismatches++;
            }
        }
        cout << operation.name << ": " << operationMismatches << " of " << operands.size() << " differ" << endl;
    }
    return mismatches == 0 ? 0 : 1;
}
