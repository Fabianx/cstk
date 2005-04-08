/***************************************************************************
                              pparse.h  -  v1.00
                             -------------------
    begin                : Fri Nov 14 2003
    copyright            : (C) 2003 by Kristof Van Laerhoven
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
#ifndef PPARSE_H
#define PPARSE_H

#include "kprof/tags.h"
#include "kprof/setparse.h"
#include <stdio.h>    // FILE
#include <stdlib.h>   // atof
#include <strings.h>  // strcasecmp

// generic name/value parameter: <param name="" value=""/>
#define _SET_PARAMSET_DEF(a) strcpy(paramset_val[(a)],paramset_defs[(a)])
#define NUM_PARAM_ATTR 2
const char paramset_attr[NUM_PARAM_ATTR][16] = { "name", "value" };
const char paramset_type[NUM_PARAM_ATTR][16] = { "CDATA","CDATA" };
const char paramset_defs[NUM_PARAM_ATTR][16] = { "", "" }; 

struct ParamSettings {
	ParamSettings() {
		name[0]='\0';
		value[0]='\0';
	}
	char name[256];
	char value[256];
	ParamSettings *next;
};

class ParamSetParse : public SetParse
{
 public:
	ParamSetParse(ParamSettings* param_set) {
		this->param_set = param_set;
		if (!param_set->name[0])  _SET_PARAMSET_DEF(0);
		if (!param_set->value[0]) _SET_PARAMSET_DEF(1);
	};
	~ParamSetParse() {};
	int read_set(char* attr, char* value) { // read <attribute> <value> 
		for (int i=0; i<NUM_PARAM_ATTR; i++) 
			if (strcasecmp(attr,paramset_attr[i])==0) { 
				strcpy(paramset_val[i],value);
				return 0;
			}
		return -1;
	};
	int update_set() {
		strcpy(param_set->name, paramset_val[0]);
		strcpy(param_set->value, paramset_val[1]);
		return 0;
	};
	int write_set(char* buffer) { 
		char tmpstr[256];
		strcpy(buffer,"\t<param ");
		for (int i=0; i<NUM_PARAM_ATTR; i++) {
			sprintf(tmpstr, "%s=\"%s\" ", paramset_attr[i],
				paramset_val[i]);
			strcat(buffer,tmpstr); // add to buffer
		}
		strcat(buffer,">");
		return 0;
	};
	int write_dtd(char* buffer) {return 0;}; // stub
	int write_tag(char* buffer) {strcpy(buffer,"param");return 0;}
 private:
	ParamSettings* param_set;
	char paramset_val[NUM_PARAM_ATTR][256];
};

/***************************************************************************
   PParse parses CSTK settings (formatted as an XML file) for generic 
   algorithm settings, located between two <params> tags. It returns the
   parsed values as integers, floating point numbers, or strings.
 ***************************************************************************/	
class PParse 
{
 public:
	PParse();
	~PParse();
	int parse(FILE* fp);
	int export_dtd(char* buffer);
	int export_xsd(char* buffer);
	void init(FILE* fp);
	void export_err(char* buffer); // print error string in buffer
	bool error() {return err;}
	
	bool  get_bool( char *name );
	char* get_string( char *name );
	double get_float( char *name );
	long int get_int( char *name );

 private:
	void parse_section(FILE* fp, char* tmpstr, unsigned int line, 
	                   bool valid_sub_tag, bool valid_att_tag);
	SetParse* *sp; 	// dynamic array of all settings tags
	unsigned int sp_size; 	// 
	ParamSettings *paramset, *parampset;
	int err;
	unsigned int errline;
};

#endif // PPARSE_H

