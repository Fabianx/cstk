/***************************************************************************
                             iparse_t.cpp  -  v1.00
                             ---------------------
    begin                : Mon Mar 28 2005
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

#include "kprof/iparse.h"
#include <stdlib.h> // exit

/***************************************************************************
  IParse reads an XML file that describes the parameters for the most common
  ways of reading sensor data (rs232, udp, etc) and launches the appropriate 
  sensor data parsers accordingly.
  
  This example just re-creates the XML file (filling in missing attributes
  with the default values) and gives the DTD for the corresponding tags.
 ***************************************************************************/

int main(int ac, char **argv)
{
	IParse input;
	if (ac<2) {
	printf("\n supply an XML Settings Document please\n\n");
	exit(1);
	} else {
	printf("\nParsing:\n\r");
	}
  
	char buff[0x10000];
	
	FILE* fp = fopen(argv[1],"r");
	if (fp==NULL) {
		printf("Error opening file %s.\n\r", argv[1]);
		return -1;
	}
	
	input.parse(fp); // parse the file
	
	input.export_dtd(buff); // print the DTD
	printf("--- DTD: -------------------------------------\n%s\n",buff);
	
	input.export_xsd(buff); // print the XML (from objects) 
	printf("--- XML: -------------------------------------\n%s\n",buff);
	
	if (input.error()) { // show errors if there are any:
		input.export_err(buff); 
		printf("%s",buff); 
		return(-1);
	}
	else
		return 0;
}

