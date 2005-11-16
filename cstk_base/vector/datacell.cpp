/***************************************************************************
                             datacell.cpp  -  v0.2
                            -----------------------
    begin                : Thu Mar 11 2004
    copyright            : (C) 2004 by Stuart Reeves, Kristof Van Laerhoven
    email                :  
 **************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "datacell.h"


/* return a format descriptor, using sign (0=unsigned, 1=signed)
   number of bits (1-64), and format (integer, float)
*/
signed char DC_typecast(bool sign, int bits, signed char format)
{
  if (format==DF_INTEGER) {
    if (!sign) {
        if (bits<=8)       return U8B_TYPE;  else
	if (bits<=16)      return U16B_TYPE; else
	if (bits<=32)      return U32B_TYPE; else
	if (bits<=64)      return U64B_TYPE; else
	                   return ERR_TYPE;
    } else {
        if (bits<=8)       return S8B_TYPE;  else
	if (bits<=16)      return S16B_TYPE; else
	if (bits<=32)      return S32B_TYPE; else
	if (bits<=64)      return S64B_TYPE; else
	                   return ERR_TYPE;
    }
  } else 
  if (format==DF_FLOAT) {
     if (bits<=32)         return F32B_TYPE; else
     if (bits<=64)         return F64B_TYPE; else
     if (bits<=96)         return F64B_TYPE; else
                           return ERR_TYPE;
  } else
                           return ERR_TYPE;
}

/* return a format descriptor, using a value
   and format (integer, float)
*/
signed char FL_typecast(f_64b value, signed char format) 
{
	if ((format<20) && (format>0))
	{
		if (fmod((f_64b)(format),2.0)==0)
		{
			if (value<=(f_64b)(u_8b_max))
				return U8B_TYPE;  
			else if (value<=(f_64b)(u_16b_max))
				return U16B_TYPE; 
			else if (value<=(f_64b)(u_32b_max))
				return U32B_TYPE; 
			#ifdef U64
			else if ((unsigned long int)(value)<=u_64b_max)
				return U64B_TYPE; 
			#endif
			else
				return ERR_TYPE;
		}
		else 
		{
			if ((value<=(f_64b)(s_8b_max))
				 && ((f_64b)(value)>=s_8b_min))
					return S8B_TYPE;  
			else if ((value<=(f_64b)(s_16b_max))
				 && ((f_64b)(value)>=s_16b_min))
				return S16B_TYPE; 
			else if ((value<=(f_64b)(s_32b_max))
				 && ((f_64b)(value)>=s_32b_min))
				return S32B_TYPE; 
			#ifdef S64
			else if (((long int)(value)<=s_64b_max)
				 && ((int)(value)>=s_64b_min))
				return S64B_TYPE; 
			#endif	
			else
				return ERR_TYPE;
		}
  	}
  	else 
	{
		if ((value<=f_32b_max) && (value>=f_32b_min))
			return F32B_TYPE; else
		if ((value<=f_64b_max) && (value>=f_64b_min))
			return F64B_TYPE; else
			return ERR_TYPE;
	}
}

DataCell::DataCell(){
    type = ERR_TYPE;
    bits = 0;
}

DataCell::~DataCell(){
    if ((type==STRING_TYPE)&&(val.stringval!=NULL)) 
        delete[] val.stringval;  // do deleting if there's a string!
}

// return the value from the DataCell object as ..
bool  DataCell::get_bool()  {   return (val.boolval);}
u_8b  DataCell::get_u8b()   {   return (val.u8val);  }
u_16b DataCell::get_u16b()  {   return (val.u16val); }
u_32b DataCell::get_u32b()  {   return (val.u32val); }
u_64b DataCell::get_u64b()  {   return (val.u64val); }
s_8b  DataCell::get_s8b()   {   return (val.s8val);  }
s_16b DataCell::get_s16b()  {   return (val.s16val); }
s_32b DataCell::get_s32b()  {   return (val.s32val); }
s_64b DataCell::get_s64b()  {   return (val.s64val); }
f_32b DataCell::get_f32b()  {   return (val.f32val); }
f_64b DataCell::get_f64b()  {   return (val.f64val); }
char  DataCell::get_char()  {   return val.charval;  }
char* DataCell::get_string(){   return val.stringval;}

u_8b  DataCell::convert_u8b()
{   
	switch(type) {
		case BOOL_TYPE: return (val.boolval);
		case U8B_TYPE:  return (val.u8val);
		case U16B_TYPE: return (val.u16val)>>(bits-8);
		case U32B_TYPE: return (val.u32val)>>(bits-24);
		#ifdef U64   
		case U64B_TYPE: return (val.u64val)>>(bits-56);
		#endif
		case S8B_TYPE:  return (val.s8val+0x7F); 
		case S16B_TYPE: return (val.s16val+0x7FFF)>>(bits-8); 
		case S32B_TYPE: return (val.s32val+0x7FFFFF)>>(bits-24);
		#ifdef S64   
		case S64B_TYPE: return (val.s64val+0x7FFFFFFFFF)>>(bits-56); 
		#endif
		case F32B_TYPE: return ((u_8b)val.f32val); 
		case F64B_TYPE: return ((u_8b)val.f64val); 
		default: return 0;
	}
}

// type/bits getting, setting and testing:
signed char DataCell::get_type() { return type;}
unsigned char DataCell::get_bits() { return bits;}
void DataCell::set_type(signed char newtype) {type=newtype;}
void DataCell::set_bits(unsigned char newbits)  {bits=newbits;}
bool DataCell::test_type(signed char testtype) {return type==testtype;}

