/***************************************************************************
                           simsetparse.cpp  -  v.0.1
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

#include "simsetparse.h"

/**Class for parsing sim settings
  *@author Kristof Van Laerhoven
  */

// read a specific attribute's value as a string
int SimSetParse::read_set(char* attr, char* value) {
	for (int i=0; i<NUM_SIM_ATTR; i++) 
		if (strcasecmp(attr,simset_attr[i])==0) { 
			strcpy(simset_val[i],value);
			return 0;
		}
	return -1;
}

// update settings from (attribute,value) table, convert from strings
int SimSetParse::update_set() {
	sim_set->size = strtol(simset_val[0],NULL,0);
	return 0;
}

// write the DTD for the settings to buffer
int SimSetParse::write_dtd(char* buffer) {
	char tmpstr[256];
	strcpy(buffer,"\t<!ELEMENT sim (channel*)>\n"); 
	for (int i=0; i<NUM_SIM_ATTR; i++) {
		sprintf(tmpstr, "\t\t<!ATTLIST   sim %s\t%s\t\"%s\">\n", 
			simset_attr[i], simset_type[i], simset_defs[i]);
		strcat(buffer,tmpstr); // add to buffer
	}
	return 0;
}

// write the sim settings in XML format to buffer 
int SimSetParse::write_set(char* buffer) {
	char tmpstr[256];
	strcpy(buffer,"\t<sim "); // start at the beginning..
	for (int i=0; i<NUM_SIM_ATTR; i++) {
		sprintf(tmpstr, "%s=\"%s\" ", simset_attr[i],
			simset_val[i]);
		strcat(buffer,tmpstr); // add to buffer
	}
	strcat(buffer,">\n");	
	return 0;
}
