/***************************************************************************
                          logfileparser.h  -  v.0.1
                             -------------------
    begin                : Fri Feb 27 2004
    copyright            : (C) 2004 by Stuart Reeves
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

#ifndef LOGFILEPARSER_H
#define LOGFILEPARSER_H

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include "sensordata/sensordata.h"
#include "sensordata/bufferparser.h"

#ifndef BIN_MODE
#define BIN_MODE 1
#endif
#ifndef ASC_MODE
#define ASC_MODE 2
#endif

#define MAX_SEP 50 // max number of seperators in a sequence

#define FILEERR_CANTOPEN  -10
#define FILEERR_CANTREAD  -11
#define FILEERR_INVTYPE   -12

#define COLUMN_SEPARATOR 	" \t"
#define BUFFER_MAX       	1024

/**Class for parsing various logfiles
  *@authors Stuart Reeves, Kristof Van Laerhoven
  */
  
struct LogFileParserSettings {
  LogFileParserSettings() { 
	filename[0]='\0';
	mode=0;
  }
  char  filename[1024];        
  short mode;
};

class LogFileParser : virtual public SensorData {
  
public:
  LogFileParser(char *filename);
  LogFileParser(LogFileParserSettings file_param);
  ~LogFileParser();  
  int read(char *channel_types, uint numchannels, DataCell *columns,  
           uint *filter, uint numcolumns);
  int read(DataCell *channels, uint numchannels);
  int read(DataCell *channels, uint *numchannels);
  int read(char *line);  
  static void parse_string(DataCell *channels, uint column, char *str);

private:
  void openFile();
  void closeFile();
  
private:
  BufferParser bp;
  LogFileParserSettings file_param;
  FILE *fp;
  int err;
}; 

#endif

