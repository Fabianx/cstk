/***************************************************************************
                           simsetparse.h  -  v.0.1
                             -------------------
    begin                : Wed Dec 1 2004
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

#ifndef SIMSETPARSE_H
#define SIMSETPARSE_H

#include "setparse.h"
#include <stdlib.h>  // atoi
#include "sensordata/simparser/simparser.h" // for the settings

#define _SET_SIMSET_DEF(a) strcpy(simset_val[(a)],simset_defs[(a)])

#define NUM_SIM_ATTR 1
const char simset_attr[NUM_SIM_ATTR][16] = {"size"};
const char simset_type[NUM_SIM_ATTR][16] = {"CDATA"};
const char simset_defs[NUM_SIM_ATTR][16] = {"5"};

/**Class for parsing sim settings 
  *@author Kristof Van Laerhoven
  */

class SimSetParse: public SetParse {
 public:
	SimSetParse(SimParserSettings* sim_set)
	{
		this->sim_set = sim_set;
		if (!sim_set->size)     	_SET_SIMSET_DEF(0);
	};
	~SimSetParse() {};
	int read_set(char* attr, char* value);  // read <attribute> <value> pair
	int update_set();                       // flush to sim
 	int write_set(char* buffer);            // write to xsd 
 	int write_dtd(char* buffer);            // write to dtd
	int write_tag(char* buffer) {strcpy(buffer,"sim");return 0;}

 private:
	SimParserSettings* sim_set;
	char simset_val[NUM_SIM_ATTR][256];
};

#endif

