/***************************************************************************
                          kprof.cpp  -  v1.00
                          -------------------
    begin                : Fri Nov 14 2003
    copyright            : (C) 2003 by Kristof Van Laerhoven
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

#include "kprof.h"

KProf::KProf() {
	err = 0;
	errline = 0;
	sd      	= NULL;
	sp      	= NULL; sp_size=0;
	rs232set	= NULL;
	udpset  	= NULL;
	logfileset	= NULL;
	simset  	= NULL;
	chset   	= NULL;
	icolset 	= NULL;
	chs     	= NULL;
	icols   	= NULL;
	filter  	= NULL;
}

KProf::~KProf() {
	// delete all elements separately from sp:
	   // TODO
	if (sp!=NULL) delete []sp;
	if (sd!=NULL) delete sd;
	if (chs!=NULL) delete []chs;
	if (icols!=NULL) delete []icols;
	if (filter!=NULL) delete []filter;
}

int KProf::parse(FILE* fp) {
	unsigned int line = 1;
	unsigned int max_tags=1024;
	err=0;
	num_chs=0;
	num_icols=0;
	sp = new SetParse*[max_tags];
	if (!fp) 
		return ERR_NOFILE;
	else while (!feof(fp))
	{	
		int t=0;
		char ch;
		bool valid_att_tag = false, valid_sub_tag = false;
		char tmpstr[MAX_TAG_LENGTH];
		do {
		   do { ch=getc(fp); if (ch=='\n') line++;
		   } while ((ch!='<')&&(!feof(fp))); // wait for a '<'
		   t=0;		   
		   do { ch=getc(fp); tmpstr[t++]=ch; if (ch=='\n') line++;
		   } while ((ch!=' ')&&(ch!='\t')&&
		  	    (ch!='\n')&&(ch!='\r')&&(ch!='>')&&(!feof(fp)));
		   tmpstr[t-1]='\0'; // extract the word following the '<'
		   int i;
		   for (i=0; i<NUM_A_ITAGS; i++)
			if (strcasecmp(tmpstr,input_att_tags[i])==0)
				valid_att_tag=true;
		   for (i=0; i<NUM_S_ITAGS; i++)
			if (strcasecmp(tmpstr,input_sub_tags[i])==0)
				valid_sub_tag=true;
		} while (!feof(fp)&&(!valid_att_tag)&&(!valid_sub_tag));
		
		if (sp_size<max_tags) {
		  if (strcasecmp(tmpstr,"rs232")==0) {
			rs232set = new Rs232ParserSettings;
			sp[sp_size] = new Rs232SetParse(rs232set);
		  }
		  else if (strcasecmp(tmpstr,"udp")==0) {
			udpset = new UDPParserSettings;
			sp[sp_size] = new UDPSetParse(udpset);
		  }
		  else if (strcasecmp(tmpstr,"logfile")==0) {
			logfileset = new LogFileParserSettings;
			sp[sp_size] = new LogFileSetParse(logfileset);
		  }
		  else if (strcasecmp(tmpstr,"sim")==0) {
			simset = new SimParserSettings;
			sp[sp_size] = new SimSetParse(simset);
		  }
		  else if (strcasecmp(tmpstr,"poll")==0) {
			if ((sp_size)>0) sp_size--; // go back to previous tag
		  }
		  else if (strcasecmp(tmpstr,"channel")==0) {
			if (chset==NULL) { // first addition
				chset = new ChannelSettings; chpset = chset;
			}
			else { 
				ChannelSettings *chnset = new ChannelSettings;
				chpset->next = chnset; chpset = chnset;
			}
			sp[sp_size] = new ChannelSetParse(chpset);
			num_chs++; // count the number of channels
		  }
		  else if (strcasecmp(tmpstr,"inputcolumn")==0) {
			if (icolset==NULL) { // first addition
				icolset = new InputColumnSettings; icolpset = icolset;
			}
			else { 
				InputColumnSettings *icolnset = new InputColumnSettings;
				icolpset->next = icolnset; icolpset = icolnset;
			}
			sp[sp_size] = new InputColumnSetParse(icolpset);
			num_icols++; // count the number of inputcolumns
		  }
		  if (valid_att_tag){
			if (sp[sp_size]->read_set(fp)!=0) { // parse all attributes
				err = ERR_INVATTR; errline = line;
			}
			if (sp[sp_size]->update_set()!=0) { // update values in set
				err = ERR_UPDATE; errline = line;
			}
			sp_size++;
		  }
		}
		else { // too many tags: array is overflowing!setparse
			err = ERR_TAGOVERFLOW; errline = line;
		}
	}
	fclose(fp);
	return err; // got error?
}

int KProf::export_dtd(char* buffer) {
	err=0;
	char tmpstr[MAX_TAG_LENGTH];
	char tmpbuf[MAX_DTD_LENGTH];
	char last_tag[MAX_TAG_LENGTH];
	last_tag[0]=0;
	buffer[0]=0;
	for (unsigned int t=0; t<sp_size; t++) {
		sp[t]->write_tag(tmpstr);
		if (strcasecmp(last_tag,tmpstr)!=0) {
			sp[t]->write_dtd(tmpbuf);
			sprintf(buffer,"%s%s",buffer,tmpbuf);
		}
		strcpy(last_tag, tmpstr);
	}
	return err;
}

int KProf::export_xsd(char* buffer) {
	err=0;
	char tmpstr[MAX_XSD_LENGTH];
	char last_mode[MAX_TAG_LENGTH]; 
	char last_tag[MAX_TAG_LENGTH];
	char curr_tag[MAX_TAG_LENGTH];
	last_mode[0]='\0';
	last_tag[0]='\0';
	strcpy(buffer,"\t<input>\n");
	for (unsigned int t=0; t<sp_size; t++) {
		sp[t]->write_set(tmpstr);
		sp[t]->write_tag(curr_tag);
		// is it a different tag than the previous?
		if ((strcasecmp(last_tag,curr_tag)!=0)
			&&(last_tag[0]!='\0')
			&&(strcasecmp(last_tag,last_mode)!=0)) {
				sprintf(buffer,"%s\t\t</%s>\n", buffer, last_mode);
				last_mode[0]='\0';
		} 
		// is it a tag that needs closing immediately?
		if ((strcasecmp(curr_tag,"channel")==0)||
		    (strcasecmp(curr_tag,"inputcolumn")==0) ||
		    (strcasecmp(curr_tag,"poll")==0)) {
			sprintf(buffer,"%s\t%s</%s>\n", buffer, tmpstr,curr_tag);
		}
		else {
			sprintf(buffer,"%s\t%s", buffer, tmpstr);
			strcpy(last_mode, curr_tag);
		} 
		strcpy(last_tag, curr_tag);
	}
	if (last_mode[0]!='\0') sprintf(buffer,"%s\t\t</%s>\n", buffer, last_mode);
	sprintf(buffer,"%s\t</input>\n",buffer);
	return err;
}

int KProf::setup_sensordata_parser() {
	err = 0; // reset errors
	// set device parser:
	 if (rs232set)   	sd = new Rs232Parser(*rs232set); 	else 
	 if (udpset)     	sd = new UDPParser(*udpset);     	else
	 if (logfileset) 	sd = new LogFileParser(*logfileset);	else
	 if (simset)     	sd = new SimParser(*simset);     	else
	 err = ERR_NOSET;
	return err;
}

int KProf::setup_channels() {
	err = 0; // reset errors
	 unsigned int ch_ctr=0; 
	 chs   	= new char[num_chs];
	 chpset = chset;
	 while (chpset != NULL) {
	 	chs[ch_ctr] = DC_typecast( chpset->sign, chpset->bits, chpset->format );
		chpset = chpset->next;
	 	ch_ctr++;
	 }
	 if (num_chs!=ch_ctr) err = ERR_CHSET;
	return err;
}

int KProf::setup_inputcolumns() {
	err = 0; // reset errors
	 unsigned int icol_ctr=0; 
	 icols 	= new DataCell[num_icols];
	 filter	= new unsigned int[num_icols];
	 icolpset = icolset;
	 while (icolpset != NULL) {
	 	filter[icol_ctr] = icolpset->channel;
	 	icols[icol_ctr].set_type(DC_typecast( icolpset->sign, icolpset->bits, 
	 	                                      icolpset->format ));
	 	icols[icol_ctr].set_bits(icolpset->bits);
	 	icolpset = icolpset->next;
	 	icol_ctr++;
	 }
	 if (num_icols!=icol_ctr) err = ERR_ICOLSET;
	return err; // should return err_code from specific parser
}

int KProf::read_buffer(char* buff) {
	int ret = sd->read(buff);
	if (ret>=0) 	buff[ret]='\0'; 
	else        	err=-ret;
	return ret;
}

int KProf::read_icols(void) {
	int ret = sd->read(chs, num_chs, icols, filter, num_icols);
	if (ret<0)	err=-ret;
	return ret;
}

void KProf::export_err(char* buffer) {
	if (err==0)
		buffer[0] = '\0'; // empty string
	else if (err<NUM_KPERRS) 
		sprintf(buffer, "%s. Line %i.\n\r", kperr_strings[err], errline);
	else 
		sprintf(buffer, "Device Error %i\n\r",err);
}

