/***************************************************************************
                         setparam_t.cpp  -  v.1.00
                         -------------------------
    begin                : Oct 21 2004
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

#include "kprof/rs232setparse.h"
#include "sensordata/rs232parser/rs232parser.h"
#include "sensordata/logfileparser/logfileparser.h"
#include "sensordata/udpparser/udpparser.h"
#include "sensordata/simparser/simparser.h"

int main(int ac, char *argv[]) {

	if (ac<2) {
		printf("\n  This demo shows how settings can be read and\n");
		printf("  generated with the setparse classes.\n");
		printf("\n   syntax:");
		printf("\n     %s <xsd>", argv[0]);
		printf("\n");
		printf("\n    <xsd> is an XML Settings Document.\n");   
		printf("\n\n");
		exit(0);
	}

	Rs232ParserSettings rs232set;

	Rs232SetParse setparse(&rs232set);
	/*
	this is how you can easily set parameters:
	setparse.read_set("baudrate","38400");
	setparse.read_set("port","/dev/tty.usbserial0");
	setparse.read_set("databits","8");
	setparse.read_set("stopbits","1");
	setparse.read_set("command","G");
	*/
	
    FILE* fp = fopen(argv[1],"r");
	char tmpstr[1024];
	if (!fp) {
		printf("error opening file!\n");
		exit(0);
	}
	else while (!feof(fp))
	{
		int t=0;
		char ch;
		do {
		   do { ch=getc(fp); } while ((ch!='<')&&(!feof(fp)));
		   t=0;
		   do { ch=getc(fp); tmpstr[t++]=ch; 
		   } while ((ch!=' ')&&(ch!='\t')&&
			  	    (ch!='\n')&&(ch!='\r')&&(ch!='>')&&(!feof(fp)));
		   tmpstr[t-1]='\0';
		} while ((strcasecmp(tmpstr,"rs232")!=0)&&
				 (strcasecmp(tmpstr,"poll")!=0)&&(!feof(fp)));
		if (setparse.read_set(fp)!=0) 
			printf("error reading rs232 parameters!\n");
	}
	fclose(fp);

	if (setparse.update_set()!=0)
		printf("error updating rs232 vars\n");
	
	setparse.write_set(tmpstr);
	printf("---- XSD: --------------------------------\n%s\n", tmpstr);
  
	setparse.write_dtd(tmpstr);
	printf("---- DTD: --------------------------------\n%s\n", tmpstr);

	return 0;
}

