/***************************************************************************
                           rs232setparse.h  -  v.0.1
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

#ifndef RS232SETPARSE_H
#define RS232SETPARSE_H

#include <string.h>
#include <stdlib.h>  // atoi
#include "sensordata/rs232parser/rs232parser.h" // for the settings

#define LINEBUFFER_MAX  1024
#define TOK_MAX 256
#define XML_SEPARATOR   " \t"

#define _SET_RS323SET_DEF(a) strcpy(rs232set_val[(a)],rs232set_defs[(a)])

#define NUM_RS232_ATTR 8
const char rs232set_attr[NUM_RS232_ATTR][16] = { 
	"port", "baudrate", "buffsize",	"databits",
	"stopbits",	"parity",	"mode",
	"command"  //  leave this item last (see code)
};
const char rs232set_type[NUM_RS232_ATTR][256] = { 
	"CDATA",	"CDATA",	"CDATA",	"(5|6|7|8)",
	"(1|2)",	"(no|odd|even|mark|space)",	"(ascii|binary)",
	"CDATA"  //  leave this item last (see code)
};
const char rs232set_defs[NUM_RS232_ATTR][16] = { 
	"/dev/ttyS0",	"115200",	"1024",	"8",
	"1",	"no",	"binary",
	"G"  //  leave this item last (see code)
};

/**Class for parsing rs232 settings 
  *@author Kristof Van Laerhoven
  */

class Rs232SetParse {
 public:
	Rs232SetParse(Rs232ParserSettings* rs232_set)
	{
		this->rs232_set = rs232_set;
		if (!rs232_set->device[0]) _SET_RS323SET_DEF(0);
		if (!rs232_set->baudrate)  _SET_RS323SET_DEF(1);
		if (!rs232_set->buff_size) _SET_RS323SET_DEF(2); 
		if (!rs232_set->databits)  _SET_RS323SET_DEF(3);	
		if (!rs232_set->stopbits)  _SET_RS323SET_DEF(4);	
		if (!rs232_set->parity)    _SET_RS323SET_DEF(5);   
		if (!rs232_set->mode)      _SET_RS323SET_DEF(6);
		if (!rs232_set->poll_char[0]) _SET_RS323SET_DEF(NUM_RS232_ATTR-1);
	};
	~Rs232SetParse() {};
	int read_set(char* attr, char* value);  // read <attribute> <value> pair
	int read_set(FILE* fp);                 // read from a file stream
	int update_set();                       // flush to rs232
 	int write_set(char* buffer);            // write to xsd 
 	int write_dtd(char* buffer);            // write to dtd
 private:
	Rs232ParserSettings* rs232_set;
	char rs232set_val[NUM_RS232_ATTR][256];
};

#endif

