/***************************************************************************
                           datacell_t.cpp  -  v.1.00
                           --------------------------
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

 /**Datacell Demo
  *@author Kristof Van Laerhoven

   This demo shows what the datacell class can be used for - most of the 
   explanations get printed, so just run it and read.
 */

#include "cstk_base/vector/datacell.h"
#include <sys/time.h>     // for timings

int main(void) {
 
	DataCell dc;

	printf("testing the datacell routines:\n\r");

	dc.set_u16val(12345); dc.set_bits(16);
	printf(" + set_u16val: value stored is %g\tbits: %i\ttype: %i\n\r", 
	dc.get(), dc.get_bits(), dc.get_type() );

	dc.set_s8val(-45); dc.set_bits(8);
	printf(" + set_s8val:  value stored is %g\tbits: %i \ttype: %i\n\r", 
	dc.get(), dc.get_bits(), dc.get_type() );
  
	dc.set(0,5,3.2);
	printf(" + set :       value stored is %g\tbits: %i \ttype: %i\n\r", 
	dc.get(), dc.get_bits(), dc.get_type() );

	dc.set(0,18566, 18565);
	printf(" + set :       value stored is %g\tbits: %i \ttype: %i\n\r", 
	dc.get(), dc.get_bits(), dc.get_type() );

	dc.set(-100.23, 18565.7812, -100);
	printf(" + set :       value stored is %g\tbits: %i \ttype: %i\n\r", 
	dc.get(), dc.get_bits(), dc.get_type() );

	char str[] = "this is a test";
	dc.set_stringval(str);
	printf(" + set_string: '%s'\n\r", dc.get_string() );

	dc.set_u16val(1023); dc.set_bits(12);
	printf(" + set_u16val: value stored is %g\tbits: %i\ttype: %i\n\r", 
	dc.get(), dc.get_bits(), dc.get_type() );
	printf(" + convert_u8b: %g\n\r", (double)dc.convert_u8b() );
	
	dc.set_u32val(56);    dc.set_bits(15);  
	printf(" + set_u16val: value stored is %g\tbits: %i\ttype: %i\n\r", 
	dc.get(), dc.get_bits(), dc.get_type() );
	printf(" + convert_u8b: %g\n\r", (double)dc.convert_u8b() );
	
	dc.set_s16val(-5600);  dc.set_bits(16);
	printf(" + set_u16val: value stored is %g\tbits: %i\ttype: %i\n\r", 
	dc.get(), dc.get_bits(), dc.get_type() );
	printf(" + convert_u8b: %g\n\r", (double)dc.convert_u8b() );

	return 0;
}


