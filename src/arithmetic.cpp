#include <bls12-381/bls12-381.hpp>

// The x86_64 assembly in arithmetic.s uses ELF directives and serves x86_64 ELF targets only; every other target,
// Apple x86_64 included, uses the C++ arithmetic in this file. BLS12_381_PORTABLE_ARITHMETIC selects the C++
// arithmetic on every target; the tests build it that way to compare it with the assembly.
#if defined(__x86_64__) && defined(__ELF__) && !defined(BLS12_381_PORTABLE_ARITHMETIC)
#define BLS12_381_X86_64_ASM
#include <cpuid.h>
#endif

using namespace std;

namespace bls12_381
{

#ifndef BLS12_381_X86_64_ASM
void _add(fp* z, const fp* x, const fp* y)
{
    uint64_t carry, _;

    tie(z->d[0], carry) = Add64(x->d[0], y->d[0], 0);
    tie(z->d[1], carry) = Add64(x->d[1], y->d[1], carry);
    tie(z->d[2], carry) = Add64(x->d[2], y->d[2], carry);
    tie(z->d[3], carry) = Add64(x->d[3], y->d[3], carry);
    tie(z->d[4], carry) = Add64(x->d[4], y->d[4], carry);
    tie(z->d[5], _)     = Add64(x->d[5], y->d[5], carry);

    // if z > q --> z -= q
    // note: this is NOT constant time
    if(!(z->d[5] < fp::MODULUS.d[5] || (z->d[5] == fp::MODULUS.d[5] && (z->d[4] < fp::MODULUS.d[4] || (z->d[4] == fp::MODULUS.d[4] && (z->d[3] < fp::MODULUS.d[3] || (z->d[3] == fp::MODULUS.d[3] && (z->d[2] < fp::MODULUS.d[2] || (z->d[2] == fp::MODULUS.d[2] && (z->d[1] < fp::MODULUS.d[1] || (z->d[1] == fp::MODULUS.d[1] && (z->d[0] < fp::MODULUS.d[0]))))))))))))
    {
        uint64_t b;
        tie(z->d[0], b) = Sub64(z->d[0], fp::MODULUS.d[0], 0);
        tie(z->d[1], b) = Sub64(z->d[1], fp::MODULUS.d[1], b);
        tie(z->d[2], b) = Sub64(z->d[2], fp::MODULUS.d[2], b);
        tie(z->d[3], b) = Sub64(z->d[3], fp::MODULUS.d[3], b);
        tie(z->d[4], b) = Sub64(z->d[4], fp::MODULUS.d[4], b);
        tie(z->d[5], _) = Sub64(z->d[5], fp::MODULUS.d[5], b);
    }
}

void _ladd(fp* z, const fp* x, const fp* y)
{
    uint64_t carry, _;
    tie(z->d[0], carry) = Add64(x->d[0], y->d[0], 0);
    tie(z->d[1], carry) = Add64(x->d[1], y->d[1], carry);
    tie(z->d[2], carry) = Add64(x->d[2], y->d[2], carry);
    tie(z->d[3], carry) = Add64(x->d[3], y->d[3], carry);
    tie(z->d[4], carry) = Add64(x->d[4], y->d[4], carry);
    tie(z->d[5], _)     = Add64(x->d[5], y->d[5], carry);
}

void _double(fp* z, const fp* x)
{
    uint64_t carry, _;

    tie(z->d[0], carry) = Add64(x->d[0], x->d[0], 0);
    tie(z->d[1], carry) = Add64(x->d[1], x->d[1], carry);
    tie(z->d[2], carry) = Add64(x->d[2], x->d[2], carry);
    tie(z->d[3], carry) = Add64(x->d[3], x->d[3], carry);
    tie(z->d[4], carry) = Add64(x->d[4], x->d[4], carry);
    tie(z->d[5], _)     = Add64(x->d[5], x->d[5], carry);

    // if z > q --> z -= q
    // note: this is NOT constant time
    if(!(z->d[5] < fp::MODULUS.d[5] || (z->d[5] == fp::MODULUS.d[5] && (z->d[4] < fp::MODULUS.d[4] || (z->d[4] == fp::MODULUS.d[4] && (z->d[3] < fp::MODULUS.d[3] || (z->d[3] == fp::MODULUS.d[3] && (z->d[2] < fp::MODULUS.d[2] || (z->d[2] == fp::MODULUS.d[2] && (z->d[1] < fp::MODULUS.d[1] || (z->d[1] == fp::MODULUS.d[1] && (z->d[0] < fp::MODULUS.d[0]))))))))))))
    {
        uint64_t b;
        tie(z->d[0], b) = Sub64(z->d[0], fp::MODULUS.d[0], 0);
        tie(z->d[1], b) = Sub64(z->d[1], fp::MODULUS.d[1], b);
        tie(z->d[2], b) = Sub64(z->d[2], fp::MODULUS.d[2], b);
        tie(z->d[3], b) = Sub64(z->d[3], fp::MODULUS.d[3], b);
        tie(z->d[4], b) = Sub64(z->d[4], fp::MODULUS.d[4], b);
        tie(z->d[5], _) = Sub64(z->d[5], fp::MODULUS.d[5], b);
    }
}

void _ldouble(fp* z, const fp* x)
{
    uint64_t carry, _;

    tie(z->d[0], carry) = Add64(x->d[0], x->d[0], 0);
    tie(z->d[1], carry) = Add64(x->d[1], x->d[1], carry);
    tie(z->d[2], carry) = Add64(x->d[2], x->d[2], carry);
    tie(z->d[3], carry) = Add64(x->d[3], x->d[3], carry);
    tie(z->d[4], carry) = Add64(x->d[4], x->d[4], carry);
    tie(z->d[5], _)     = Add64(x->d[5], x->d[5], carry);
}

void _subtract(fp* z, const fp* x, const fp* y)
{
    uint64_t b;
    tie(z->d[0], b) = Sub64(x->d[0], y->d[0], 0);
    tie(z->d[1], b) = Sub64(x->d[1], y->d[1], b);
    tie(z->d[2], b) = Sub64(x->d[2], y->d[2], b);
    tie(z->d[3], b) = Sub64(x->d[3], y->d[3], b);
    tie(z->d[4], b) = Sub64(x->d[4], y->d[4], b);
    tie(z->d[5], b) = Sub64(x->d[5], y->d[5], b);
    if(b != 0)
    {
        uint64_t c, _;
        tie(z->d[0], c) = Add64(z->d[0], fp::MODULUS.d[0], 0);
        tie(z->d[1], c) = Add64(z->d[1], fp::MODULUS.d[1], c);
        tie(z->d[2], c) = Add64(z->d[2], fp::MODULUS.d[2], c);
        tie(z->d[3], c) = Add64(z->d[3], fp::MODULUS.d[3], c);
        tie(z->d[4], c) = Add64(z->d[4], fp::MODULUS.d[4], c);
        tie(z->d[5], _) = Add64(z->d[5], fp::MODULUS.d[5], c);
    }
}

void _lsubtract(fp* z, const fp* x, const fp* y)
{
    uint64_t b, _;
    tie(z->d[0], b) = Sub64(x->d[0], y->d[0], 0);
    tie(z->d[1], b) = Sub64(x->d[1], y->d[1], b);
    tie(z->d[2], b) = Sub64(x->d[2], y->d[2], b);
    tie(z->d[3], b) = Sub64(x->d[3], y->d[3], b);
    tie(z->d[4], b) = Sub64(x->d[4], y->d[4], b);
    tie(z->d[5], _) = Sub64(x->d[5], y->d[5], b);
}
#endif

#ifdef BLS12_381_X86_64_ASM
void __negate(fp* z, const fp* x);
void _negate(fp* z, const fp* x)
{
    if(x->isZero())
    {
        *z = *x;
        return;
    }
    __negate(z, x);
}
#else
void _negate(fp* z, const fp* x)
{
    if(x->isZero())
    {
        *z = *x;
        return;
    }
    uint64_t borrow, _;
    tie(z->d[0], borrow) = Sub64(fp::MODULUS.d[0], x->d[0], 0);
    tie(z->d[1], borrow) = Sub64(fp::MODULUS.d[1], x->d[1], borrow);
    tie(z->d[2], borrow) = Sub64(fp::MODULUS.d[2], x->d[2], borrow);
    tie(z->d[3], borrow) = Sub64(fp::MODULUS.d[3], x->d[3], borrow);
    tie(z->d[4], borrow) = Sub64(fp::MODULUS.d[4], x->d[4], borrow);
    tie(z->d[5], _)      = Sub64(fp::MODULUS.d[5], x->d[5], borrow);
}
#endif

#ifdef BLS12_381_X86_64_ASM
void __multiply(fp* z, const fp* x, const fp* y);
void __mul_ex(fp* z, const fp* x, const fp* y);

typedef void (*blsmul_func_t)(fp*, const fp*, const fp*);

static bool cpu_has_bmi2_and_adx() {
    // borrowed from: https://github.com/Mysticial/FeatureDetector/blob/master/src/x86/cpu_x86.cpp
    int32_t info[4];
    __cpuid_count(0, 0, info[0], info[1], info[2], info[3]);
    int nIds = info[0];
    if(nIds >= 0x00000007) {
        __cpuid_count(0x00000007, 0, info[0], info[1], info[2], info[3]);
        if((info[1] & (1 <<  8)) && (info[1] & (1 << 19))) // BMI2 && ADX
            return true;
    }
    return false;
}

extern "C" char** _dl_argv;

extern "C" blsmul_func_t __attribute__((no_sanitize_address)) resolve_blsmul() {
    int argc = *(int*)(_dl_argv - 1);
    char** my_environ = (char**)(_dl_argv + argc + 1);
    while(*my_environ != nullptr) {
       const char disable_str[] = "BLS_DISABLE_BMI2";
        if(strncmp(*my_environ++, disable_str, strlen(disable_str)) == 0)
           return __multiply;
    }

    if(cpu_has_bmi2_and_adx())
       return __mul_ex;
    return __multiply;
}

void _multiply(fp*, const fp*, const fp*) __attribute__((ifunc("resolve_blsmul")));
#else
void _multiply(fp* z, const fp* x, const fp* y)
{
    array<uint64_t, 6> t;
    array<uint64_t, 3> c;
    {
        // round 0
        uint64_t v = x->d[0];
        tie(c[1], c[0]) = Mul64(v, y->d[0]);
        uint64_t m = c[0] * fp::INP;
        c[2] = madd0(m, fp::MODULUS.d[0], c[0]);
        tie(c[1], c[0]) = madd1(v, y->d[1], c[1]);
        tie(c[2], t[0]) = madd2(m, fp::MODULUS.d[1], c[2], c[0]);
        tie(c[1], c[0]) = madd1(v, y->d[2], c[1]);
        tie(c[2], t[1]) = madd2(m, fp::MODULUS.d[2], c[2], c[0]);
        tie(c[1], c[0]) = madd1(v, y->d[3], c[1]);
        tie(c[2], t[2]) = madd2(m, fp::MODULUS.d[3], c[2], c[0]);
        tie(c[1], c[0]) = madd1(v, y->d[4], c[1]);
        tie(c[2], t[3]) = madd2(m, fp::MODULUS.d[4], c[2], c[0]);
        tie(c[1], c[0]) = madd1(v, y->d[5], c[1]);
        tie(t[5], t[4]) = madd3(m, fp::MODULUS.d[5], c[0], c[2], c[1]);
    }
    {
        // round 1
        uint64_t v = x->d[1];
        tie(c[1], c[0]) = madd1(v, y->d[0], t[0]);
        uint64_t m = c[0] * fp::INP;
        c[2] = madd0(m, fp::MODULUS.d[0], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[1], c[1], t[1]);
        tie(c[2], t[0]) = madd2(m, fp::MODULUS.d[1], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[2], c[1], t[2]);
        tie(c[2], t[1]) = madd2(m, fp::MODULUS.d[2], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[3], c[1], t[3]);
        tie(c[2], t[2]) = madd2(m, fp::MODULUS.d[3], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[4], c[1], t[4]);
        tie(c[2], t[3]) = madd2(m, fp::MODULUS.d[4], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[5], c[1], t[5]);
        tie(t[5], t[4]) = madd3(m, fp::MODULUS.d[5], c[0], c[2], c[1]);
    }
    {
        // round 2
        uint64_t v = x->d[2];
        tie(c[1], c[0]) = madd1(v, y->d[0], t[0]);
        uint64_t m = c[0] * fp::INP;
        c[2] = madd0(m, fp::MODULUS.d[0], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[1], c[1], t[1]);
        tie(c[2], t[0]) = madd2(m, fp::MODULUS.d[1], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[2], c[1], t[2]);
        tie(c[2], t[1]) = madd2(m, fp::MODULUS.d[2], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[3], c[1], t[3]);
        tie(c[2], t[2]) = madd2(m, fp::MODULUS.d[3], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[4], c[1], t[4]);
        tie(c[2], t[3]) = madd2(m, fp::MODULUS.d[4], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[5], c[1], t[5]);
        tie(t[5], t[4]) = madd3(m, fp::MODULUS.d[5], c[0], c[2], c[1]);
    }
    {
        // round 3
        uint64_t v = x->d[3];
        tie(c[1], c[0]) = madd1(v, y->d[0], t[0]);
        uint64_t m = c[0] * fp::INP;
        c[2] = madd0(m, fp::MODULUS.d[0], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[1], c[1], t[1]);
        tie(c[2], t[0]) = madd2(m, fp::MODULUS.d[1], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[2], c[1], t[2]);
        tie(c[2], t[1]) = madd2(m, fp::MODULUS.d[2], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[3], c[1], t[3]);
        tie(c[2], t[2]) = madd2(m, fp::MODULUS.d[3], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[4], c[1], t[4]);
        tie(c[2], t[3]) = madd2(m, fp::MODULUS.d[4], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[5], c[1], t[5]);
        tie(t[5], t[4]) = madd3(m, fp::MODULUS.d[5], c[0], c[2], c[1]);
    }
    {
        // round 4
        uint64_t v = x->d[4];
        tie(c[1], c[0]) = madd1(v, y->d[0], t[0]);
        uint64_t m = c[0] * fp::INP;
        c[2] = madd0(m, fp::MODULUS.d[0], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[1], c[1], t[1]);
        tie(c[2], t[0]) = madd2(m, fp::MODULUS.d[1], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[2], c[1], t[2]);
        tie(c[2], t[1]) = madd2(m, fp::MODULUS.d[2], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[3], c[1], t[3]);
        tie(c[2], t[2]) = madd2(m, fp::MODULUS.d[3], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[4], c[1], t[4]);
        tie(c[2], t[3]) = madd2(m, fp::MODULUS.d[4], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[5], c[1], t[5]);
        tie(t[5], t[4]) = madd3(m, fp::MODULUS.d[5], c[0], c[2], c[1]);
    }
    {
        // round 5
        uint64_t v = x->d[5];
        tie(c[1], c[0]) = madd1(v, y->d[0], t[0]);
        uint64_t m = c[0] * fp::INP;
        c[2] = madd0(m, fp::MODULUS.d[0], c[0]);
        tie(c[1], c[0])    = madd2(v, y->d[1], c[1], t[1]);
        tie(c[2], z->d[0]) = madd2(m, fp::MODULUS.d[1], c[2], c[0]);
        tie(c[1], c[0])    = madd2(v, y->d[2], c[1], t[2]);
        tie(c[2], z->d[1]) = madd2(m, fp::MODULUS.d[2], c[2], c[0]);
        tie(c[1], c[0])    = madd2(v, y->d[3], c[1], t[3]);
        tie(c[2], z->d[2]) = madd2(m, fp::MODULUS.d[3], c[2], c[0]);
        tie(c[1], c[0])    = madd2(v, y->d[4], c[1], t[4]);
        tie(c[2], z->d[3]) = madd2(m, fp::MODULUS.d[4], c[2], c[0]);
        tie(c[1], c[0])    = madd2(v, y->d[5], c[1], t[5]);
        tie(z->d[5], z->d[4]) = madd3(m, fp::MODULUS.d[5], c[0], c[2], c[1]);
    }

    // if z > q --> z -= q
    // note: this is NOT constant time
    if(!(z->d[5] < fp::MODULUS.d[5] || (z->d[5] == fp::MODULUS.d[5] && (z->d[4] < fp::MODULUS.d[4] || (z->d[4] == fp::MODULUS.d[4] && (z->d[3] < fp::MODULUS.d[3] || (z->d[3] == fp::MODULUS.d[3] && (z->d[2] < fp::MODULUS.d[2] || (z->d[2] == fp::MODULUS.d[2] && (z->d[1] < fp::MODULUS.d[1] || (z->d[1] == fp::MODULUS.d[1] && (z->d[0] < fp::MODULUS.d[0]))))))))))))
    {
        uint64_t b, _;
        tie(z->d[0], b) = Sub64(z->d[0], fp::MODULUS.d[0], 0);
        tie(z->d[1], b) = Sub64(z->d[1], fp::MODULUS.d[1], b);
        tie(z->d[2], b) = Sub64(z->d[2], fp::MODULUS.d[2], b);
        tie(z->d[3], b) = Sub64(z->d[3], fp::MODULUS.d[3], b);
        tie(z->d[4], b) = Sub64(z->d[4], fp::MODULUS.d[4], b);
        tie(z->d[5], _) = Sub64(z->d[5], fp::MODULUS.d[5], b);
    }
}
#endif

#ifdef BLS12_381_X86_64_ASM
void _square(fp* z, const fp* x)
{
    __multiply(z, x, x);
}
#else
void _square(fp* z, const fp* x)
{
    _multiply(z, x, x);
}
#endif

// Add64 returns the sum with carry of x, y and carry: sum = x + y + carry.
// The carry input must be 0 or 1; otherwise the behavior is undefined.
// The carryOut output is guaranteed to be 0 or 1.
//
// This function's execution time does not depend on the inputs.
tuple<uint64_t, uint64_t> Add64(
    const uint64_t& x,
    const uint64_t& y,
    const uint64_t& carry
)
{
    // Overflow builtins: exact carry and borrow out for a carry or borrow in of 0 or 1, compiled to add with carry.
    uint64_t sum;
    const bool carryXY = __builtin_add_overflow(x, y, &sum);
    const bool carryIn = __builtin_add_overflow(sum, carry, &sum);
    return {sum, static_cast<uint64_t>(carryXY | carryIn)};
}

// Sub64 returns the difference of x, y and borrow: diff = x - y - borrow.
// The borrow input must be 0 or 1; otherwise the behavior is undefined.
// The borrowOut output is guaranteed to be 0 or 1.
//
// This function's execution time does not depend on the inputs.
tuple<uint64_t, uint64_t> Sub64(
    const uint64_t& x,
    const uint64_t& y,
    const uint64_t& borrow
)
{
    uint64_t diff;
    const bool borrowXY = __builtin_sub_overflow(x, y, &diff);
    const bool borrowIn = __builtin_sub_overflow(diff, borrow, &diff);
    return {diff, static_cast<uint64_t>(borrowXY | borrowIn)};
}

// Mul64 returns the 128-bit product of x and y: (hi, lo) = x * y
// with the product bits' upper half returned in hi and the lower
// half returned in lo.
//
// This function's execution time does not depend on the inputs.
tuple<uint64_t, uint64_t> Mul64(
    const uint64_t& x,
    const uint64_t& y
)
{
    uint128_t result = static_cast<uint128_t>(x) * static_cast<uint128_t>(y);
    uint64_t lo = result;
    uint64_t hi = result >> 64;
    return {hi, lo};
}

tuple<uint64_t, uint64_t, uint64_t> madd(
    const uint64_t& a,
    const uint64_t& b,
    const uint64_t& t,
    const uint64_t& u,
    const uint64_t& v
)
{
    uint64_t hi, lo, carry, _v, _u, _t, _;
    tie(hi, lo) = Mul64(a, b);
    tie(_v, carry) = Add64(lo, v, 0);
    tie(_u, carry) = Add64(hi, u, carry);
    tie(_t, _) = Add64(t, 0, carry);
    return {_t, _u, _v};
}

// madd0 hi = a*b + c (discards lo bits)
uint64_t madd0(
    const uint64_t& a,
    const uint64_t& b,
    const uint64_t& c
)
{
    uint64_t carry, hi, lo, _;
    tie(hi, lo) = Mul64(a, b);
    tie(_, carry) = Add64(lo, c, 0);
    tie(hi, _) = Add64(hi, 0, carry);
    return hi;
}

// madd1 hi, lo = a*b + c
tuple<uint64_t, uint64_t> madd1(
    const uint64_t& a,
    const uint64_t& b,
    const uint64_t& c
)
{
    uint64_t carry, hi, lo, _;
    tie(hi, lo) = Mul64(a, b);
    tie(lo, carry) = Add64(lo, c, 0);
    tie(hi, _) = Add64(hi, 0, carry);
    return {hi, lo};
}

// madd2 hi, lo = a*b + c + d
tuple<uint64_t, uint64_t> madd2(
    const uint64_t& a,
    const uint64_t& b,
    const uint64_t& c,
    const uint64_t& d
)
{
    uint64_t carry, hi, lo, _c, _;
    tie(hi, lo) = Mul64(a, b);
    tie(_c, carry) = Add64(c, d, 0);
    tie(hi, _) = Add64(hi, 0, carry);
    tie(lo, carry) = Add64(lo, _c, 0);
    tie(hi, _) = Add64(hi, 0, carry);
    return {hi, lo};
}

tuple<uint64_t, uint64_t> madd3(
    const uint64_t& a,
    const uint64_t& b,
    const uint64_t& c,
    const uint64_t& d,
    const uint64_t& e
)
{
    uint64_t carry, hi, lo, _c, _;
    tie(hi, lo) = Mul64(a, b);
    tie(_c, carry) = Add64(c, d, 0);
    tie(hi, _) = Add64(hi, 0, carry);
    tie(lo, carry) = Add64(lo, _c, 0);
    tie(hi, _) = Add64(hi, e, carry);
    return {hi, lo};
}

} // namespace bls12_381
