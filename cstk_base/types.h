/***************************************************************************
                              types.h  -  v.0.1
                             -------------------
    begin                : Fri Feb 27 2004
    copyright            : (C) 2004 by Kristof Van Laerhoven
    email                : 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CSTK_TYPES_H
#define CSTK_TYPES_H

/**   
  *@authors: Kristof Van Laerhoven
  */

#include <limits.h> // int limits
#include <float.h>  // float limits

/*
    These types are definitions to keep the space they occupy transparent,
    using the minimum-value requirements in the limits.h standard header.
    C and C++ compilers have varying ways of representing types like 
    integer, so we use these limits to make sure a certain range is met
    for a particular value.
    Just to make sure we're talking about the same ranges: 

    Integers:                                 Redefined from:
     * s_8b: signed 8 bit                   -> signed char
     * u_8b: unsigned 8 bit                 -> unsigned char
     * s_16b: signed 16 bit                 -> signed short
     * u_16b: unsigned 16 bit               -> unsigned short
     * s_32b: signed 32 bit                 -> signed long
     * u_32b: unsigned 32 bit               -> unsigned long
     * s_64b: signed 64 bit                 -> signed long long
     * u_64b: unsigned 64 bit               -> unsigned long long
    Floating Points: 
     * f_32b: floating point using 32 bits  -> float
     * f_64b: floating point using 64 bits  -> double
     * f_96b: floating point using 96 bits  -> long double
     
     (note the lack of any int type here..)  
     
     On top of that, there are _min and _max defines that point to their
     respective absolute minima and maxima.         
*/  

// signed 8-bit, {-127,127}
typedef signed char s_8b; 
#define s_8b_min SCHAR_MIN
#define s_8b_max SCHAR_MAX
// unsigned 8-bit, {0,255}
typedef unsigned char u_8b; 
#define u_8b_min 0
#define u_8b_max UCHAR_MAX
// signed 16-bit, {-32767,32767}
typedef signed short s_16b; 
#define s_16b_min SHRT_MIN
#define s_16b_max SHRT_MAX
// unsigned 16-bit, {0,65535}
typedef unsigned short u_16b;
#define u_16b_min 0
#define u_16b_max USHRT_MAX
// signed 32-bit, {-2147483647,2147483647}
typedef signed long s_32b; 
#define s_32b_min LONG_MIN
#define s_32b_max LONG_MAX
// unsigned 32-bit, {0,4294967295}
typedef unsigned long u_32b;
#define u_32b_min 0
#define u_32b_max ULONG_MAX
// signed 64-bit, {-9223372036854775807,9223372036854775807}
typedef signed long long s_64b; 
#define s_64b_min LLONG_MIN
#define s_64b_max LLONG_MAX
// unsigned 64-bit, {0,18446744073709551615}
typedef unsigned long long u_64b;
#define u_64b_min 0
#define u_64b_max ULLONG_MAX
// 32-bit float, [1.17549435e-38,3.40282347e+38F] 
typedef float f_32b;
#define f_32b_min FLT_MIN
#define f_32b_max FLT_MAX
#define f_32b_dig FLT_DIG
// 64-bit float, [2.2250738585072014e-308,1.7976931348623157e+308] 
typedef double f_64b;
#define f_64b_min DBL_MIN
#define f_64b_max DBL_MAX
#define f_64b_dig DBL_DIG
// 96-bit float, [2.2250738585072014e-308L,1.7976931348623157e+308L] 
typedef long double f_96b;
#define f_96b_min LDBL_MIN
#define f_96b_max LDBL_MAX
#define f_96b_dig LDBL_DIG

#endif
