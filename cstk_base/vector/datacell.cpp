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
char DC_typecast(bool sign, int bits, char format){
    
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

char FL_typecast(double value, char format){
    
	if (format<20)
	{
		if (fmod((double)(format),2.0)==0)
		{
			if (value<=(double)(u_8b_max))	return U8B_TYPE;  
			else if (value<=(double)(u_16b_max))	return U16B_TYPE; 
			else if (value<=(double)(u_32b_max))	return U32B_TYPE; 
			#ifdef U64
			else if ((unsigned long int)(value)<=u_64b_max)	return U64B_TYPE; 
			#endif
			else
				return ERR_TYPE;
		}
		else 
		{
			if ((value<=(double)(s_8b_max)) && ((double)(value)>=s_8b_min))		return S8B_TYPE;  
			else if ((value<=(double)(s_16b_max)) && ((double)(value)>=s_16b_min))	return S16B_TYPE; 
			else if ((value<=(double)(s_32b_max)) && ((double)(value)>=s_32b_min))	return S32B_TYPE; 
			#ifdef S64
			else if (((long int)(value)<=s_64b_max) && ((int)(value)>=s_64b_min))	return S64B_TYPE; 
			#endif	
			else
				return ERR_TYPE;
		}
  	} 
  	else 
	{
		if ((value<=f_32b_max) && (value>=f_32b_min))	return F32B_TYPE; else
		if ((value<=f_64b_max) && (value>=f_64b_min))	return F64B_TYPE; else
								return ERR_TYPE;
	}
}

DataCell::DataCell(){
    type = ERR_TYPE;
}

DataCell::~DataCell(){
    if ((type==STRING_TYPE)&&(val.stringval!=NULL)) 
        delete[] val.stringval;  // do deleting if there's a string!
}

// return the value from the DataCell object as an unsigned 8 bit
u_8b DataCell::get_u8b() {

   switch(type) {
	case U8B_TYPE:  return (val.u8val); break;
	case U16B_TYPE: return (val.u16val/(u_8b)(pow(2,bits-8))); break;
	case U32B_TYPE: return (val.u32val/(u_8b)(pow(2,bits-24))); break;
	case U64B_TYPE: return (val.u64val/(u_8b)(pow(2,bits-56))); break;
	case S8B_TYPE:  return (val.s8val+0x7F); break;
	case S16B_TYPE: return ((val.s16val+0x7F)/0xFF); break;
	case S32B_TYPE: return ((val.s32val+0x7FFF)/0xFFFF); break;   
	case S64B_TYPE: return ((val.s64val+0x7FFFFFFF)/0xFFFFFFFF); break;
	case F32B_TYPE: return ((int)val.f32val); break;
	case F64B_TYPE: return (u_8b) type; break; 
	case F96B_TYPE: return (u_8b) type; break;
	default: return 0; break;   
   }
    
}

// return the value from the DataCell object as an unsigned 16 bit
u_16b DataCell::get_u16b() {

   switch(type) {
	case U8B_TYPE:  return (val.u8val); break;
	case U16B_TYPE: return (val.u16val); break;
	case U32B_TYPE: return (val.u32val/0xFF); break;
	case U64B_TYPE: return (val.u64val/0xFFFF); break;
	case S8B_TYPE:  return (val.s8val+0x7F); break;
	case S16B_TYPE: return (val.s16val+0x7F); break;
	case S32B_TYPE: return ((val.s32val+0x7F)/0xFF); break;   
	case S64B_TYPE: return ((val.s64val+0x7FFF)/0xFFFF); break;
	case F32B_TYPE: return (u_16b) type; break;
	case F64B_TYPE: return (u_16b) type; break; 
	case F96B_TYPE: return (u_16b) type; break;
	default: return 0; break;   
   }
    
}

// return the value from the DataCell object as an unsigned 32 bit
u_32b DataCell::get_u32b() {

   switch(type) {
	case U8B_TYPE:  return (val.u8val); break;
	case U16B_TYPE: return (val.u16val); break;
	case U32B_TYPE: return (val.u32val); break;
	case U64B_TYPE: return (val.u64val/0xFFFF); break;
	case S8B_TYPE:  return (val.s8val+0x7F); break;
	case S16B_TYPE: return (val.s16val+0x7F); break;
	case S32B_TYPE: return (val.s32val+0x7F); break;   
	case S64B_TYPE: return ((val.s64val+0x7FFF)/0xFFFF); break;
	case F32B_TYPE: return (u_32b) type; break;
	case F64B_TYPE: return (u_32b) type; break; 
	case F96B_TYPE: return (u_32b) type; break;
	default: return 0; break;   
   }
    
}

// return the value from the DataCell object as an unsigned 64 bit
u_64b DataCell::get_u64b() {

   switch(type) {
	case U8B_TYPE:  return (val.u8val); break;
	case U16B_TYPE: return (val.u16val); break;
	case U32B_TYPE: return (val.u32val); break;
	case U64B_TYPE: return (val.u64val); break;
	case S8B_TYPE:  return (val.s8val+0x7F); break;
	case S16B_TYPE: return (val.s16val+0x7F); break;
	case S32B_TYPE: return (val.s32val+0x7F); break;   
	case S64B_TYPE: return (val.s64val+0x7FFF); break;
	case F32B_TYPE: return (u_64b) type; break;
	case F64B_TYPE: return (u_64b) type; break; 
	case F96B_TYPE: return (u_64b) type; break;
	default: return 0; break;   
   }
    
}

// return the value from the DataCell object as a signed 8 bit
s_8b DataCell::get_s8b() {

   switch(type) {
	case U8B_TYPE:  return (val.u8val); break;
	case U16B_TYPE: return (val.u16val/(s_8b)(pow(2,bits-8))); break;
	case U32B_TYPE: return (val.u32val/(s_8b)(pow(2,bits-24))); break;
	case U64B_TYPE: return (val.u64val/(s_8b)(pow(2,bits-56))); break;
	case S8B_TYPE:  return (val.s8val); break;
	case S16B_TYPE: return ((val.s16val)/0xFF); break;
	case S32B_TYPE: return ((val.s32val)/0xFFFF); break;   
	case S64B_TYPE: return ((val.s64val)/0xFFFFFFFF); break;
	case F32B_TYPE: return (s_8b) type; break;
	case F64B_TYPE: return (s_8b) type; break; 
	case F96B_TYPE: return (s_8b) type; break;
	default: return 0; break;   
   }

}

// return the value from the DataCell object as a signed 16 bit
s_16b DataCell::get_s16b() {

   switch(type) {
	case U8B_TYPE:  return (val.u8val); break;
	case U16B_TYPE: return (val.u16val); break;
	case U32B_TYPE: return (val.u32val/(s_16b)(pow(2,bits-24))); break;
	case U64B_TYPE: return (val.u64val/(s_16b)(pow(2,bits-56))); break;
	case S8B_TYPE:  return (val.s8val); break;
	case S16B_TYPE: return (val.s16val); break;
	case S32B_TYPE: return ((val.s32val)/0xFFFF); break;   
	case S64B_TYPE: return ((val.s64val)/0xFFFFFFFF); break;
	case F32B_TYPE: return (s_16b) type; break;
	case F64B_TYPE: return (s_16b) type; break; 
	case F96B_TYPE: return (s_16b) type; break;
	default: return 0; break;   
   }

}

// return the value from the DataCell object as a signed 32 bit
s_32b DataCell::get_s32b() {

   switch(type) {
	case U8B_TYPE:  return (val.u8val); break;
	case U16B_TYPE: return (val.u16val); break;
	case U32B_TYPE: return (val.u32val); break;
	case U64B_TYPE: return (val.u64val/(s_32b)(pow(2,bits-56))); break;
	case S8B_TYPE:  return (val.s8val); break;
	case S16B_TYPE: return (val.s16val); break;
	case S32B_TYPE: return (val.s32val); break;   
	case S64B_TYPE: return ((val.s64val)/0xFFFFFFFF); break;
	case F32B_TYPE: return (s_32b) type; break;
	case F64B_TYPE: return (s_32b) type; break; 
	case F96B_TYPE: return (s_32b) type; break;
	default: return 0; break;   
   }

}

// return the value from the DataCell object as a signed 64 bit
s_64b DataCell::get_s64b() {

   switch(type) {
	case U8B_TYPE:  return (val.u8val); break;
	case U16B_TYPE: return (val.u16val); break;
	case U32B_TYPE: return (val.u32val); break;
	case U64B_TYPE: return (val.u64val); break;
	case S8B_TYPE:  return (val.s8val); break;
	case S16B_TYPE: return (val.s16val); break;
	case S32B_TYPE: return (val.s32val); break;   
	case S64B_TYPE: return (val.s64val); break;
	case F32B_TYPE: return (s_64b) type; break;
	case F64B_TYPE: return (s_64b) type; break; 
	case F96B_TYPE: return (s_64b) type; break;
	default: return 0; break;   
   }

}

// return the value from the DataCell object as a float 32 bit
f_32b DataCell::get_f32b() {

   switch(type) {
	case U8B_TYPE:  return (val.u8val); break;
	case U16B_TYPE: return (val.u16val); break;
	case U32B_TYPE: return (val.u32val); break;
	case U64B_TYPE: return (val.u64val); break;
	case S8B_TYPE:  return (val.s8val); break;
	case S16B_TYPE: return (val.s16val); break;
	case S32B_TYPE: return (val.s32val); break;   
	case S64B_TYPE: return (val.s64val); break;
	case F32B_TYPE: return (val.f32val); break;
	case F64B_TYPE: return (f_32b) type; break; 
	case F96B_TYPE: return (f_32b) type; break;
	default: return 0; break;   
   }

}

// return the value from the DataCell object as a float 64 bit
f_64b DataCell::get_f64b() {

   switch(type) {
	case U8B_TYPE:  return (val.u8val); break;
	case U16B_TYPE: return (val.u16val); break;
	case U32B_TYPE: return (val.u32val); break;
	case U64B_TYPE: return (val.u64val); break;
	case S8B_TYPE:  return (val.s8val); break;
	case S16B_TYPE: return (val.s16val); break;
	case S32B_TYPE: return (val.s32val); break;   
	case S64B_TYPE: return (val.s64val); break;
	case F32B_TYPE: return (val.f32val); break;
	case F64B_TYPE: return (val.f64val); break; 
	case F96B_TYPE: return (f_64b) type; break;
	default: return 0; break;
   }

}

// return the value from the DataCell object as a char
char DataCell::get_char() {
	switch (type) {
		case CHAR_TYPE: 
			return val.charval; 
			break;
		default:
			return (char)type;
			break;
	}
}

// return the value from the DataCell object as a string
char *DataCell::get_string() {
	switch (type) {
		case CHAR_TYPE:
			return 0;
			//return (char*)val.charval; // error
		case STRING_TYPE:
			return val.stringval;
		default:
			// TODO: Crashes
			return NULL;//(char*)type;
			break;
	}
}

char DataCell::get_type() {
	return type;
}

void DataCell::set_type(char newtype) {
	type=newtype;
}

bool DataCell::test_type(char testtype) {
	return type==testtype;
}
  
void DataCell::set_u8val(u_8b newval) {
	val.u8val=newval;    
	type = U8B_TYPE;
}

void DataCell::set_u16val(u_16b newval) {
	val.u16val=newval; 
	type = U16B_TYPE;
}
  
void DataCell::set_u32val(u_32b newval) {
	val.u32val=newval; 
	type = U32B_TYPE;
}

void DataCell::set_u64val(u_64b newval) {
	val.u64val=newval; 
	type = U64B_TYPE;
}

void DataCell::set_s8val(s_8b newval) {
	val.s8val=newval;    
	type = S8B_TYPE;
}

void DataCell::set_s16val(s_16b newval) {
	val.s16val=newval; 
	type = S16B_TYPE;
}

void DataCell::set_s32val(s_32b newval) {
	val.s32val=newval; 
	type = S32B_TYPE;
}

void DataCell::set_s64val(s_64b newval) {
	val.s64val=newval; 
	type = S64B_TYPE;
}

void DataCell::set_f32val(f_32b newval) {
	val.f32val=newval; 
	type = F32B_TYPE;
}

void DataCell::set_f64val(f_64b newval) {
	val.f64val=newval; 
	type = F64B_TYPE;
}

void DataCell::set_charval(char newval) {
	val.charval=newval;
	type = CHAR_TYPE;
}

void DataCell::set_stringval(char *newval) {
	val.stringval = new char[strlen(newval) + 1];
	strcpy(val.stringval, newval);
	type = STRING_TYPE;
}

void DataCell::set_u16val(u_8b b1, u_8b b2) {
  val.u16val = b1*0x100+b2;
}

void DataCell::set_u32val(u_8b b1, u_8b b2, u_8b b3, u_8b b4) {
  val.u32val = b1*0x1000000+b2*0x10000+b3*0x100+b4;
}

void DataCell::set_u64val(u_8b b1, u_8b b2, u_8b b3, u_8b b4,
                          u_8b b5, u_8b b6, u_8b b7, u_8b b8) {
#ifdef U64
  val.u64val = b1*0x100000000000000 + b2*0x1000000000000 +
               b3*0x10000000000 + b4*0x100000000 + b5*0x1000000 +
	       b6*0x10000 + b7*0x100 + b8;
#endif
}

void DataCell::set_s16val(u_8b b1, u_8b b2) {
  val.s16val = b1*0x7F+b2;
}

void DataCell::set_s32val(u_8b b1, u_8b b2, u_8b b3, u_8b b4) {
  val.s32val = b1*0x7FFFFFF+b2*0x10000+b3*0x100+b4;
}

void DataCell::set_s64val(u_8b b1, u_8b b2, u_8b b3, u_8b b4,
                          u_8b b5, u_8b b6, u_8b b7, u_8b b8) {
#ifdef S64
  val.s64val = b1*0x7FFFFFFFFFFFFF + b2*0x1000000000000 +
               b3*0x10000000000 + b4*0x100000000 + b5*0x1000000 +
	       b6*0x10000 + b7*0x100 + b8;
#endif
}

void DataCell::set_bits(int newbits) {
   bits = newbits;
}

int DataCell::get_bits() {
   return bits;
}

void DataCell::operator +=(s_16b value){
   switch(type) {
	case U8B_TYPE:  val.u8val += (u_8b)value; break;
	case U16B_TYPE: val.u16val+= (u_16b)value; break;
	case U32B_TYPE: val.u32val+= (u_32b)value; break;
	case U64B_TYPE: val.u64val+= (u_64b)value; break;
	case S8B_TYPE:  val.s8val += (s_8b)value; break;
	case S16B_TYPE: val.s16val+= (s_16b)value; break;
	case S32B_TYPE: val.s32val+= (s_32b)value; break;   
	case S64B_TYPE: val.s64val+= (s_64b)value; break;
	case F32B_TYPE: val.f32val+= (f_32b)value; break;
	case F64B_TYPE: val.f64val+= (f_32b)value; break; 
	case F96B_TYPE: val.f64val+= (f_32b)value; break;
	default: /*?*/; break;
   }
}

void DataCell::operator +=(f_32b value){
   switch(type) {
	case U8B_TYPE:  val.u8val += (u_8b)value; break;
	case U16B_TYPE: val.u16val+= (u_16b)value; break;
	case U32B_TYPE: val.u32val+= (u_32b)value; break;
	case U64B_TYPE: val.u64val+= (u_64b)value; break;
	case S8B_TYPE:  val.s8val += (s_8b)value; break;
	case S16B_TYPE: val.s16val+= (s_16b)value; break;
	case S32B_TYPE: val.s32val+= (s_32b)value; break;   
	case S64B_TYPE: val.s64val+= (s_64b)value; break;
	case F32B_TYPE: val.f32val+= value; break;
	case F64B_TYPE: val.f64val+= value; break; 
	case F96B_TYPE: val.f64val+= value; break;
	default: /*?*/; break;
   }
}

bool DataCell::operator <(f_32b value){
   switch(type) {
	case U8B_TYPE:  return ((f_32b)val.u8val  < value); break;
	case U16B_TYPE: return ((f_32b)val.u16val < value); break;
	case U32B_TYPE: return ((f_32b)val.u32val < value); break;
	case U64B_TYPE: return ((f_32b)val.u64val < value); break;
	case S8B_TYPE:  return ((f_32b)val.s8val  < value); break;
	case S16B_TYPE: return ((f_32b)val.s16val < value); break;
	case S32B_TYPE: return ((f_32b)val.s32val < value); break;   
	case S64B_TYPE: return ((f_32b)val.s64val < value); break;
	case F32B_TYPE: return (val.f32val < value);        break;
	case F64B_TYPE: return (val.f64val < (f_32b)value); break; 
	case F96B_TYPE: return (val.f64val < (f_32b)value); break;
	default: return 0; break;
   }
}

bool DataCell::operator >(f_32b value){
   switch(type) {
	case U8B_TYPE:  return ((f_32b)val.u8val  > value); break;
	case U16B_TYPE: return ((f_32b)val.u16val > value); break;
	case U32B_TYPE: return ((f_32b)val.u32val > value); break;
	case U64B_TYPE: return ((f_32b)val.u64val > value); break;
	case S8B_TYPE:  return ((f_32b)val.s8val  > value); break;
	case S16B_TYPE: return ((f_32b)val.s16val > value); break;
	case S32B_TYPE: return ((f_32b)val.s32val > value); break;   
	case S64B_TYPE: return ((f_32b)val.s64val > value); break;
	case F32B_TYPE: return (val.f32val > value);        break;
	case F64B_TYPE: return (val.f64val > (f_64b)value); break; 
	case F96B_TYPE: return (val.f64val > (f_32b)value); break;
	default: return 0; break;
   }
}

