/***************************************************************************
                            vect_types.h  -  v.1.10
                            -----------------------
    begin                : Aug 17 2002
    copyright            : (C) 2002-2003 by Kristof Van Laerhoven
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

#ifndef VECT_TYPES_H
#define VECT_TYPES_H

#include "cstk_base/types.h"

/**** CSTK Vector Type Definitions *****************************************/
/* these three types are re-organised for the vector classes               */
/***************************************************************************/
  // Vector Element type -> for the basic KVector elements only
   typedef u_8b ve_t;
   #ifndef min_ve
   #define min_ve u_8b_min // 0
   #endif
   #ifndef max_ve
   #define max_ve u_8b_max // 255
   #endif
  // Vector Element Iterator type
   typedef s_16b vei_t;
   #ifndef min_vei
   #define min_vei s_16b_min // -32767
   #endif
   #ifndef max_vei
   #define max_vei s_16b_max // 32767
   #endif
  // Vector Element Sum type -> sums the vector elements
   typedef s_32b ves_t;
   #ifndef min_ves
   #define min_ves -(max_ve*max_vei) // -8355585
   #endif
   #ifndef max_ves
   #define max_ves (max_ve*max_vei)  // 8355585
   #endif
  // OverAll Sum type -> sums about anything
   typedef f_64b oas_t;
   #ifndef min_oas
   #define min_oas f64b_min // 2.2250738585072014e-308
   #endif
   #ifndef max_oas
   #define max_oas f64b_max // 1.7976931348623157e+308
   #endif

#endif

