/***************************************************************************
                           gngsetparse.h  -  v.0.1
                             -------------------
    begin                : Fri Apr 08 2005
    copyright            : (C) 2005 by Martin Berchtold
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

#ifndef GNGSETPARSE_H
#define GNGSETPARSE_H

#include "setparse.h"
#include <stdlib.h>  // atoi
#include "algorithms/gng/gng.h" // for the settings

#define _SET_GNGSET_DEF(a) strcpy(gngset_val[(a)],gngset_defs[(a)])

#define NUM_GNG_ATTR 6

const char gngset_attr[NUM_GNG_ATTR][25] = { 
	"age_max",		"d",		"alpha",
	"epsilon_b", 		"epsilon_n",
	"vecdim"
};
const char gngset_type[NUM_GNG_ATTR][256] = { 
	"CDATA",		"CDATA",	"CDATA",
	"CDATA",		"CDATA",
	"CDATA"
};
const char gngset_defs[NUM_GNG_ATTR][16] = { 
	"10",			"0.8",		"0.8",
	"0.05",			"0.01",
	"4"
};

class GNGSetParse: public SetParse {
 public:
	GNGSetParse(GNG_PARAM *gng_set)
	{
		this->gng_set = gng_set;
		if (!gng_set->age_max)	_SET_GNGSET_DEF(0);
		if (!gng_set->d)	_SET_GNGSET_DEF(1);
		if (!gng_set->alpha)	_SET_GNGSET_DEF(2);
		if (!gng_set->epsilon_b) _SET_GNGSET_DEF(3);
		if (!gng_set->epsilon_n)  _SET_GNGSET_DEF(4);
		if (!gng_set->vecdim)  _SET_GNGSET_DEF(4);
	};
	~GNGSetParse() {};
	int read_set(char* attr, char* value);  // read <attribute> <value> pair
	int update_set();                       // flush to gng
	int write_set(char* buffer);            // write to xsd 
	int write_dtd(char* buffer);            // write to dtd
	int write_tag(char* buffer) {strcpy(buffer,"gng");return 0;}

 private:
	GNG_PARAM* gng_set;
	char gngset_val[NUM_GNG_ATTR][256];
};

#endif