// set base type and value:
void DataCell::set_boolval(bool newval) {val.boolval=newval;type = BOOL_TYPE;}
void DataCell::set_u8val(u_8b newval)   {val.u8val=newval;  type = U8B_TYPE; }
void DataCell::set_u16val(u_16b newval) {val.u16val=newval; type = U16B_TYPE;}
void DataCell::set_u32val(u_32b newval) {val.u32val=newval; type = U32B_TYPE;}
void DataCell::set_u64val(u_64b newval) {val.u64val=newval; type = U64B_TYPE;}
void DataCell::set_s8val(s_8b newval)   {val.s8val=newval;  type = S8B_TYPE; }
void DataCell::set_s16val(s_16b newval) {val.s16val=newval; type = S16B_TYPE;}
void DataCell::set_s32val(s_32b newval) {val.s32val=newval; type = S32B_TYPE;}
void DataCell::set_s64val(s_64b newval) {val.s64val=newval; type = S64B_TYPE;}
void DataCell::set_f32val(f_32b newval) {val.f32val=newval; type = F32B_TYPE;}
void DataCell::set_f64val(f_64b newval) {val.f64val=newval; type = F64B_TYPE;}
void DataCell::set_charval(char newval) {val.charval=newval;type = CHAR_TYPE;}
void DataCell::set_stringval(char *newval) 
{
	val.stringval = new char[strlen(newval) + 1];
	strcpy(val.stringval, newval);
	type = STRING_TYPE;
}


void DataCell::set_u16val(u_8b b1, u_8b b2) 
{
	val.u16val = b1*0x100+b2;
}

void DataCell::set_u32val(u_8b b1, u_8b b2, u_8b b3, u_8b b4)
{
	val.u32val = b1*0x1000000+b2*0x10000+b3*0x100+b4;
}

void DataCell::set_u64val(u_8b b1, u_8b b2, u_8b b3, u_8b b4,
                          u_8b b5, u_8b b6, u_8b b7, u_8b b8) 
{
#ifdef U64
	val.u64val = b1*0x100000000000000 + b2*0x1000000000000 +
		b3*0x10000000000 + b4*0x100000000 + b5*0x1000000 +
		b6*0x10000 + b7*0x100 + b8;
#endif
}

void DataCell::set_s16val(u_8b b1, u_8b b2) 
{
	val.s16val = b1*0x7F+b2;
}

void DataCell::set_s32val(u_8b b1, u_8b b2, u_8b b3, u_8b b4) 
{
	val.s32val = b1*0x7FFFFFF+b2*0x10000+b3*0x100+b4;
}

void DataCell::set_s64val(u_8b b1, u_8b b2, u_8b b3, u_8b b4,
                          u_8b b5, u_8b b6, u_8b b7, u_8b b8) 
{
#ifdef S64
	val.s64val = b1*0x7FFFFFFFFFFFFF + b2*0x1000000000000 +
		b3*0x10000000000 + b4*0x100000000 + b5*0x1000000 +
		b6*0x10000 + b7*0x100 + b8;
#endif
}

f_64b DataCell::get()
{
   switch(type) {
	case BOOL_TYPE: return ((f_64b)val.boolval);
	case U8B_TYPE:  return ((f_64b)val.u8val);
	case U16B_TYPE: return ((f_64b)val.u16val); 
	case U32B_TYPE: return ((f_64b)val.u32val); 
	case U64B_TYPE: return ((f_64b)val.u64val); 
	case S8B_TYPE:  return ((f_64b)val.s8val); 
	case S16B_TYPE: return ((f_64b)val.s16val); 
	case S32B_TYPE: return ((f_64b)val.s32val);    
	case S64B_TYPE: return ((f_64b)val.s64val); 
	case F32B_TYPE: return (val.f32val); 
	case F64B_TYPE: return (val.f64val);
	default: return 0; 
   }
}

// generic set for the lazy and ignorant: :o)
// requires an observed minimum, maximum, and value
void  DataCell::set(f_64b newmin, f_64b newmax, f_64b newval)
{
	// test for floats anywhere:
	if ( isfloat(newmin) || isfloat(newmax) || isfloat(newval) )
	{
		if ((f_32b_min<=newmin)&&(newmax<=(f_64b)f_32b_max)) 
			set_f32val((f_32b) newval);
		else	set_f64val(newval); 
	}
	else if (newmin>=0)
	{	// go for unsigned: 
		if ((0<=newmin)&&(newmax<=1))                      
			set_boolval((bool) newval);
		else if ((u_8b_min<=newmin)&&(newmax<=u_8b_max))   
			set_u8val((u_8b) newval);
		else if ((u_16b_min<=newmin)&&(newmax<=u_16b_max)) 
			set_u16val((u_16b) newval);
		else if ((u_32b_min<=newmin)&&(newmax<=u_32b_max)) 
			set_u32val((u_32b) newval);
		#ifdef U64
		else if ((u_64b_min<=newmin)&&(newmax<=u_64b_max)) 
			set_u64val((u_64b) newval);
		#endif
		else if ((f_32b_min<=newmin)&&(newmax<=f_32b_max)) 
			set_f32val((f_32b) newval);
		else	set_f64val(newval); 
	}
	else
	{	// go for signed: 
		if ((s_8b_min<=newmin)&&(newmax<=s_8b_max))   
			set_u8val((s_8b) newval);
		else if ((s_16b_min<=newmin)&&(newmax<=s_16b_max)) 
			set_u16val((s_16b) newval);
		else if ((s_32b_min<=newmin)&&(newmax<=s_32b_max)) 
			set_u32val((s_32b) newval);
		#ifdef U64
		else if ((s_64b_min<=newmin)&&(newmax<=s_64b_max)) 
			set_u64val((s_64b) newval);
		#endif
		else if ((f_32b_min<=newmin)&&(newmax<=f_32b_max)) 
			set_f32val((f_32b) newval);
		else	set_f64val(newval); 
	}
}



