/***************************************************************************
                           ksomparse.cpp  -  v.0.1
                             -------------------
    begin                : Wed Jan 19 2005
    copyright            : (C) 2005 by Martin Berchtold
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

#include "ksomsetparse.h"

// read a specific attribute's value as a string
int KSOMSetParse::read_set(char* attr, char* value) 
{
	for (int i=0; i<NUM_KSOM_ATTR; i++) 
		if (strcasecmp(attr,ksomset_attr[i])==0) { 
			strcpy(ksomset_val[i],value);
			return 0;
		}
	return -1;
}
	
// update settings from (attribute,value) table, convert from strings
int KSOMSetParse::update_set() 
{
	ksom_set->max_x = strtol(ksomset_val[0],NULL,0);
	ksom_set->max_y = strtol(ksomset_val[1],NULL,0);
	ksom_set->vecdim = strtol(ksomset_val[2],NULL,0);
	if (strcasecmp(ksomset_val[3],"on")==0)
			ksom_set->autol = true;
	else if (strcasecmp(ksomset_val[3],"off")==0)
			ksom_set->autol = false;
	ksom_set->lfct = strtol(ksomset_val[4],NULL,0);
	ksom_set->c = strtod(ksomset_val[5],NULL);
	ksom_set->epoch = strtol(ksomset_val[6],NULL,0);
	ksom_set->nfct = strtol(ksomset_val[7],NULL,0);
	ksom_set->nb_radius = strtod(ksomset_val[8],NULL);
	ksom_set->dist = strtol(ksomset_val[9],NULL,0);
	ksom_set->minkexp = strtol(ksomset_val[10],NULL,0);
	ksom_set->d = strtod(ksomset_val[11],NULL);
	ksom_set->roh = strtod(ksomset_val[12],NULL);
	return 0;
}

// write the DTD for the settings to buffer
int KSOMSetParse::write_dtd(char* buffer) 
{
	char tmpstr[256];
	strcpy(buffer,"\t<!ELEMENT ksom>\n"); 
	for (int i=0; i<NUM_KSOM_ATTR; i++) {
		sprintf(tmpstr, "\t\t<!ATTLIST   ksom %s\t%s\t\"%s\">\n", 
			ksomset_attr[i], ksomset_type[i], ksomset_defs[i]);
		strcat(buffer,tmpstr); // add to buffer
	}
	return 0;
}

// write the rs232 settings in XML format to buffer 
int KSOMSetParse::write_set(char* buffer) 
{
	char tmpstr[256];
	strcpy(buffer,"\t<ksom "); // start at the beginning..
	for (int i=0; i<NUM_KSOM_ATTR; i++) {
		sprintf(tmpstr, "%s=\"%s\" ", ksomset_attr[i],
			ksomset_val[i]);
		strcat(buffer,tmpstr); // add to buffer
	}
	strcat(buffer,">\n");	
	return 0;
}
