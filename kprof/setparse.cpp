/***************************************************************************
                           setparse.cpp  -  v.0.1
                           ----------------------
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

#include "setparse.h"

/**Class for parsing settings 
  *@author Kristof Van Laerhoven
  */

// read all attributes from file and store (attribute,value) pairs
int SetParse::read_set(FILE* fp) {
	// read till '>'
	char c=0;
	char att[TOK_MAX];
	char val[TOK_MAX];
	unsigned int token_counter = 0;
	int err=0;
	while (!feof(fp)&&(c!='>')) {
		c = fgetc(fp);
		if ((c==' ')||(c=='\t')||(c=='\n')||(c=='\r')||(c=='=')) { 
			if (token_counter>0)
				att[token_counter]='\0'; // stop attribute
		} 
		else {
			if (c=='"') {  // get value:
				token_counter=0;
				do {
					c = getc(fp);
					val[token_counter++] = c;
					if ((c=='>')||(feof(fp))||(token_counter>TOK_MAX-1)) 
						return -1; // error!!!
				} while (c!='"');
				val[token_counter-1] = '\0';
				// read in the attribute-value pair
				err = read_set(att,val);
				if (err!=0) return err;
				token_counter=0;
			} else
				att[token_counter++] = c;
		}
	}
	return 0;
}

