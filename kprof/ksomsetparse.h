/***************************************************************************
                           ksomsetparse.h  -  v.0.1
                             -------------------
    begin                : Wed Jan 19 2005
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

#ifndef KSOMSETPARSE_H
#define KSOMSETPARSE_H

#include "setparse.h"
#include <stdlib.h>  // atoi
#include "algorithms/ksom/ksom.h" // for the settings

#define _SET_KSOMSET_DEF(a) strcpy(ksomset_val[(a)],ksomset_defs[(a)])

#define NUM_KSOM_ATTR 13
const char ksomset_attr[NUM_KSOM_ATTR][25] = { 
	"max_x",		"max_y",	"vecdim",
	"auto_learn", 		"learn_fct", 	"learn_const_c",	"epoch",
	"neighbourhood_fct",	"neighbourhood_radius",
	"distance",		"mink_exponent",
	"mexicanhat_d",		"mexicanhat_roh" 
};
const char ksomset_type[NUM_KSOM_ATTR][256] = { 
	"CDATA",		"CDATA",	"CDATA",
	"(on|off)",		"(0|1|2)",	"CDATA",		"CDATA",
	"(0|1|2|3|4|5)",	"CDATA",	
	"(0|1|2|3)",		"CDATA",
	"CDATA",		"CDATA"
};
const char ksomset_defs[NUM_KSOM_ATTR][16] = { 
	"20",			"20",		"5",
	"off",			"0",		"0.5",			"1",
	"0",			"5",		
	"2", 			"2",
	"",			"100" 
};

/**Class for parsing rs232 settings 
  *@author Kristof Van Laerhoven
  */

class KSOMSetParse: public SetParse {
 public:
	KSOMSetParse(KSOMSettings *ksom_set)
	{
		this->ksom_set = ksom_set;
		if (!ksom_set->max_x)	_SET_KSOMSET_DEF(0);
		if (!ksom_set->max_y)	_SET_KSOMSET_DEF(1);
		if (!ksom_set->vecdim)	_SET_KSOMSET_DEF(2);
		if (!ksom_set->autol) _SET_KSOMSET_DEF(3);
		if (!ksom_set->lfct)  _SET_KSOMSET_DEF(4);
		if (!ksom_set->c) _SET_KSOMSET_DEF(5); 
		if (!ksom_set->epoch)  _SET_KSOMSET_DEF(6);	
		if (!ksom_set->nfct)  _SET_KSOMSET_DEF(7);	
		if (!ksom_set->nb_radius)    _SET_KSOMSET_DEF(8);   
		if (!ksom_set->dist)      _SET_KSOMSET_DEF(9);
		if (!ksom_set->minkexp) _SET_KSOMSET_DEF(10);
		if (!ksom_set->d) _SET_KSOMSET_DEF(11);
		if (!ksom_set->roh) _SET_KSOMSET_DEF(12);
	};
	~KSOMSetParse() {};
	int read_set(char* attr, char* value);  // read <attribute> <value> pair
	int update_set();                       // flush to ksom
	int write_set(char* buffer);            // write to xsd 
	int write_dtd(char* buffer);            // write to dtd
	int write_tag(char* buffer) {strcpy(buffer,"ksom");return 0;}

 private:
	KSOMSettings* ksom_set;
	char ksomset_val[NUM_KSOM_ATTR][256];
};

#endif

