/***************************************************************************
                           windowparse.cpp  -  v.0.1
                             -------------------
    begin                : Wed Oct 20 2004
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

#include "windowsetparse.h"

/**Class for parsing window settings
  *@author Kristof Van Laerhoven
  */

// read a specific attribute's value as a string
int WindowSetParse::read_set(char* attr, char* value) 
{
	for (int i=0; i<NUM_WINDOW_ATTR; i++) 
		if (strcasecmp(attr,windowset_attr[i])==0) { 
			strcpy(windowset_val[i],value);
			return 0;
		}
	return -1;
}
	
// update settings from (attribute,value) table, convert from strings
int WindowSetParse::update_set() 
{
	window_set->xpos      = strtol(windowset_val[0],NULL,0);
	window_set->ypos      = strtol(windowset_val[1],NULL,0);
	window_set->width     = strtol(windowset_val[2],NULL,0);
	window_set->height    = strtol(windowset_val[3],NULL,0);
	window_set->border    = strtol(windowset_val[4],NULL,0);
	window_set->framerate = strtol(windowset_val[5],NULL,0);
	window_set->skip      = strtol(windowset_val[6],NULL,0);
	return 0;
}

// write the DTD for the settings to buffer
int WindowSetParse::write_dtd(char* buffer) 
{
	char tmpstr[256];
	strcpy(buffer,"\t<!ELEMENT window (plot*)>\n"); 
	for (int i=0; i<NUM_WINDOW_ATTR; i++) {
		sprintf(tmpstr, "\t\t<!ATTLIST   window %s\t%s\t\"%s\">\n", 
			windowset_attr[i], windowset_type[i], windowset_defs[i]);
		strcat(buffer,tmpstr); // add to buffer
	}
	return 0;
}

// write the window settings in XML format to buffer 
int WindowSetParse::write_set(char* buffer) 
{
	char tmpstr[256];
	strcpy(buffer,"<window "); // start at the beginning..
	for (int i=0; i<NUM_WINDOW_ATTR; i++) {
		sprintf(tmpstr, "%s=\"%s\" ", windowset_attr[i],
			windowset_val[i]);
		strcat(buffer,tmpstr); // add to buffer
	}
	strcat(buffer,">\n");	
	return 0;
}
