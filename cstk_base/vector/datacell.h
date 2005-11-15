/***************************************************************************
                            datacell.h  -  v.0.1
                           ----------------------
    begin                : Fri Feb 27 2004
    copyright            : (C) 2004 by Stuart Reeves, Kristof Van Laerhoven
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

#ifndef DATACELL_H
#define DATACELL_H

/***************************************************************************
  Datacell                           
  --------      format:             sign:          guaranteed to use:
      |
      |------> bool              |              |    8b
      |------> integer           | -> signed    |    8b,  16b,  32b,  64b
      |                          | -> unsigned  |    8b,  16b,  32b,  64b
      |------> floating point    |              |               32b,  64b
      |------> character         |              |    8b
      |------> string pointer    |              |    pointer

 Examples:                                 
   * to store a value:  
          set_u16b(12078);           // straightforward
          set_u16val( 0x1F, 0x23);   // store ( 0x1F * 0x100 + 0x23 )
          set_s8b(-26); set_bits(6); // store -26 as a 6-bit signed integer
   * to retrieve a value:
          s32b t = get_s32b();       // straightforward
   
 ***************************************************************************/

#include "cstk_base/types.h"
#include <math.h>  //pow in DataCell 
#include <stdio.h>
#include <string.h>

// all types that a DataCell can hold:
#define ERR_TYPE	-1
#define NULL_TYPE	0
#define S8B_TYPE 	1  
#define U8B_TYPE	2
#define S16B_TYPE 	3
#define U16B_TYPE 	4
#define S32B_TYPE 	5
#define U32B_TYPE 	6
#define S64B_TYPE	7
#define U64B_TYPE	8
#define F32B_TYPE	20
#define F64B_TYPE	21
#define BOOL_TYPE	40
#define CHAR_TYPE	65
#define STRING_TYPE	66

// signed toggle:
#define SIGN_SIGNED	1
#define SIGN_UNSIGNED	0

// define the type of data:
#define DF_NONE  	0
#define DF_INTEGER 	1
#define DF_FLOAT	2
#define DF_CHAR  	3
#define DF_STRING	4

// Maximum string length (we assume long strings are unnecessary)
#define STRING_MAX	1024

/* return a format descriptor, using sign (0=unsigned, 1=signed) 
   number of bits (1-64), and format (integer, float)
  */
signed char DC_typecast(bool sign, int bits, signed char format);

signed char FL_typecast(double value, signed char format);

#define isfloat(a) ((a==floor(a))? 0 : 1)

/** DataCell, to be used for converting sensor data packets from 
    elsewhere to import or export into the CSTK tools, also a pretty
    critical component in the DVector class..
  */
class DataCell {
 public:     
	DataCell();
	~DataCell();
  
	unsigned char get_bits();
	signed char get_type();
	void set_bits(unsigned char newbits);
	void set_type(char newtype);
	bool test_type(char testtype);
  
	// generic get and set:
	f_64b get();
	void set(f_64b newmin, f_64b newmax, f_64b newval);
    
	// return the value from the DataCell object as an ..
	u_8b  convert_u8b();	bool  get_bool();
	u_8b  get_u8b();  	s_8b  get_s8b();
	u_16b get_u16b(); 	s_16b get_s16b();
	u_32b get_u32b(); 	s_32b get_s32b();
	u_64b get_u64b(); 	s_64b get_s64b();  
	f_32b get_f32b(); 	f_64b get_f64b();
	char  get_char(); 	char* get_string();
  
	// set the value from the DataCell object as an ..
	void set_boolval(bool newval);
	void set_u8val(u_8b newval);   void set_s8val(s_8b newval);
	void set_u16val(u_16b newval); void set_s16val(s_16b newval);
	void set_u32val(u_32b newval); void set_s32val(s_32b newval);
	void set_u64val(u_64b newval); void set_s64val(s_64b newval);
	void set_f32val(f_32b newval); void set_f64val(f_64b newval);
	void set_charval(char newval); void set_stringval(char *newval);
  
	// construct the value with unsigned byte portions: 
	void set_u16val(u_8b b1, u_8b b2);
	void set_u32val(u_8b b1, u_8b b2, u_8b b3, u_8b b4);
	void set_u64val(u_8b b1, u_8b b2, u_8b b3, u_8b b4,
			u_8b b5, u_8b b6, u_8b b7, u_8b b8);
	void set_s16val(u_8b b1, u_8b b2);
	void set_s32val(u_8b b1, u_8b b2, u_8b b3, u_8b b4);
	void set_s64val(u_8b b1, u_8b b2, u_8b b3, u_8b b4,
			u_8b b5, u_8b b6, u_8b b7, u_8b b8);
   
 private:
	signed char type; // see defines above
	union Value {
		bool	boolval;
		s_8b	s8val;  	u_8b	u8val;
		s_16b	s16val;  	u_16b	u16val;
		s_32b	s32val;  	u_32b	u32val;
		s_64b	s64val;  	u_64b	u64val;
		f_32b	f32val;  	f_64b	f64val;
		char	charval;	char*   stringval;
	} val;
	unsigned char bits; // the accurate number of bits (for scaling)
};

#endif
