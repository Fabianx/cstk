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

#include "kprof/setparse.h"
#include "kprof/rs232setparse.h"
#include "kprof/udpsetparse.h"
#include "kprof/logfilesetparse.h"
#include "kprof/simsetparse.h"
#include "sensordata/sensordata.h"
#include "sensordata/rs232parser/rs232parser.h"
#include "sensordata/logfileparser/logfileparser.h"
#include "sensordata/udpparser/udpparser.h"
#include "sensordata/simparser/simparser.h"
#include "misc/conio.h"

#define ERR_INVATTR      12
#define ERR_INVTAG       13
#define ERR_INVCH        14
#define ERR_TAGOVERFLOW  15
#define ERR_NOINPUT      16
#define ERR_UPDATE       17

#define NUM_A_ITAGS 5
const char input_att_tags[NUM_A_ITAGS][16] = {
	"rs232", "udp", "logfile", "sim", "poll" };

int main(int ac, char *argv[]) {
	
	if (ac<2) {
		printf("\n  This tool reads a CSTK XSD file and\n");
		printf("  parses its input section. The user\n");
		printf("  then gets the option of annotating\n");
		printf("  the incoming data with a key as it\n");
		printf("  streams in.\n");
		printf("\n   syntax:");
		printf("\n     %s <xsd> [options]", argv[0]);
		printf("\n");
		printf("\n    <xsd> is an XML Settings Document.\n");
		printf("\n    valid options:");
		printf("\n     -i: add iterator in front");
		printf("\n     -t: add timestamp in front");
		printf("\n\n");
		exit(0);
	}
	
	bool iterator = false;
	bool timestamp = false;
	
	if (ac>2) {
		if (strcasecmp(argv[2],"-i")==0) 
			iterator=true;
		if (strcasecmp(argv[2],"-t")==0) 
			timestamp=true;
	}
	
	ConIO con;
	
	//----- These are valid settings elements: -------
	 Rs232ParserSettings   	*rs232set	= NULL;
	 UDPParserSettings     	*udpset  	= NULL;
	 LogFileParserSettings 	*logfileset	= NULL;
	 SimParserSettings     	*simset  	= NULL;
	//------------------------------------------------
	
	// generic attribute parser array:	
	 int max_tags = 1024;
	 SetParse*	setparse[max_tags];
	 for (int j=0; j<max_tags; j++)
	 	setparse[j] = NULL;
	 int      	parser_counter=0;
	 
	 int err=0;
	 unsigned int line=1;
	 unsigned int errorline=0;
	 
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
		bool valid_att_tag = false;
		do {
		   do { ch=getc(fp); if (ch=='\n') line++;
		   } while ((ch!='<')&&(!feof(fp)));
		   t=0;
		   do { ch=getc(fp); tmpstr[t++]=ch; if (ch=='\n') line++;
		   } while ((ch!=' ')&&(ch!='\t')&&
		  	    (ch!='\n')&&(ch!='\r')&&(ch!='>')&&(!feof(fp)));
		   tmpstr[t-1]='\0';
		   int i;
		   for (i=0; i<NUM_A_ITAGS; i++)
			if (strcasecmp(tmpstr,input_att_tags[i])==0)
				valid_att_tag=true;
		} while (!feof(fp)&&(!valid_att_tag));
		
		if (parser_counter<max_tags) {
		  if (strcasecmp(tmpstr,"rs232")==0) {
			rs232set = new Rs232ParserSettings;
			setparse[parser_counter] = new Rs232SetParse(rs232set);
		  }
		  else if (strcasecmp(tmpstr,"udp")==0) {
			udpset = new UDPParserSettings;
			setparse[parser_counter] = new UDPSetParse(udpset);
		  }
		  else if (strcasecmp(tmpstr,"logfile")==0) {
			logfileset = new LogFileParserSettings;
			setparse[parser_counter] = new LogFileSetParse(logfileset);
		  }
		  else if (strcasecmp(tmpstr,"sim")==0) {
			simset = new SimParserSettings;
			setparse[parser_counter] = new SimSetParse(simset);
		  }
		  else if (strcasecmp(tmpstr,"poll")==0) {
			if ((parser_counter-1)>=0) parser_counter--;
		  }
		  if (valid_att_tag){ 
			if (setparse[parser_counter]->read_set(fp)!=0) {
				err = ERR_INVATTR; // wrong attribute
				errorline = line;
			}
			if (setparse[parser_counter]->update_set()!=0) {
				err = ERR_UPDATE; // error during update
				errorline = line;
			}
			parser_counter++;
		  }
		}
		else { // too many tags: array is overflowing!
			err = ERR_TAGOVERFLOW;
			errorline = line;
		}
	}
	fclose(fp);
	
	if (err!=0) {
		printf("Error (number %i) at line %i\n",err,errorline);
		//exit(err);
	}
	
	SensorData *sd=NULL;
		
	// set the input mode:
	tmpstr[0]=0;
	for (int t=0; t<parser_counter; t++) {
		setparse[t]->write_tag(tmpstr);
		if (strcasecmp(tmpstr,"rs232")==0) {
			if (rs232set) 	sd = new Rs232Parser(*rs232set);
		} else if (strcasecmp(tmpstr,"udp")==0) {
			if (udpset) 	sd = new UDPParser(*udpset);
		} else if (strcasecmp(tmpstr,"logfile")==0) {
			if (logfileset)	sd = new LogFileParser(*logfileset);
		} else if (strcasecmp(tmpstr,"sim")==0) {
			if (simset) 	sd = new SimParser(*simset);
		} 
	}

	if (sd!=NULL) {
	 // show what string is being returned
	  tmpstr[0]=0;
	  char ch=0, cl='0';
	  long int t=0;
	  while (ch!='q'&&ch!='Q')
	  {
		int ret=sd->read(tmpstr);
		if (ret>0) 
		{
			tmpstr[ret]='\0';
			if (iterator)
				printf("%6li\t%s\t%c\n\r",t,tmpstr,cl);
			else
				printf("%s\t%c\n\r",tmpstr,cl);
			t++;
		}
		ch = con.kb_getc();
		if (ch!=0) cl = ch;
	  }
	}
	else 
	{
		err = ERR_NOINPUT;
		printf("Error (number %i) at line %i\n",err,errorline);
	}
	
	// delete pointers:
	for(int i=0; i<max_tags;i++) 
		if (setparse[i]!=NULL) delete setparse[i];
		
	return err;
}

