/***************************************************************************
                           udpsetparse.h  -  v.0.1
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

#ifndef UDPSETPARSE_H
#define UDPSETPARSE_H

#include "setparse.h"
#include <stdlib.h>  // atoi
#include "sensordata/udpparser/udpparser.h" // for the settings

#define _SET_UDPSET_DEF(a) strcpy(udpset_val[(a)],udpset_defs[(a)])

#define NUM_UDP_ATTR 4
const char udpset_attr[NUM_UDP_ATTR][16] = { 
	"port", "timeout", "targethost", "mode"};
const char udpset_type[NUM_UDP_ATTR][16] = { 
	"CDATA", "CDATA", "CDATA", "(ascii|binary)"};
const char udpset_defs[NUM_UDP_ATTR][16] = { 
	"2221", "200", "localhost", "ascii"};

/**Class for parsing udp settings 
  *@author Kristof Van Laerhoven
  */

class UDPSetParse: public SetParse {
 public:
	UDPSetParse(UDPParserSettings* udp_set)
	{
		this->udp_set = udp_set;
		if (!udp_set->port)     	_SET_UDPSET_DEF(0);
		if (!udp_set->timeout)  	_SET_UDPSET_DEF(1);
		if (!udp_set->targethost[0])	_SET_UDPSET_DEF(2);
		if (!udp_set->mode)     	_SET_UDPSET_DEF(3);
	};
	~UDPSetParse() {};
	int read_set(char* attr, char* value);  // read <attribute> <value> pair
	int update_set();                       // flush to udp
 	int write_set(char* buffer);            // write to xsd 
 	int write_dtd(char* buffer);            // write to dtd
	int write_tag(char* buffer) {strcpy(buffer,"udp");return 0;}
 private:
	UDPParserSettings* udp_set;
	char udpset_val[NUM_UDP_ATTR][1024];
};

#endif

