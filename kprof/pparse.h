/***************************************************************************
                              pparse.h  -  v1.00
                             -------------------
    begin                : Fri Nov 14 2003
    copyright            : (C) 2003 by Kristof Van Laerhoven
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
#ifndef PPARSE_H
#define PPARSE_H

#include "kprof/tags.h"
#include <stdio.h>    // FILE
#include <strings.h>  // strcasecmp

/***************************************************************************
   PParse parses CSTK settings (formatted as an XML file) for generic 
   algorithm settings, located between two <params> tags. It returns the
   parsed values as integers, floating point numbers, or strings.
 ***************************************************************************/	
class PParse 
{
 public:
	PParse();
	~PParse();

	int parse(FILE* fp);

	int export_dtd(char* buffer);
	int export_xsd(char* buffer);

	void init(FILE* fp);

	void export_err(char* buffer); // print error string in buffer
	bool error() {return err;}

 private:
	void parse_params(FILE* fp, char* tmpstr, unsigned int line, 
	                 bool valid_sub_tag, bool valid_att_tag);

	int err;
	unsigned int errline;
};

#endif // PPARSE_H

