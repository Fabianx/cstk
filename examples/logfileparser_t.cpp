/***************************************************************************
                         logfileparser_t.cpp  -  v.1.00
                             -------------------
    begin                : Tues Apr 27 2004
    copyright            : (C) 2004 by Stuart Reeves
    email                :  str@cs.nott.ac.uk
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "sensordata/logfileparser/logfileparser.h"
#include "cstk_base/vector/datacell.h"

int main (int argc, char *argv[]) {

	if (argc < 3) {
		printf("Usage: logfileparser_demo <logfile> <numchannels>\n");
		return 0;
	}
	
	uint ch = 0;
	sscanf(argv[2], "%d", &ch);
	
	DataCell data[ch];
	LogFileParser *lfp;
	lfp = new LogFileParser(argv[1]);
	while (lfp->read(data, ch)) {
		for (uint i = 0; i < ch; ++i) {
			printf("cell[%i] = ", i);
			switch (data[i].get_type()) {
				case STRING_TYPE: printf(data[i].get_string()); break;
				case CHAR_TYPE: printf("%c ",data[i].get_char()); break;
				default: printf("%f", data[i].get_f64b()); break;
			}
			printf("\n");
		}
	}
	
	delete lfp;

	return 0;
}

