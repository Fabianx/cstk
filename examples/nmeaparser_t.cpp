/***************************************************************************
                         nmeaparser_t.cpp  -  v.1.00
                             -------------------
    begin                : Sun May 2 2004
    copyright            : (C) 2004 by Stuart Reeves
 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "sensordata/nmeaparser/nmeaparser.h"
#include "sensordata/logfileparser/logfileparser.h"

int main (int argc, char *argv[]) {

	if (argc < 3) {
		printf("Usage: nmeaparser_demo <logfile> <numchannels>\n");
		return 0;
	}
	
	uint ch = 0;
	sscanf(argv[2], "%d", &ch);
	
	DataCell data[ch];

	LogFileParser *lfp = new LogFileParser(argv[1]);
	// We can specify the a file in the NMEAParser constructor, but
	// this example intends to demonstrate how NMEAParser can be used 
	// purely as a parser
	NMEAParser	  *np  = new NMEAParser();

	char line[1024];
	int l = 0;	
	while (lfp->read(line)) {
		printf("string=%s,line=%i\n",line,l);
		if (np->parse_sentence(data, ch, line)) {
			for (uint i = 0; i < ch; ++i) {
				printf("cell[%i] = ", i);
				switch (data[i].get_type()) {
					case STRING_TYPE: printf(data[i].get_string()); break;
					case CHAR_TYPE: printf("%c", data[i].get_char()); break;
					default: printf("%f", data[i].get_f64b()); break;
					
				}
				printf("\n");
			}
		}
		++l;
	}

	delete np;
	delete lfp;
	
	return 0;
}

