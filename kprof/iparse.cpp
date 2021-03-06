/***************************************************************************
                          iparse.cpp  -  v1.10
                          -------------------
    begin                : Mon Mar 28 2005
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

#include "iparse.h" 

IParse::IParse() 
{
	err     	= 0;
	errline 	= 0;
	sd      	= NULL;
	sp      	= NULL; 
	sp_size 	= 0;
	rs232set	= NULL;
	udpset  	= NULL;
	logfileset	= NULL;
	simset  	= NULL;
	ichset   	= NULL;
	icolset 	= NULL;
	ichs     	= NULL;
	icols   	= NULL;
	filter  	= NULL;
	binvect 	= NULL;
	kvect   	= NULL;
	dvect   	= NULL;
}

IParse::~IParse()
{
	// delete all elements separately from sp:
	for (unsigned int i=0; i<sp_size; i++) delete sp[i];
	if (sp!=NULL)     delete []sp;
	if (sd!=NULL)     delete sd;
	if (ichs!=NULL)   delete []ichs;
	if (icols!=NULL)  delete []icols;
	if (filter!=NULL) delete []filter;
	if (binvect!=NULL)delete binvect;
	if (kvect!=NULL)  delete kvect;
	if (dvect!=NULL)  delete dvect;
}

// we're parsing, and have arrived at the input section:
void IParse::parse_input(FILE* fp, char* tmpstr, unsigned int line, 
                        bool valid_sub_tag, bool valid_att_tag)
{
	if (strcasecmp(tmpstr,"input")==0) {
		//;
	}
	else if (strcasecmp(tmpstr,"rs232")==0) {
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
		if (ichset==NULL) { // first addition
			ichset = new ChannelSettings; ichpset = ichset;
		}
		else { 
			ChannelSettings *chnset = new ChannelSettings;
			ichpset->next = chnset; ichpset = chnset;
		}
		sp[sp_size] = new ChannelSetParse(ichpset);
		num_ichs++; // count the number of input channels
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
	else if (!valid_sub_tag) {
		err = ERR_INVTAG; errline = line;
	}
	if (valid_att_tag){
		if (sp[sp_size]->read_set(fp)!=0) {
			err = ERR_INVATTR; errline = line;
		}
		if (sp[sp_size]->update_set()!=0) { 
			err = ERR_UPDATE; errline = line;
		}
		sp_size++;
	}
}

int IParse::parse(FILE* fp) 
{
	unsigned int line = 1;
	unsigned int max_tags=1024;
	err=0;
	num_ichs=0;
	num_icols=0;
	sp = new SetParse*[max_tags];
	bool input_section_tag = false; // no section 
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
			for (i=0; i<NUM_SUBTAGS; i++) // sub tag ?
				if (strcasecmp(tmpstr,sub_tags[i])==0) 
					valid_sub_tag=true;
			if (input_section_tag)  // what section are we in again?
			{
				for (i=0; i<NUM_A_ITAGS; i++) 
					if (strcasecmp(tmpstr,input_att_tags[i])==0)
						valid_att_tag=true;
			}
			if (strcasecmp(tmpstr,"input")==0) {
				input_section_tag = true; 
				valid_sub_tag = true;
			}
		} while (!feof(fp)&&(!valid_att_tag)&&(!valid_sub_tag));
		if (feof(fp)) return err;
		if (sp_size<max_tags) {
			if (input_section_tag) 
				parse_input(fp,tmpstr,line,valid_sub_tag,valid_att_tag);
		}
		else { // too many tags: array is overflowing!
			err = ERR_TAGOVERFLOW; errline = line;
		}
	}
	fclose(fp);
	return err; // got error?
}

int IParse::export_dtd(char* buffer) 
{
	char tmpstr[MAX_TAG_LENGTH];
	char tmpbuf[MAX_DTD_LENGTH];
	char last_tag[MAX_TAG_LENGTH];
	last_tag[0]=0;
	buffer[0]=0;
	char *authorstr="The CSTK Team";
	char *titlestr="CommonSense ToolKit";
	unsigned int i;
	
	sprintf(buffer,"<!DOCTYPE CSTK [\n\t");
	sprintf(buffer,"%s<!ENTITY AUTHOR \"%s\">\n\t",buffer,authorstr);
	sprintf(buffer,"%s<!ENTITY SOFTWARE \"%s\">\n",buffer,titlestr);
	sprintf(buffer,"%s\t<!ELEMENT input ((%s",buffer,input_att_tags[3]);
	for (i=4; i<NUM_A_ITAGS; i++)
		sprintf(buffer,"%s|%s",buffer,input_att_tags[i]);
	sprintf(buffer,"%s),inputcolumn*)>\n",buffer);
	for (unsigned int t=0; t<sp_size; t++) {
		sp[t]->write_tag(tmpstr);
		if (strcasecmp(last_tag,tmpstr)!=0) {
			sp[t]->write_dtd(tmpbuf);
			sprintf(buffer,"%s%s",buffer,tmpbuf);
		}
		strcpy(last_tag, tmpstr);
	}
	sprintf(buffer,"%s]>\n",buffer);
	return err;
}

int IParse::export_xsd(char* buffer) 
{
	char tmpstr[MAX_XSD_LENGTH];
	char last_mode[MAX_TAG_LENGTH]; 
	char last_tag[MAX_TAG_LENGTH];
	char curr_tag[MAX_TAG_LENGTH];
	last_mode[0]='\0';
	last_tag[0]='\0';
	buffer[0] = '\0';
	if (sp_size) {
		for (unsigned int t=0; t<sp_size; t++) {
			sp[t]->write_tag(curr_tag);
			sp[t]->write_set(tmpstr);
			// is it a different tag than the previous?
			if ((strcasecmp(last_tag,curr_tag)!=0)
				&&(last_tag[0]!='\0')
				&&(strcasecmp(last_tag,last_mode)!=0)
				&&(last_mode[0]!=0)) {
					sprintf(buffer,"%s\t\t</%s>\n", buffer, last_mode);
					last_mode[0]='\0';
			} 
			// is it a tag that needs closing immediately?
			if ((strcasecmp(curr_tag,"channel")==0)||
			(strcasecmp(curr_tag,"inputcolumn")==0) ||
			(strcasecmp(curr_tag,"plot")==0) ||
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
	}
	return err;
}

int IParse::setup_sensordata_parser() 
{
	// set device parser:
	 if (rs232set)   	sd = new Rs232Parser(*rs232set); 	else 
	 if (udpset)     	sd = new UDPParser(*udpset);     	else
	 if (logfileset) 	sd = new LogFileParser(*logfileset);	else
	 if (simset)     	sd = new SimParser(*simset);     	else
	 err = ERR_NOSET;
	return err;
}

int IParse::setup_inputchannels() 
{
	 unsigned int ch_ctr=0; 
	 ichs  	= new char[num_ichs];
	 ichpset = ichset;
	 while (ichpset != NULL) {
		ichs[ch_ctr] = DC_typecast( ichpset->sign, ichpset->bits, ichpset->format );
		ichpset = ichpset->next;
		ch_ctr++;
	 }
	 if (num_ichs!=ch_ctr) err = ERR_CHSET;
	return err;
}

int IParse::setup_inputcolumns() 
{
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
	 if (num_icols==0) err = ERR_NOICOLS;
	return err; // should return err_code from specific parser
}

int IParse::read_buffer(char* buff) 
{
	int ret = sd->read(buff);
	if (ret>=0) 	buff[ret]='\0'; 
	else        	err=-ret;
	return ret;
}

int IParse::read_icols(void) 
{
	int ret = sd->read(ichs, num_ichs, icols, filter, num_icols);
	if (ret<0)	err=-ret;
	return ret;
}

// read the inputcolumns and convert into a binvector
int IParse::read_binvect(void) 
{
	int ret = read_icols();
	if ( ret == (int)num_icols ) {
		for (int i=0; i<ret; i++) {
			binvect->set_comp( icols[i].get_bool(), i);
		}
	}
	return ret;
}
// read the inputcolumns and convert into a kvector
int IParse::read_kvect(void) 
{
	int ret = read_icols();
	if ( ret == (int)num_icols ) {
		for (int i=0; i<ret; i++) {
			kvect->add_comp(icols[i].convert_u8b());
		}
	}
	return ret;
}
// read the inputcolumns and convert into a dvector
int IParse::read_dvect(void) 
{
	int ret = read_icols();
	if ( ret == (int)num_icols ) {
		for (int i=0; i<ret; i++) {
			dvect->set_comp( icols[i].get(), icols[i].get_type() , i); 
		}
	}
	return ret;
}

void IParse::export_err(char* buffer) 
{
	if (err==0)
		buffer[0] = '\0'; // empty string
	else if (err<NUM_KPERRS) 
		sprintf(buffer, "%s Line %i.\n\r", kperr_strings[err], errline);
	else 
		sprintf(buffer, "Device Error %i\n\r",err);
	err = 0;
}

void IParse::init(FILE* fp) 
{
	parse(fp);
	setup_sensordata_parser();
	setup_inputchannels();
	setup_inputcolumns();

	// setup binvector:
	if (binvect==NULL)
	{
		binvect = new BinVector;
		binvect->create(num_icols);
	}
	// setup kvector:
	if (kvect==NULL)
	{
		kvect = new KVector;
		kvect->createVector(num_icols);
	}
	// setup dvector:
	if (dvect==NULL)
	{
		dvect = new DVector;
		dvect->create(num_icols);
	}
}

