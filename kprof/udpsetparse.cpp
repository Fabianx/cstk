/***************************************************************************
                           udpsetparse.cpp  -  v.0.1
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

#include "udpsetparse.h"

/**Class for parsing udp settings
  *@author Kristof Van Laerhoven
  */

// read a specific attribute's value as a string
int UDPSetParse::read_set(char* attr, char* value) {
	for (int i=0; i<NUM_UDP_ATTR; i++) 
		if (strcasecmp(attr,udpset_attr[i])==0) { 
			strcpy(udpset_val[i],value);
			return 0;
		}
	return -1;
}

// update settings from (attribute,value) table, convert from strings
int UDPSetParse::update_set() {
	udp_set->port = strtol(udpset_val[0],NULL,0);
	udp_set->timeout = strtol(udpset_val[1],NULL,0);
	strcpy(udp_set->targethost, udpset_val[2]);
	if (strcasecmp(udpset_val[3],"ascii")==0)
			udp_set->mode = ASC_MODE;
	else if (strcasecmp(udpset_val[3],"binary")==0)
			udp_set->mode = BIN_MODE;
	else return -1;
	return 0;
}

// write the DTD for the settings to buffer
int UDPSetParse::write_dtd(char* buffer) {
	char tmpstr[256];
	strcpy(buffer,"\t<!ELEMENT udp (channel*)>\n"); 
	for (int i=0; i<NUM_UDP_ATTR; i++) {
		sprintf(tmpstr, "\t\t<!ATTLIST   udp %s\t%s\t\"%s\">\n", 
			udpset_attr[i], udpset_type[i], udpset_defs[i]);
		strcat(buffer,tmpstr); // add to buffer
	}
	return 0;
}

// write the udp settings in XML format to buffer 
int UDPSetParse::write_set(char* buffer) {
	char tmpstr[256];
	strcpy(buffer,"\t<udp "); // start at the beginning..
	for (int i=0; i<NUM_UDP_ATTR; i++) {
		sprintf(tmpstr, "%s=\"%s\" ", udpset_attr[i],
			udpset_val[i]);
		strcat(buffer,tmpstr); // add to buffer
	}
	strcat(buffer,">\n");	
	return 0;
}

