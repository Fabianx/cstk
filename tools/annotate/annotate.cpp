/***************************************************************************
                         annotate.cpp  -  v.1.00
                         ------------------------
    begin                : Dec 14 2004
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

#include "cstk_base/vector/binvector.h"
#include "cstk_base/vector/kvector.h"
#include "cstk_base/vector/dvector.h"
#include "kprof/iparse.h"
#include "misc/conio.h"
#include <sys/time.h>     // for timings

#define BINVECTOR_MODE 1
#define KVECTOR_MODE   2
#define DVECTOR_MODE   3

int main(int ac, char **args) {
	
	if (ac<2) 
	{
		printf("\n  This tool reads a CSTK XML file and\n");
		printf("  parses its input section. The user\n");
		printf("  then gets the option of annotating\n");
		printf("  the incoming data with a key as it\n");
		printf("  streams in.\n");
		printf("\n   syntax:");
		printf("\n     %s <xsd> [options]", args[0]);
		printf("\n");
		printf("\n    <xsd> is a CSTK XML file.\n");
		printf("\n    valid options:");
		printf("\n     -i: add iterator in front");
		printf("\n     -t: add timestamp in front");
		printf("\n     -b: output binvectors (bits)");
		printf("\n     -k: output kvectors (0-255)");
		printf("\n     -d: output dvectors (mixed types)");
		printf("\n    if neither b,k,or d are specified, the");
		printf("\n    raw buffers will be printed.");
		printf("\n\n More info can be found in the CSTK manual");
		printf("\n\n (http://cstk.sf.net/). No really, read it.");
		printf("\n\n");
		exit(0);
	}
  
	char buff[0x10000]; // for error msgs
	IParse input;       // reads xml file and plots
	ConIO con;          // keyboard io
	
	FILE* fp = fopen(args[1],"r");
	if (!fp) {printf("Error opening file %s.\n", args[1]); return -1;}
	
	bool iterator = false;
	bool timestamp = false;
	char mode = 0;
	
	for (int i=2; i<ac; i++) {
		if (ac>i) {
			if (strcasecmp(args[i],"-i")==0) 
				iterator=true;
			else if (strcasecmp(args[i],"-t")==0) 
				timestamp=true;
			else if (strcasecmp(args[i],"-b")==0) 
				mode=BINVECTOR_MODE;
			else if (strcasecmp(args[i],"-k")==0) 
				mode=KVECTOR_MODE;
			else if (strcasecmp(args[i],"-d")==0) 
				mode=DVECTOR_MODE;
		}
	}
	
	input.init(fp); // parse file and setup inputcolumns
	
	if (input.error()) 
		{ input.export_err(buff); printf("%s\n",buff); return -1;}

	long unsigned int t=0;
	char ch=0, cl='0'; // input character, class id
	struct timeval tv;
	struct timezone tzv;
	bool quit = false;
	int ret;

	while (!quit) 
	{	
		switch (mode) { 
			case BINVECTOR_MODE: ret = input.read_binvect();
				break;
			case KVECTOR_MODE: ret = input.read_kvect();
				break;
			case DVECTOR_MODE: ret = input.read_dvect();
				break;
			default: 	ret = input.read_buffer(buff);
				break;
		}
		
		if ( ret <= 0 ) 
		{
			printf("Error(%i)\n",ret);
		}
		else
		{
			if (iterator) printf("%6lu\t",t);
			if (timestamp) {
				gettimeofday(&tv, &tzv);
				printf("%li\t%li\t", (long int)tv.tv_sec, 
					(long int)tv.tv_usec);
			}
			switch (mode) { 
				case BINVECTOR_MODE: 
					printf("%s\t", input.binvect->to_string());
					break;
				case KVECTOR_MODE: 
					printf("%s\t", input.kvect->to_string("%i "));
					break;
				case DVECTOR_MODE: 
					printf("%s\t", input.dvect->to_string());
					break;
				default: 
					printf("%6s\t", buff);
					break;
			}
			printf("%c\n",cl);
		}
		
		ch = con.kb_getc();
		if (ch!=0) cl = ch;
		if ( (ch=='q') || (ch=='Q') ) quit = true;
		t++; // increment integer timestamp
	}
	
	return 0; // no error
}

