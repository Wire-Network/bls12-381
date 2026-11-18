#pragma once

#include <array>
#include <cstdint>

// The C++ field arithmetic on little-endian limbs, built under another namespace so that it links next to the x86_64
// assembly. arithmetic_differential compares the two.
namespace portable_arithmetic
{

typedef std::array<uint64_t, 6> limbs;

limbs add(const limbs& x, const limbs& y);
limbs ladd(const limbs& x, const limbs& y);
limbs dbl(const limbs& x);
limbs ldbl(const limbs& x);
limbs subtract(const limbs& x, const limbs& y);
limbs lsubtract(const limbs& x, const limbs& y);
limbs negate(const limbs& x);
limbs multiply(const limbs& x, const limbs& y);
limbs square(const limbs& x);

} // namespace portable_arithmetic
