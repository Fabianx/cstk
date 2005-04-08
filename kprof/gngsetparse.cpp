/***************************************************************************
                           gngparse.cpp  -  v.0.1
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

#include "gngsetparse.h"

// read a specific attribute's value as a string
int GNGSetParse::read_set(char* attr, char* value) 
{
	for (int i=0; i<NUM_GNG_ATTR; i++) 
		if (strcasecmp(attr,gngset_attr[i])==0) { 
			strcpy(gngset_val[i],value);
			return 0;
		}
	return -1;
}
	
// update settings from (attribute,value) table, convert from strings
int GNGSetParse::update_set() 
{
	gng_set->age_max = strtol(gngset_val[0],NULL,0);
	gng_set->d = strtod(gngset_val[1],NULL);
	gng_set->alpha = strtod(gngset_val[2],NULL);
	gng_set->epsilon_b = strtod(gngset_val[3],NULL);
	gng_set->epsilon_n = strtod(gngset_val[4],NULL);
	gng_set->vecdim = strtol(gngset_val[5],NULL,0);
	return 0;
}

// write the DTD for the settings to buffer
int GNGSetParse::write_dtd(char* buffer) 
{
	char tmpstr[256];
	strcpy(buffer,"\t<!ELEMENT gng>\n"); 
	for (int i=0; i<NUM_GNG_ATTR; i++) {
		sprintf(tmpstr, "\t\t<!ATTLIST   gng %s\t%s\t\"%s\">\n", 
			gngset_attr[i], gngset_type[i], gngset_defs[i]);
		strcat(buffer,tmpstr); // add to buffer
	}
	return 0;
}

int GNGSetParse::write_set(char* buffer) 
{
	char tmpstr[256];
	strcpy(buffer,"\t<gng "); // start at the beginning..
	for (int i=0; i<NUM_GNG_ATTR; i++) {
		sprintf(tmpstr, "%s=\"%s\" ", gngset_attr[i],
			gngset_val[i]);
		strcat(buffer,tmpstr); // add to buffer
	}
	strcat(buffer,">\n");	
	return 0;
}
