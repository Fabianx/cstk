/***************************************************************************
                              channelsetparse.h  -  v1.00
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

#define CH_INT_FORMAT   1
#define CH_FLOAT_FORMAT 2

#define CH_SIGNED   1
#define CH_UNSIGNED 2

#define _SET_CHSET_DEF(a) strcpy(chset_val[(a)],chset_defs[(a)])

#define NUM_CH_ATTR 5
const char chset_attr[NUM_CH_ATTR][16] = {
	"id", "name", "bits", "sign", "format" };
const char chset_type[NUM_CH_ATTR][20] = {
	"CDATA", "CDATA","CDATA", "(signed|unsigned)", "(integer|float)"};
const char chset_defs[NUM_CH_ATTR][16] = {
	"0", "unknown", "8", "unsigned", "integer"};

// channel:
struct ChannelSettings{
   ChannelSettings() {
	id = -1;
	name[0]='\0';
	bits = 0;
	sign = 0;
	format = 0;
	next = NULL;
   }
   int id;
   char name[256];
   int bits;
   bool sign;
   char format;
   ChannelSettings *next;
};

class ChannelSetParse: public SetParse {
 public:
	ChannelSetParse(ChannelSettings* ch_set)
	{
		this->ch_set = ch_set;
		if (ch_set->id<0)      	_SET_CHSET_DEF(0);
		if (!ch_set->name)     	_SET_CHSET_DEF(1);
		if (!ch_set->bits)     	_SET_CHSET_DEF(2);
		if (!ch_set->sign)     	_SET_CHSET_DEF(3);
		if (!ch_set->format)   	_SET_CHSET_DEF(4);
	};
	~ChannelSetParse() {};
	int read_set(char* attr, char* value);  // read <attribute> <value> pair
	int update_set();                       // flush to ch
	int write_set(char* buffer);            // write to xsd
	int write_dtd(char* buffer);            // write to dtd
	int write_tag(char* buffer) {strcpy(buffer,"channel");return 0;}
	int write_set(void** set) {*set = ch_set;return 0;}
 private:
	ChannelSettings* ch_set;
	char chset_val[NUM_CH_ATTR][256];
};

