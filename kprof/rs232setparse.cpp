/***************************************************************************
                           rs232parse.cpp  -  v.0.1
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

#include "rs232setparse.h"

/**Class for parsing rs232 settings
  *@author Kristof Van Laerhoven
  */

// read a specific attribute's value as a string
int Rs232SetParse::read_set(char* attr, char* value) {
	for (int i=0; i<NUM_RS232_ATTR; i++) 
		if (strcasecmp(attr,rs232set_attr[i])==0) { 
			strcpy(rs232set_val[i],value);
			return 0;
		}
	return -1;
}
	
// update settings from (attribute,value) table, convert from strings
int Rs232SetParse::update_set() {
	strcpy(rs232_set->device, rs232set_val[0]);
	switch (strtol(rs232set_val[1],NULL,0)) {
		case 300:    rs232_set->baudrate = B300;    break;
		case 1200:   rs232_set->baudrate = B1200;   break;
		case 2400:   rs232_set->baudrate = B2400;   break;
		case 4800:   rs232_set->baudrate = B4800;   break;
		case 9600:   rs232_set->baudrate = B9600;   break;
		case 19200:  rs232_set->baudrate = B19200;  break;
		case 38400:  rs232_set->baudrate = B38400;  break;
		case 57600:  rs232_set->baudrate = B57600;  break;
		case 115200: rs232_set->baudrate = B115200; break;
		case 230400: rs232_set->baudrate = B230400; break;
		default:     rs232_set->baudrate = B115200; break;
	}
	rs232_set->buff_size= strtol(rs232set_val[2],NULL,0);
	rs232_set->databits = strtol(rs232set_val[3],NULL,0);
	rs232_set->stopbits = strtol(rs232set_val[4],NULL,0);
	if (strcasecmp(rs232set_val[5],"no")==0)
			rs232_set->parity = PAR_NO;
	else if (strcasecmp(rs232set_val[5],"odd")==0)
			rs232_set->parity = PAR_ODD;
	else if (strcasecmp(rs232set_val[5],"even")==0)
			rs232_set->parity = PAR_EVEN;
	else if (strcasecmp(rs232set_val[5],"mark")==0)
			rs232_set->parity = PAR_MARK;
	else if (strcasecmp(rs232set_val[5],"space")==0)
			rs232_set->parity = PAR_SPACE;
	else return -1; // error!
	if (strcasecmp(rs232set_val[6],"ascii")==0)
			rs232_set->mode = ASC_MODE;
	else if (strcasecmp(rs232set_val[6],"binary")==0)
			rs232_set->mode = BIN_MODE;
	else return -1;
	strcpy(rs232_set->poll_char, rs232set_val[NUM_RS232_ATTR-1]);
	return 0;
}

// write the DTD for the settings to buffer
int Rs232SetParse::write_dtd(char* buffer) {
	char tmpstr[256];
	strcpy(buffer,"\t<!ELEMENT rs232 (poll?,channel*)>\n"); 
	for (int i=0; i<NUM_RS232_ATTR-1; i++) {
		sprintf(tmpstr, "\t\t<!ATTLIST   rs232 %s\t%s\t\"%s\">\n", 
			rs232set_attr[i], rs232set_type[i], rs232set_defs[i]);
		strcat(buffer,tmpstr); // add to buffer
	}
	strcat(buffer, "\t<!ELEMENT poll (#PCDATA)>\n");
	strcat(buffer,"\t\t<!ATTLIST   poll command\tCDATA\t#IMPLIED>\n");
	strcat(buffer, 
	"\t\t<!ATTLIST   poll mode\t(ascii|binary)\t\"ascii\">\n");
	return 0;
}

// write the rs232 settings in XML format to buffer 
int Rs232SetParse::write_set(char* buffer) {
	char tmpstr[256];
	strcpy(buffer,"\t<rs232 "); // start at the beginning..
	for (int i=0; i<NUM_RS232_ATTR-1; i++) {
		sprintf(tmpstr, "%s=\"%s\" ", rs232set_attr[i],
			rs232set_val[i]);
		strcat(buffer,tmpstr); // add to buffer
	}
	strcat(buffer,">\n");	
	if (rs232set_val[NUM_RS232_ATTR-1][0]!=0) 
	{
		sprintf(tmpstr, "\t\t<poll %s=\"%s\" %s=\"%s\">\n", 
			rs232set_attr[NUM_RS232_ATTR-1], rs232set_val[NUM_RS232_ATTR-1],
			// automatically determine the mode from the first byte:
			"mode",(rs232set_val[NUM_RS232_ATTR-1][0]>31)?"ascii":"binary");
		strcat(buffer,tmpstr);	
	}
	return 0;
}

