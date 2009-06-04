#ifndef _COMPLEX_H
#error "Never use <bits/cmathcalls.h> directly; include <complex.h> instead."
#endif

#define _Mdouble_complex_ _Mdouble_ _Complex
#define __real__
#define __imag__


/* Trigonometric functions.  */

/* Arc cosine of Z.  */
__MATHCALL (cacos, (_Mdouble_complex_ __z));
/* Arc sine of Z.  */
__MATHCALL (casin, (_Mdouble_complex_ __z));
/* Arc tangent of Z.  */
__MATHCALL (catan, (_Mdouble_complex_ __z));

/* Cosine of Z.  */
__MATHCALL (ccos, (_Mdouble_complex_ __z));
/* Sine of Z.  */
__MATHCALL (csin, (_Mdouble_complex_ __z));
/* Tangent of Z.  */
__MATHCALL (ctan, (_Mdouble_complex_ __z));


/* Hyperbolic functions.  */

/* Hyperbolic arc cosine of Z.  */
__MATHCALL (cacosh, (_Mdouble_complex_ __z));
/* Hyperbolic arc sine of Z.  */
__MATHCALL (casinh, (_Mdouble_complex_ __z));
/* Hyperbolic arc tangent of Z.  */
__MATHCALL (catanh, (_Mdouble_complex_ __z));

/* Hyperbolic cosine of Z.  */
__MATHCALL (ccosh, (_Mdouble_complex_ __z));
/* Hyperbolic sine of Z.  */
__MATHCALL (csinh, (_Mdouble_complex_ __z));
/* Hyperbolic tangent of Z.  */
__MATHCALL (ctanh, (_Mdouble_complex_ __z));


/* Exponential and logarithmic functions.  */

/* Exponential function of Z.  */
__MATHCALL (cexp, (_Mdouble_complex_ __z));

/* Natural logarithm of Z.  */
__MATHCALL (clog, (_Mdouble_complex_ __z));

#ifdef __USE_GNU
/* The base 10 logarithm is not defined by the standard but to implement
   the standard C++ library it is handy.  */
__MATHCALL (clog10, (_Mdouble_complex_ __z));
#endif

/* Power functions.  */

/* Return X to the Y power.  */
__MATHCALL (cpow, (_Mdouble_complex_ __x, _Mdouble_complex_ __y));

/* Return the square root of Z.  */
__MATHCALL (csqrt, (_Mdouble_complex_ __z));


/* Absolute value, conjugates, and projection.  */

/* Absolute value of Z.  */
__MATHDECL (_Mdouble_,cabs, (_Mdouble_complex_ __z));

/* Argument value of Z.  */
__MATHDECL (_Mdouble_,carg, (_Mdouble_complex_ __z));

/* Complex conjugate of Z.  */
__MATHCALL (conj, (_Mdouble_complex_ __z));

/* Projection of Z onto the Riemann sphere.  */
__MATHCALL (cproj, (_Mdouble_complex_ __z));


/* Decomposing complex values.  */

/* Imaginary part of Z.  */
__MATHDECL (_Mdouble_,cimag, (_Mdouble_complex_ __z));

/* Real part of Z.  */
__MATHDECL (_Mdouble_,creal, (_Mdouble_complex_ __z));


#if 0
/* Now some optimized versions.  GCC has handy notations for these
   functions.  Recent GCC handles these as builtin functions so does
   not need inlines.  */
#if defined __GNUC__ && !__GNUC_PREREQ (2, 97) && defined __OPTIMIZE__

/* Imaginary part of Z.  */
extern __inline _Mdouble_
__MATH_PRECNAME(cimag) (_Mdouble_complex_ __z) __THROW
{
  return __imag__ __z;
}

/* Real part of Z.  */
extern __inline _Mdouble_
__MATH_PRECNAME(creal) (_Mdouble_complex_ __z) __THROW
{
  return __real__ __z;
}

/* Complex conjugate of Z.  */
extern __inline _Mdouble_complex_
__MATH_PRECNAME(conj) (_Mdouble_complex_ __z) __THROW
{
  return __extension__ ~__z;
}

#endif
#endif 
