/***************************************************************************
                           bufferparser.h  -  v.0.1
                           ------------------------
    begin                : Thu Jan 13 2005
    copyright            : (C) 2005 by Kristof Van Laerhoven
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

#ifndef BUFFER_PARSER_H
#define BUFFER_PARSER_H

#include "cstk_base/types.h" 
#include "cstk_base/vector/datacell.h" 

#ifndef uint
#define uint unsigned int
#endif

#define COLUMN_SEPARATOR 	" \t"

/** class for parsing a buffer containing either ascii data or binary data

  *@authors: Kristof Van Laerhoven
*/

// class just contains two functions that parse a buffer:
struct BufferParser 
{  
   int parse_bin(char *buf, uint n, char* channel_types, uint numchannels, 
                 DataCell* columns,  uint* filter, uint numcolumns);
   int parse_asc(char *buf, char* channel_types, uint numchannels, 
                 DataCell* columns,  uint* filter, uint numcolumns);   
   uint BufferParser::filtrate(uint *filter, uint numcolumns, uint channel);

};

#endif //BUFFER_PARSER_H
