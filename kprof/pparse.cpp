/***************************************************************************
                          pparse.cpp  -  v1.00
                          --------------------
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

#include "pparse.h" 

PParse::PParse() 
{
	err = 0;	errline = 0;
}

PParse::~PParse()
{
}

void PParse::parse_params(FILE* fp, char* tmpstr, unsigned int line, 
                        bool valid_sub_tag, bool valid_att_tag)
{
	if (strcasecmp(tmpstr,"params")==0) {
	}
	else if (!valid_sub_tag) {
		err = ERR_INVTAG; errline = line;
	}
	if (valid_att_tag || (strcasecmp(tmpstr,"params")==0)){
		
		
	}
}

int PParse::parse(FILE* fp) 
{
	unsigned int line = 1;
	//unsigned int max_tags=1024;
	err=0;
	bool valid_section_tag = false;
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
			if (valid_section_tag) // we're in <window> 
			{
				for (i=0; i<NUM_A_WTAGS; i++) // window tag? 
					if (strcasecmp(tmpstr,window_att_tags[i])==0) 
						valid_att_tag=true;
			}
			if (strcasecmp(tmpstr,"params")==0) {
				valid_section_tag=true; 
				valid_sub_tag=true;
			}
		} while (!feof(fp)&&(!valid_att_tag)&&(!valid_sub_tag));
		if (feof(fp)) return err;
		if (valid_section_tag) 
				parse_params(fp,tmpstr,line,valid_sub_tag,valid_att_tag);
		else { // too many tags: array is overflowing!setparse
			err = ERR_TAGOVERFLOW; errline = line;
		}
	}
	fclose(fp);
	return err; // got error?
}

int PParse::export_dtd(char* buffer) 
{
	return err;
}

int PParse::export_xsd(char* buffer) 
{
	return err;
}

void PParse::export_err(char* buffer) 
{
	if (err==0)
		buffer[0] = '\0'; // empty string
	else if (err<NUM_KPERRS) 
		sprintf(buffer, "%s Line %i.\n\r", kperr_strings[err], errline);
	else 
		sprintf(buffer, "Device Error %i\n\r",err);
	err = 0;
}

void PParse::init(FILE* fp) 
{
	parse(fp);
}

