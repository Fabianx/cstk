/***************************************************************************
                             kprof_t.cpp  -  v1.00
                             ---------------------
    begin                : Tue Mar 09 2004
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

#include "kprof/kprof.h"
#include <stdlib.h> // exit

int main(int ac, char **argv) {

	KProf profile;
	if (ac<2) {
	printf("\n supply an XML Settings Document please\n\n");
	exit(1);
	} else {
	printf("\nParsing:\n\r");
	}
  
	char buff[0x10000];
	KProf kp;
	
	FILE* fp = fopen(argv[1],"r");
	if (fp==NULL) {
		printf("Error opening file %s.\n\r", argv[1]);
		return -1;
	}
	
	kp.parse(fp);
	
	kp.export_dtd(buff);
	printf("--- DTD: -------------------------------------\n%s\n",buff);
	
	kp.export_xsd(buff);
	printf("--- XSD: -------------------------------------\n%s\n",buff);
	
	if (kp.error()) { 
		kp.export_err(buff); 
		printf("%s",buff); 
		return(-1);
	}
	else
		return 0;
}

