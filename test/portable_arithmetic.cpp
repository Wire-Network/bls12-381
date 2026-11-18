// Built with bls12_381 defined to another namespace and BLS12_381_PORTABLE_ARITHMETIC defined.
#include <bls12-381/bls12-381.hpp>

#include "portable_arithmetic.hpp"

namespace portable_arithmetic
{

using bls12_381::fp;

limbs add(const limbs& x, const limbs& y)
{
    fp z, a(x), b(y);
    bls12_381::_add(&z, &a, &b);
    return z.d;
}

limbs ladd(const limbs& x, const limbs& y)
{
    fp z, a(x), b(y);
    bls12_381::_ladd(&z, &a, &b);
    return z.d;
}

limbs dbl(const limbs& x)
{
    fp z, a(x);
    bls12_381::_double(&z, &a);
    return z.d;
}

limbs ldbl(const limbs& x)
{
    fp z, a(x);
    bls12_381::_ldouble(&z, &a);
    return z.d;
}

limbs subtract(const limbs& x, const limbs& y)
{
    fp z, a(x), b(y);
    bls12_381::_subtract(&z, &a, &b);
    return z.d;
}

limbs lsubtract(const limbs& x, const limbs& y)
{
    fp z, a(x), b(y);
    bls12_381::_lsubtract(&z, &a, &b);
    return z.d;
}

limbs negate(const limbs& x)
{
    fp z, a(x);
    bls12_381::_negate(&z, &a);
    return z.d;
}

limbs multiply(const limbs& x, const limbs& y)
{
    fp z, a(x), b(y);
    bls12_381::_multiply(&z, &a, &b);
    return z.d;
}

limbs square(const limbs& x)
{
    fp z, a(x);
    bls12_381::_square(&z, &a);
    return z.d;
}

} // namespace portable_arithmetic
