/***************************************************************************
                           setparse.h  -  v.0.1
                           --------------------
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

#ifndef SETPARSE_H
#define SETPARSE_H

#include <string.h>
#include <stdio.h> // FILE*

#define LINEBUFFER_MAX  1024
#define TOK_MAX 256
#define XML_SEPARATOR   " \t"

/**Class for parsing logfile settings 
  *@author Kristof Van Laerhoven
  */


struct SetParse {
	virtual ~SetParse() {};
	virtual int read_set(char* attr, char* value)=0;  // read <attribute> <value> pair
	int read_set(FILE* fp);                 // read from a file stream
	virtual int update_set()=0;                       // flush to logfile
	virtual int write_set(char* buffer)=0;            // write to xsd 
	virtual int write_dtd(char* buffer)=0;            // write to dtd
	virtual int write_tag(char* buffer)=0;            // write the tag
};

#endif

