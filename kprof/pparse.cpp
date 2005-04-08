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
	sp  = NULL;     sp_size=0;
	paramset = NULL;
}

PParse::~PParse()
{
	// delete all elements separately from sp:
	for (unsigned int i=0; i<sp_size; i++) delete sp[i];
	if (sp!=NULL) delete []sp;
}

void PParse::parse_section(FILE* fp, char* tmpstr, unsigned int line, 
                           bool valid_sub_tag, bool valid_att_tag)
{
	if (strcasecmp(tmpstr,"params")==0) {
	}
	else if (strcasecmp(tmpstr,"param")==0) {
		if (paramset==NULL) { // first addition
			paramset = new ParamSettings; parampset = paramset;
		}
		else { 
			ParamSettings *paramnset = new ParamSettings;
			parampset->next = paramnset; parampset = paramnset;
		}
		sp[sp_size] = new ParamSetParse(parampset);
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

int PParse::parse(FILE* fp) 
{
	unsigned int line = 1;
	unsigned int max_tags=1024;
	err=0;
	bool valid_section_tag = false;
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
			for (i=0; i<NUM_SUBTAGS; i++) // sub tag ?
				if (strcasecmp(tmpstr,sub_tags[i])==0) 
					valid_sub_tag=true;
			if (valid_section_tag) // we're in <params> 
			{
				for (i=0; i<NUM_A_PTAGS; i++) // params tag? 
					if (strcasecmp(tmpstr,params_att_tags[i])==0) 
						valid_att_tag=true;
			}
			if (strcasecmp(tmpstr,"params")==0) {
				valid_section_tag=true; 
				valid_sub_tag=true;
			}
		} while (!feof(fp)&&(!valid_att_tag)&&(!valid_sub_tag));
		if (feof(fp)) return err;
		if (valid_section_tag) 
				parse_section(fp,tmpstr,line,valid_sub_tag,valid_att_tag);
	}
	return err; // got error?
}

int PParse::export_dtd(char* buffer) 
{
	return err;
}

int PParse::export_xsd(char* buffer) 
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
			if (strcasecmp(curr_tag,"param")==0) {
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


bool PParse::get_bool( char *name )
{
	parampset = paramset;
	while(parampset != NULL) {
		if (strcasecmp(name, parampset->name)==0) {
			if (strcasecmp(parampset->value, "true")==0) {
				return true;
			} else
			if (strcasecmp(parampset->value, "false")==0) {
				return false;
			} 
		}
		parampset = parampset->next;
	}
	//err = 1;
	return false;
}

char* PParse::get_string( char *name )
{
	parampset = paramset;
	while(parampset != NULL) {
		if (strcasecmp(name, parampset->name)==0) {
			return (parampset->value);
		}
		parampset = parampset->next;
	}
	//err = 1;
	return NULL;
}

double PParse::get_float( char *name )
{
	parampset = paramset;
	while(parampset != NULL) {
		if (strcasecmp(name, parampset->name)==0) {
			return atof(parampset->value);
		}
		parampset = parampset->next;
	}
	//err = 1;
	return 0.0;
}

long int PParse::get_int( char *name )
{
	parampset = paramset;
	while(parampset != NULL) {
		if (strcasecmp(name, parampset->name)==0) {
			return strtol(parampset->value,NULL,0);
		}
		parampset = parampset->next;
	}
	//err = 1;
	return 0;
}

