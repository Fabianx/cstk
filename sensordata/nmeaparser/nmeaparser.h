/***************************************************************************
                          nmeaparser.h  -  v.0.1
                             -------------------
    begin                : Sat Mar 27 2004
    copyright            : (C) 2004 by Stuart Reeves
	email                : str@cs.nott.ac.uk
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef NMEAPARSER_H
#define NMEAPARSER_H

#include <string.h>
#include <stdio.h>
#include "cstk_base/vector/datacell.h"

#ifndef UINT
#define uint unsigned int
#endif

#define BUFFER_MAX 				1024
#define NMEA_INIT				"$"
#define NMEA_PROPRIETARY_INIT 	"P"
#define NMEA_SEPARATOR			","
#define NMEA_TALKER_ID_LEN		2
#define NMEA_SENTENCE_ID_LEN	3
#define NMEA_MAX_LEN			82
#define NMEA_CHECKSUM_CHAR		'*'

#define MFR_HONEYWELL			"TNTHPR"

/** Class for parsing NMEA protocol `sentences'
  *
  * @author Stuart Reeves
  */

class NMEAParser {
  
public:
	NMEAParser(char *filename);
  	NMEAParser();
  	~NMEAParser();
  	
	// Parses the current sentence from the file, and places the values in 
	// channels consecutively
  	int parse_sentence(DataCell *channels, uint numchannels);
	
	/* Parses a given sentence. All NMEA protocol sentences (NMEA-0183 standard
	 * follow the format:
	 * 
	 * $<talker ID><sentence ID>,<field>,<field>,...<optional checksum><CR/LF>
	 *
	 * Maximum sentence length is 82 chars. Talker ID is 2 chars, sentence ID 
	 * is 3 chars. Checksum is an asterisk ("*") followed by two hex digits. The
	 * checksum is the xor of everything inbetween "$" and the "*".
	 *
	 * Proprietary sentences start in a slightly different way:
	 *
	 * $P<manufacturer ID>,... 
	 *
	 * Where the manufacturer ID is 3 digits. */
	int parse_sentence(DataCell *channels, 
					   uint 	 numchannels, 
					   char 	*sentence);

	
private:
  	void openFile();
  	void closeFile();
  
private:
 	char *filename;
 	FILE *fp;
  
}; 

#endif

