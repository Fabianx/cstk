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

#include "kprof/setparse.h"
#include "kprof/rs232setparse.h"
#include "kprof/logfilesetparse.h"
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
	LogFileParserSettings logfileset;
	
	SetParse*	setparse[256];
	int		parser_counter=0;
	//Rs232SetParse    setrs232parse(&rs232set);
	//LogFileSetParse  setlogfileparse(&logfileset);
	
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
		} while ( (strcasecmp(tmpstr,"rs232")!=0)&&
		          (strcasecmp(tmpstr,"logfile")!=0)&&
		          (strcasecmp(tmpstr,"poll")!=0)&&(!feof(fp)));
		if ((strcasecmp(tmpstr,"rs232")==0)||(strcasecmp(tmpstr,"poll")==0)) {
			// if it doesn't exist already - stub
			setparse[parser_counter] = new Rs232SetParse(&rs232set);
			if (setparse[parser_counter]->read_set(fp)!=0) 
				printf("error reading parameters!\n");
			parser_counter++;
		}
		else if (strcasecmp(tmpstr,"logfile")==0) {
			setparse[parser_counter] = new LogFileSetParse(&logfileset);
			if (setparse[parser_counter]->read_set(fp)!=0) 
				printf("error reading parameters!\n");
			parser_counter++;
		}
	}
	fclose(fp);
	
	printf("<!-- ***** DTD section ************************* -->\n");
	for (int t=0; t<parser_counter; t++) {
		if (setparse[t]->update_set()!=0)
			printf("error updating vars in section %i.\n",t);
		setparse[t]->write_dtd(tmpstr);
		printf("%s\n", tmpstr);
	}

	printf("<!-- ***** XSD section ************************* -->\n");
	for (int t=0; t<parser_counter; t++) {
		if (setparse[t]->update_set()!=0)
			printf("error updating vars in section %i.\n",t);
		setparse[t]->write_set(tmpstr);
		printf("%s\n", tmpstr);
	}
	
	return 0;
}
