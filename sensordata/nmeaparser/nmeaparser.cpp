/***************************************************************************
                         nmeaparser.cpp  -  v.0.1
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

#include "nmeaparser.h"
#include "sensordata/logfileparser/logfileparser.h"

/** Class for parsing NMEA protocol sentences
  * @author Stuart Reeves */

NMEAParser::NMEAParser(char *filename) {
	this->filename = filename;
	fp = NULL;
}


NMEAParser::NMEAParser() {
	this->filename = NULL;
	fp = NULL;
}

NMEAParser::~NMEAParser() {
	closeFile();
	delete[] this->filename;
}


int NMEAParser::parse_sentence(DataCell *channels, uint numchannels) {

	openFile();
	if (this->filename != NULL && fp != NULL) {
		char buf[BUFFER_MAX];

		if (fgets(buf, sizeof(buf), fp) == NULL)
			return 0;
		else
			return parse_sentence(channels, numchannels, buf);
		
	} else 
		return 0;
}


int NMEAParser::parse_sentence(DataCell *channels, 
							   uint 	 numchannels, 
							   char 	*sentence) {
	
	// TODO: handle blank lines, sort this out properly
	if (sentence[0] == '\n')
		return -1;
	
	// Store the sentence for potential checksumming later
	char currSentence[NMEA_MAX_LEN];
	strcpy(currSentence, sentence);
	
	char *token = strtok(currSentence, NMEA_SEPARATOR);
	
	// Test the first token for protocol compatibility
	if (strncmp(token, NMEA_INIT, strlen(NMEA_INIT)) == 0) {
		// Check for proprietary flag (token+1 removes the NMEA_INIT char)
		if (strncmp(token+1, 
					NMEA_PROPRIETARY_INIT, 
					strlen(NMEA_PROPRIETARY_INIT)) == 0) {
			
			if (strlen(token+1) > 2) {
				char *mancode = token+2;
				// Set the two channels as the proprietary init and code
				if (sizeof(channels) >= 2) {
					channels[0].set_type(STRING_TYPE);
					channels[0].set_stringval(NMEA_PROPRIETARY_INIT);
					channels[1].set_type(STRING_TYPE);
					channels[1].set_stringval(mancode);
				} else
					return 0;
			
			} else {
				printf("bad manufacturer code\n");
				return 0;
			}

					
		} else {
			
			if (strlen(token+1) >= NMEA_TALKER_ID_LEN + NMEA_SENTENCE_ID_LEN) {
				
				char talkerID[NMEA_TALKER_ID_LEN+1];
				strcpy(talkerID, token+1);
				talkerID[NMEA_TALKER_ID_LEN] = '\0';
				
				char *sentenceID = token + 1 + NMEA_TALKER_ID_LEN;

				// Set the two channels as the talker and sentence IDs
				if (sizeof(channels) >= 2) {
					channels[0].set_type(STRING_TYPE);
					channels[0].set_stringval(talkerID);
					channels[1].set_type(STRING_TYPE);
					channels[1].set_stringval(sentenceID);
				} else
					return 0;
				
			} else {
				printf("bad sentence id\n");
				return 0;
			}
		}
		
		
		char checksumChars[3];
		uint sLen			    = strlen(sentence);
		long calcChecksum 		= 0;
		// This flag indicates whether we are testing the checksum or not
		bool parseChecksumChars = false;
		
		for (uint j = 1; j < sLen; ++j) {
			if (parseChecksumChars && j < sLen-1) {
				checksumChars[0] = sentence[j];
				checksumChars[1] = sentence[j+1];
				break;
			} else {
				if (sentence[j] == NMEA_CHECKSUM_CHAR)
					parseChecksumChars = true;
				else
					calcChecksum ^= sentence[j];
			}
		}
		
		if (parseChecksumChars) {
			
			// Work out if the calculated checksum matches the read value
			long readChecksum = 0;
			sscanf(checksumChars, "%lx", &readChecksum);
			if (calcChecksum != readChecksum) {
				printf("Bad checksum\n");
				return 0;
			}
		
		} else
			printf("NO checksum\n");

		uint i = 2; // Due to talker and sentence IDs

		while ((token = strtok(NULL, NMEA_SEPARATOR)) && 
			   token[0] != NMEA_CHECKSUM_CHAR && 
			   i < numchannels) {

			LogFileParser::parse_string(channels, i, token);
			++i;
		}

		return 1;
				
	} else
		printf("bad sentence protocol\n");

	return 0;
}


void NMEAParser::closeFile() {
	if (fp != NULL)
		fclose(fp);
}


void NMEAParser::openFile() {
	if (fp == NULL)
		fp = fopen(this->filename, "r");
}



