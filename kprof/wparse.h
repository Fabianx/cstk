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

#include "kprof/tags.h"
#include "kprof/setparse.h"
#include "kprof/windowsetparse.h"
#include "kprof/plotsetparse.h"
#include "viz/x11/kvplot.h"
#include "algorithms/peak/peak.h"


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

