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
#include "kprof/udpsetparse.h"
#include "kprof/logfilesetparse.h"
#include "kprof/simsetparse.h"
#include "kprof/channelsetparse.h"
#include "kprof/inputcolumnsetparse.h"
#include "sensordata/sensordata.h"
#include "sensordata/rs232parser/rs232parser.h"
#include "sensordata/logfileparser/logfileparser.h"
#include "sensordata/udpparser/udpparser.h"
#include "sensordata/simparser/simparser.h"

#define ERR_INVATTR      12
#define ERR_INVTAG       13
#define ERR_INVCH        14
#define ERR_TAGOVERFLOW  15

#define NUM_A_ITAGS 6
const char input_att_tags[NUM_A_ITAGS][16] = {
	"rs232", "udp", "logfile", "sim", 
	"channel", "inputcolumn"
	};
#define NUM_S_ITAGS 3
const char input_sub_tags[NUM_S_ITAGS][16] = {
	"poll", "!--", "packet" };

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
	
	//----- These are valid settings elements: -------
	 Rs232ParserSettings   	*rs232set	= NULL;
	 UDPParserSettings     	*udpset  	= NULL;
	 LogFileParserSettings 	*logfileset	= NULL;
	 SimParserSettings     	*simset  	= NULL;
	 ChannelSettings     	*chset=NULL; 	// linked list  
	 InputColumnSettings 	*icolset=NULL;	// linked list 
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
	 
	/*
	//this is how you can easily set parameters:
	Rs232SetParse setparse(&rs232set);
	setparse.read_set("baudrate","38400");
	setparse.read_set("port","/dev/tty.usbserial0");
	setparse.read_set("databits","8");
	setparse.read_set("stopbits","1");
	setparse.read_set("command","G");
	Rs232Parser rs232(rs232set);
	char charbuffer[1024];
	while(1) {
		rs232.read(charbuffer);
		printf("%s\n",charbuffer);
	}
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
		bool valid_att_tag = false;
		bool valid_sub_tag = false;
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
		   for (i=0; i<NUM_S_ITAGS; i++)
			if (strcasecmp(tmpstr,input_sub_tags[i])==0)
				valid_sub_tag=true;
		} while (!feof(fp)&&(!valid_att_tag)&&(!valid_sub_tag));
		
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
			if ((parser_counter-1)>0) 
			if (setparse[parser_counter-1]->read_set(fp)!=0) 
				printf("error reading parameters!\n");
		  }
		  else if (strcasecmp(tmpstr,"channel")==0) {
			// i_chset = add_channel();
			// setparse[parser_counter] = new ChannelSetParse(i_chset);
			chset = new ChannelSettings;
			setparse[parser_counter] = new ChannelSetParse(chset);
		  }
		  else if (strcasecmp(tmpstr,"inputcolumn")==0) { 
			// i_icolset = add_inputcolumn();
			// setparse[parser_counter] = new InputColumnSetParse(i_icolset);
			icolset = new InputColumnSettings;
			setparse[parser_counter] = new InputColumnSetParse(icolset);
		  }
		  else if (!feof(fp)&&(!valid_sub_tag)) {
			err = ERR_INVTAG; // wrong tag
			errorline = line;
		  }
		  if (valid_att_tag){
			if (setparse[parser_counter]->read_set(fp)!=0) {
				err = ERR_INVATTR; // wrong attribute
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
		exit(err);
	}
	else 
	{
	// print the parameters in XSD and their DTD:
	  printf("<!-- ***** DTD section ************************* -->\n");
	  for (int t=0; t<parser_counter; t++) {
		if (setparse[t]->update_set()!=0)
			printf("error updating vars in section %i.\n",t);
		setparse[t]->write_dtd(tmpstr);
		printf("%s", tmpstr);
	  }
	  printf("<!-- ***** XSD section ************************* -->\n");
	  char last_mode[256]; 
	  char last_tag[256];
	  char curr_tag[256];
	  last_mode[0]='\0';
	  last_tag[0]='\0';
	  printf("\t<input>\n");
	  for (int t=0; t<parser_counter; t++) {
		setparse[t]->write_set(tmpstr);
		setparse[t]->write_tag(curr_tag);
		// is it a different tag than the previous?
		if ((strcasecmp(last_tag,curr_tag)!=0)
			&&(last_tag[0]!='\0')
			&&(strcasecmp(last_tag,last_mode)!=0)) {
				printf("\t\t</%s>\n", last_mode);
				last_mode[0]='\0';
		} 
		// is it a tag that needs closing immediately?
		if ((strcasecmp(curr_tag,"channel")==0)||
		    (strcasecmp(curr_tag,"inputcolumn")==0) ||
		    (strcasecmp(curr_tag,"poll")==0)) {
			printf("\t%s</%s>\n", tmpstr,curr_tag);
		}
		else {
			printf("\t%s", tmpstr);
			strcpy (last_mode, curr_tag);
		} 
		strcpy (last_tag, curr_tag);
	  }
	  if (last_mode[0]!='\0') printf("\t\t</%s>\n", last_mode);
	  printf("\t</input>\n");
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
	
	// setup the dvector:
	// TODO
	
	// show what string is being returned
	tmpstr[0]=0;
	for (int t=0; t<100; t++) {
		int ret=sd->read(tmpstr);
		tmpstr[ret]='\0';
		printf("*%s*\n",tmpstr);
	}
	
	// delete pointers:
	for(int i=0; i<max_tags;i++) 
		if (setparse[i]!=NULL) delete setparse[i];
	
	return err;
}

