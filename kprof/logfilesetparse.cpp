/***************************************************************************
                           logfileparse.cpp  -  v.0.1
                           --------------------------
    begin                : Tue Nov 30 2004
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
	
#include "logfilesetparse.h"
	
/**Class for parsing logfile settings 
  *@author Kristof Van Laerhoven
  */
	
// read a specific attribute's value as a string
int LogFileSetParse::read_set(char* attr, char* value) {
	for (int i=0; i<NUM_LOGFILE_ATTR; i++) 
		if (strcasecmp(attr,logfileset_attr[i])==0) { 
			strcpy(logfileset_val[i],value);
			return 0;
		}
	return -1;
}
	
// update settings from (attribute,value) table, convert from strings
int LogFileSetParse::update_set() {
	strcpy(logfile_set->filename, logfileset_val[0]);
	if (strcasecmp(logfileset_val[1],"ascii")==0)
			logfile_set->mode = ASC_MODE;
	else if (strcasecmp(logfileset_val[1],"binary")==0)
			logfile_set->mode = BIN_MODE;
	else return -1;
	return 0;
}
	
// write the DTD for the settings to buffer
int LogFileSetParse::write_dtd(char* buffer) {
	char tmpstr[256];
	strcpy(buffer,"\t<!ELEMENT logfile (channel*)>\n"); 
	for (int i=0; i<NUM_LOGFILE_ATTR; i++) {
		sprintf(tmpstr, "\t\t<!ATTLIST   logfile %s\t%s\t\"%s\">\n", 
		        logfileset_attr[i], logfileset_type[i], logfileset_defs[i]);
		strcat(buffer,tmpstr); // add to buffer
	}
	return 0;
}
	
// write the logfile settings in XML format to buffer 
int LogFileSetParse::write_set(char* buffer) {
	char tmpstr[256];
	strcpy(buffer,"\t<logfile "); // start at the beginning..
	for (int i=0; i<NUM_LOGFILE_ATTR; i++) {
		sprintf(tmpstr, "%s=\"%s\" ", logfileset_attr[i],
			logfileset_val[i]);
		strcat(buffer,tmpstr); // add to buffer
	}
	strcat(buffer,">\n");
	return 0;
}

