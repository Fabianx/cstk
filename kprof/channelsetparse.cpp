/***************************************************************************
                          channelsetparse.cpp  -  v1.00
                          -----------------------------
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

#include "channelsetparse.h"

/**Class for parsing channel settings
  *@author Kristof Van Laerhoven
  */

// read a specific attribute's value as a string
int ChannelSetParse::read_set(char* attr, char* value) {
	for (int i=0; i<NUM_CH_ATTR; i++) 
		if (strcasecmp(attr,chset_attr[i])==0) { 
			strcpy(chset_val[i],value);
			return 0;
		}
	return -1;
}

// update settings from (attribute,value) table, convert from strings
int ChannelSetParse::update_set() {
	ch_set->id = strtol(chset_val[0],NULL,0);
	strcpy(ch_set->name, chset_val[1]);
	ch_set->bits = strtol(chset_val[2],NULL,0);
	if (strcasecmp(chset_val[3],"signed")==0)
			ch_set->sign = CH_SIGNED;
	else if (strcasecmp(chset_val[3],"unsigned")==0)
			ch_set->sign = CH_UNSIGNED;
	else return -1;
	if (strcasecmp(chset_val[4],"integer")==0)
			ch_set->format = CH_INT_FORMAT;
	else if (strcasecmp(chset_val[4],"float")==0)
			ch_set->format = CH_FLOAT_FORMAT;
	else return -1;
	return 0;
}

// write the DTD for the settings to buffer
int ChannelSetParse::write_dtd(char* buffer) {
	char tmpstr[256];
	strcpy(buffer,"\t<!ELEMENT channel>\n"); 
	for (int i=0; i<NUM_CH_ATTR; i++) {
		sprintf(tmpstr, "\t\t<!ATTLIST   channel %s\t%s\t\"%s\">\n", 
			chset_attr[i], chset_type[i], chset_defs[i]);
		strcat(buffer,tmpstr); // add to buffer
	}
	return 0;
}

// write the ch settings in XML format to buffer 
int ChannelSetParse::write_set(char* buffer) {
	char tmpstr[256];
	strcpy(buffer,"\t\t<channel "); // start at the beginning..
	for (int i=0; i<NUM_CH_ATTR; i++) {
		sprintf(tmpstr, "%s=\"%s\" ", chset_attr[i],
			chset_val[i]);
		strcat(buffer,tmpstr); // add to buffer
	}
	strcat(buffer,">");
	return 0;
}
