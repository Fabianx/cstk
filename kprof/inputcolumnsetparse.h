/***************************************************************************
                              inputcolumnsetparse.h  -  v1.00
                              ---------------------------
    begin                : Thu Dec 2 2004
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

/**Parser for CSTK input
  *@author Kristof Van Laerhoven
*/ 
 
#include "setparse.h"
#include <stdlib.h>  // strtol

#define ICOL_INT_FORMAT   1
#define ICOL_FLOAT_FORMAT 2

#define ICOL_SIGNED   1
#define ICOL_UNSIGNED 2

#define _SET_ICOLSET_DEF(a) strcpy(icolset_val[(a)],icolset_defs[(a)])

#define NUM_ICOL_ATTR 6
const char icolset_attr[NUM_ICOL_ATTR][16] = {
	"id", "channel", "name", "bits", "sign", "format" };
const char icolset_type[NUM_ICOL_ATTR][20] = {
	"CDATA", "CDATA", "CDATA", "CDATA", 
	"(signed|unsigned)", "(integer|float)"};
const char icolset_defs[NUM_ICOL_ATTR][16] = {
	"0", "0", "unknown", "8", "unsigned", "integer"};


struct InputColumnSettings{
   InputColumnSettings() {
	id = -1;
	channel = -1;
	name[0]='\0';
	bits = 0;
	sign = 0;
	format = 0;
	next = NULL;
   }
   int id;
   int channel;
   char name[256];
   int bits;
   bool sign;
   char format;
   InputColumnSettings *next;
};


class InputColumnSetParse: public SetParse {
 public:
	InputColumnSetParse(InputColumnSettings* icol_set)
	{
		this->icol_set = icol_set;
		if (icol_set->id<0)      	_SET_ICOLSET_DEF(0);
		if (icol_set->channel<0)      	_SET_ICOLSET_DEF(1);
		if (!icol_set->name)     	_SET_ICOLSET_DEF(2);
		if (!icol_set->bits)     	_SET_ICOLSET_DEF(3);
		if (!icol_set->sign)     	_SET_ICOLSET_DEF(4);
		if (!icol_set->format)   	_SET_ICOLSET_DEF(5);
	};
	~InputColumnSetParse() {};
	int read_set(char* attr, char* value);  // read <attribute> <value> pair
	int update_set();                       // flush to ch
	int write_set(char* buffer);            // write to xsd 
	int write_dtd(char* buffer);            // write to dtd
	int write_tag(char* buffer) {strcpy(buffer,"inputcolumn");return 0;}
	
 private:
	InputColumnSettings* icol_set;
	char icolset_val[NUM_ICOL_ATTR][256];
};
