/***************************************************************************
                              plotsetparse.h  -  v1.00
                              ------------------------
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
#ifndef PLOTSETPARSE_H
#define PLOTSETPARSE_H

/**Parser for CSTK plots
  *@author Kristof Van Laerhoven
*/ 
 
#include "setparse.h"
#include <stdlib.h>  // strtol

#define ICOL_INT_FORMAT   1
#define ICOL_FLOAT_FORMAT 2

#define PLOT_TIME_TYPE   1
#define PLOT_HIST_TYPE   2
#define PLOT_PEAK_TYPE   3
#define PLOT_TEXT_TYPE   4
#define PLOT_IMPL_TYPE   5
#define PLOT_SPIK_TYPE   6

#define _SET_PLOTSET_DEF(a) strcpy(plotset_val[(a)],plotset_defs[(a)])

#define NUM_PLOT_ATTR 7
const char plotset_attr[NUM_PLOT_ATTR][16] = {
	"id", "type", "title", "src", "res", "scale", "color" };
const char plotset_type[NUM_PLOT_ATTR][64] = {
	"CDATA", "(timeseries|histogram|peakplot|text|impule|spiketrain)", 
	"CDATA", "CDATA", "CDATA", "(yes|no)", "CDATA"};
const char plotset_defs[NUM_PLOT_ATTR][16] = {
	"0", "timeseries", "", "0", "100", "no", "0"};

struct PlotSettings {
   PlotSettings() {
	id = -1;
	type = -1;
	title[0]='\0';
	src = -1;
	res = 0;
	scale = false;
	color = 0;
	next = NULL;
   }
   int id;
   int type;
   char title[256];
   int src;
   unsigned int res;
   bool scale;
   unsigned int color;
   PlotSettings *next;
};


class PlotSetParse: public SetParse {
 public:
	PlotSetParse(PlotSettings* plot_set)
	{
		this->plot_set = plot_set;
		if (plot_set->id<0)      	_SET_PLOTSET_DEF(0);
		if (plot_set->type<0)      	_SET_PLOTSET_DEF(1);
		if (!plot_set->title)     	_SET_PLOTSET_DEF(2);
		if (plot_set->src<0)      	_SET_PLOTSET_DEF(3);
		if (!plot_set->res)     	_SET_PLOTSET_DEF(4);
		if (!plot_set->scale)     	_SET_PLOTSET_DEF(5);
		if (!plot_set->color)   	_SET_PLOTSET_DEF(6);
	};
	~PlotSetParse() {};
	int read_set(char* attr, char* value);  // read <attribute> <value> pair
	int update_set();                       // flush to ch
	int write_set(char* buffer);            // write to xsd 
	int write_dtd(char* buffer);            // write to dtd
	int write_tag(char* buffer) {strcpy(buffer,"plot");return 0;}
 private:
	PlotSettings* plot_set;
	char plotset_val[NUM_PLOT_ATTR][256];
};

#endif // PLOTSETPARSE_H

