/***************************************************************************
                              parse.h  -  v1.00
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
#ifndef WPARSE_H
#define WPARSE_H

#include "kprof/setparse.h"
#include "kprof/windowsetparse.h"
#include "kprof/plotsetparse.h"
#include "viz/x11/kvplot.h"
#include "algorithms/peak/peak.h"

#define ERR_INVATTR      1
#define ERR_INVTAG       2
#define ERR_INVCH        3
#define ERR_TAGOVERFLOW  4
#define ERR_NOINPUT      5
#define ERR_UPDATE       6
#define ERR_NOFILE       7
#define ERR_NOSET        8
#define ERR_CHSET        9
#define ERR_ICOLSET      10
#define ERR_NOICOLS      11
#define ERR_NOWINDOW     12

#define MAX_TAG_LENGTH   255
#define MAX_DTD_LENGTH   2048
#define MAX_XSD_LENGTH   1024

#define NUM_KPERRS       13
const char kperr_strings[NUM_KPERRS][32] = 
	{ "No error found.",
	  "Invalid attribute found.",
	  "Invalid tag found.",
	  "Invalid channel found.",
	  "Tag overflow (too many tags).",
	  "No input.",
	  "Error during updating.",
	  "No such file.",
	  "No parser encountered.",
	  "Error setting channels.",
	  "Error setting inputcolumns.",
	  "No inputcolumns found.",
	  "No window section found."};

#define NUM_A_WTAGS 1
const char window_att_tags[NUM_A_WTAGS][16] = {
	"plot"
	};

// valid but empty CSTK tags:
#define NUM_SUBTAGS 2
const char sub_tags[NUM_SUBTAGS][16] = {
	"!--", "packet" };

/***************************************************************************
 WParse parses CSTK settings (formatted as an XML file) for window and plot
 settings. It supports currently X11-based visualisation for:
	- rtplot: timeseries, histogram, peakplot, textplot, impulse, 
	          spiketrain (as rows)
	- topoplot: barplot, linesplot (as cells)
	- 2dplot: 2dplot (fills whole window)
 ***************************************************************************/	
class WParse {
 public:
	WParse();
	~WParse();
	
	int parse(FILE* fp);
	
	int export_dtd(char* buffer);
	int export_xsd(char* buffer);
	
	int setup_window();
	int kvplot(KVector *kvect, Peak *kpeak);
	int check_events();
	
	void init(FILE* fp);
	
	void export_err(char* buffer); // print error string in buffer
	bool error() {return err;}
	
	KVPlot *kp;     	// KVectorPlot generating the plots
	Pk pk;          	// Peak struct
	
	unsigned int num_plots;	// number of plots
	
 private:
	void parse_window(FILE* fp, char* tmpstr, unsigned int line, 
	                 bool valid_sub_tag, bool valid_att_tag);
	
	int err;
	unsigned int errline;
	
	SetParse* *sp; 	// dynamic array of all settings tags
	unsigned int sp_size; 	// 
	//----- These are valid settings lists: ---------------
	 PlotSettings     	*plotset, *plotpset;
	 WindowSettings 	*winset;
	//-----------------------------------------------------
};

#endif // WPARSE_H

