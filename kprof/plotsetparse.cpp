/***************************************************************************
                          plotsetparse.cpp  -  v1.00
                          --------------------------
    begin                : Thu Dec 2 2004
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

#include "plotsetparse.h"

/**Class for parsing plot settings
  *@author Kristof Van Laerhoven
  */

// read a specific attribute's value as a string
int PlotSetParse::read_set(char* attr, char* value) {
	for (int i=0; i<NUM_PLOT_ATTR; i++) 
		if (strcasecmp(attr,plotset_attr[i])==0) { 
			strcpy(plotset_val[i],value);
			return 0;
		}
	return -1;
}

// update settings from (attribute,value) table, convert from strings
int PlotSetParse::update_set() {
	plot_set->id = strtol(plotset_val[0],NULL,0);
	if (strcasecmp(plotset_val[1],"timeseries")==0)
			plot_set->type = PLOT_TIME_TYPE;
	else if (strcasecmp(plotset_val[1],"histogram")==0)
			plot_set->type = PLOT_HIST_TYPE;
	else if (strcasecmp(plotset_val[1],"peakplot")==0)
			plot_set->type = PLOT_PEAK_TYPE;
	else return -1;
	strcpy(plot_set->title, plotset_val[2]);
	plot_set->src = strtol(plotset_val[3],NULL,0);
	plot_set->res = strtol(plotset_val[4],NULL,0);
	if (strcasecmp(plotset_val[5],"yes")==0)
			plot_set->scale = true;
	else if (strcasecmp(plotset_val[5],"no")==0)
			plot_set->scale = false;
	else return -1;
	plot_set->color = strtol(plotset_val[6],NULL,0);
	return 0;
}

// write the DTD for the settings to buffer
int PlotSetParse::write_dtd(char* buffer) {
	char tmpstr[256];
	strcpy(buffer,"\t<!ELEMENT plot>\n"); 
	for (int i=0; i<NUM_PLOT_ATTR; i++) {
		sprintf(tmpstr, "\t\t<!ATTLIST   plot %s\t%s\t\"%s\">\n", 
			plotset_attr[i], plotset_type[i], plotset_defs[i]);
		strcat(buffer,tmpstr); // add to buffer
	}
	return 0;
}

// write the ch settings in XML format to buffer 
int PlotSetParse::write_set(char* buffer) {
	char tmpstr[256];
	strcpy(buffer,"\t<plot "); // start at the beginning..
	for (int i=0; i<NUM_PLOT_ATTR; i++) {
		sprintf(tmpstr, "%s=\"%s\" ", plotset_attr[i],
			plotset_val[i]);
		strcat(buffer,tmpstr); // add to buffer
	}
	strcat(buffer,">");
	return 0;
}

