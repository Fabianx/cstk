/***************************************************************************
                          inputcolumnsetparse.cpp  -  v1.00
                          ---------------------------------
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

#include "inputcolumnsetparse.h"

/**Class for parsing input column settings
  *@author Kristof Van Laerhoven
  */

// read a specific attribute's value as a string
int InputColumnSetParse::read_set(char* attr, char* value) {
	for (int i=0; i<NUM_ICOL_ATTR; i++) 
		if (strcasecmp(attr,icolset_attr[i])==0) { 
			strcpy(icolset_val[i],value);
			return 0;
		}
	return -1;
}

// update settings from (attribute,value) table, convert from strings
int InputColumnSetParse::update_set() {
	icol_set->id = strtol(icolset_val[0],NULL,0);
	icol_set->channel = strtol(icolset_val[1],NULL,0);
	strcpy(icol_set->name, icolset_val[2]);
	icol_set->bits = strtol(icolset_val[3],NULL,0);
	if (strcasecmp(icolset_val[4],"signed")==0)
			icol_set->sign = ICOL_SIGNED;
	else if (strcasecmp(icolset_val[4],"unsigned")==0)
			icol_set->sign = ICOL_UNSIGNED;
	else return -1;
	if (strcasecmp(icolset_val[5],"integer")==0)
			icol_set->format = ICOL_INT_FORMAT;
	else if (strcasecmp(icolset_val[5],"float")==0)
			icol_set->format = ICOL_FLOAT_FORMAT;
	else return -1;
	return 0;
}

// write the DTD for the settings to buffer
int InputColumnSetParse::write_dtd(char* buffer) {
	char tmpstr[256];
	strcpy(buffer,"\t<!ELEMENT inputcolumn>\n"); 
	for (int i=0; i<NUM_ICOL_ATTR; i++) {
		sprintf(tmpstr, "\t\t<!ATTLIST   inputcolumn %s\t%s\t\"%s\">\n", 
			icolset_attr[i], icolset_type[i], icolset_defs[i]);
		strcat(buffer,tmpstr); // add to buffer
	}
	return 0;
}

// write the ch settings in XML format to buffer 
int InputColumnSetParse::write_set(char* buffer) {
	char tmpstr[256];
	strcpy(buffer,"\t<inputcolumn "); // start at the beginning..
	for (int i=0; i<NUM_ICOL_ATTR; i++) {
		sprintf(tmpstr, "%s=\"%s\" ", icolset_attr[i],
			icolset_val[i]);
		strcat(buffer,tmpstr); // add to buffer
	}
	strcat(buffer,">");
	return 0;
}

