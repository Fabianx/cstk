/***************************************************************************
                           logfilesetparse.h  -  v.0.1
                           ---------------------------
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

#ifndef LOGFILESETPARSE_H
#define LOGFILESETPARSE_H

#include "setparse.h"
#include "sensordata/logfileparser/logfileparser.h" // for the settings

#define _SET_LOGFILESET_DEF(a) strcpy(logfileset_val[(a)],logfileset_defs[(a)])

#define NUM_LOGFILE_ATTR 2
const char logfileset_attr[NUM_LOGFILE_ATTR][16] = {"filename", "mode"};
const char logfileset_type[NUM_LOGFILE_ATTR][16] = {"CDATA", "(ascii|binary)"};
const char logfileset_defs[NUM_LOGFILE_ATTR][16] = {"data.txt", "ascii"};

/**Class for parsing logfile settings 
  *@author Kristof Van Laerhoven
  */

class LogFileSetParse : public SetParse {
 public:
	LogFileSetParse(LogFileParserSettings* logfile_set)
	{
		this->logfile_set = logfile_set;
		if (logfile_set->filename!=NULL)	_SET_LOGFILESET_DEF(0);
		if (!logfile_set->mode)	_SET_LOGFILESET_DEF(1);
	};
	~LogFileSetParse() {};
	int read_set(char* attr, char* value);  // read <attribute> <value> pair
	int update_set();                       // flush to logfile
	int write_set(char* buffer);            // write to xsd 
	int write_dtd(char* buffer);            // write to dtd
	int write_tag(char* buffer) {strcpy(buffer,"logfile");return 0;}

 private:
	LogFileParserSettings* logfile_set;
	char logfileset_val[NUM_LOGFILE_ATTR][1024];
};

#endif

