/***************************************************************************
                              kprof.h  -  v1.00
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

#include "kprof/setparse.h"
#include "kprof/rs232setparse.h"
#include "kprof/udpsetparse.h"
#include "kprof/logfilesetparse.h"
#include "kprof/simsetparse.h"
#include "kprof/channelsetparse.h"
#include "kprof/inputcolumnsetparse.h"
#include "kprof/windowsetparse.h"
#include "kprof/plotsetparse.h"
#include "sensordata/sensordata.h"
#include "sensordata/rs232parser/rs232parser.h"
#include "sensordata/logfileparser/logfileparser.h"
#include "sensordata/udpparser/udpparser.h"
#include "sensordata/simparser/simparser.h"
#include "viz/x11/kvplot.h"

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

#define MAX_TAG_LENGTH   255
#define MAX_DTD_LENGTH   2048
#define MAX_XSD_LENGTH   1024

#define NUM_KPERRS       12
const char kperr_strings[NUM_KPERRS][32] = 
	{ "No error found.",
	  "Invalid attribute found.",
	  "Invalid tag found.",
	  "Invalid channel found.",
	  "Tag overflow (too many tags).",
	  "No input.",
	  "Error during updating.",
	  "No such file!",
	  "No parser encountered.",
	  "Error setting channels.",
	  "Error setting inputcolumns.",
	  "No inputcolumns found."};


// CSTK tags linked to settings:
#define NUM_A_ITAGS 7
const char input_att_tags[NUM_A_ITAGS][16] = {
	"channel", "inputcolumn", "poll",
	"rs232", "udp", "logfile", "sim"
	};
#define NUM_A_OTAGS 6
const char output_att_tags[NUM_A_OTAGS][16] = {
	"channel", "outputcolumn", "poll",
	"rs232", "udp", "logfile"
	};
#define NUM_A_WTAGS 1
const char window_att_tags[NUM_A_WTAGS][16] = {
	"plot"
	};
// valid but empty CSTK tags:
#define NUM_SUBTAGS 2
const char sub_tags[NUM_SUBTAGS][16] = { 
	"!--", "packet" };
// main section tags:
#define NUM_S_TAGS 3
const char section_tags[NUM_S_TAGS][16] = {
	"input", "output", "window" };


class KProf {
 public:
	KProf();
	~KProf();
	
	int parse(FILE* fp);
	
	int export_dtd(char* buffer);
	int export_xsd(char* buffer);
	
	int setup_sensordata_parser();
	int setup_inputchannels();
	int setup_inputcolumns();
	
	int setup_window();
	int kvplot();
	int read_kvect();
	
	int read_buffer(char* buff);
	int read_icols(void);
	
	void export_err(char* buffer); // print error string in buffer
	bool error() {return err;}
	
	SensorData *sd; // will point to the 1 object generating data
	KVector *kvect;
	KVPlot *kp; // will point to a KVectorPlot generating the plots
	
	
	DataCell *icols;       	// datacell array of all input columns 
	unsigned int num_icols;	// number of input columns
	unsigned int num_plots;	// number of plots
	
 private:
	void parse_input(FILE* fp, char* tmpstr, unsigned int line, 
	                 bool valid_sub_tag, bool valid_att_tag);
	void parse_output(FILE* fp, char* tmpstr, unsigned int line, 
	                 bool valid_sub_tag, bool valid_att_tag);
	void parse_window(FILE* fp, char* tmpstr, unsigned int line, 
	                 bool valid_sub_tag, bool valid_att_tag);
	
	char *ichs;             	// array of types in channels
	unsigned int num_ichs;  	// number of channels
	unsigned int *filter;   	 // array of mapping filter
	
	int err;
	unsigned int errline;
	
	SetParse* *sp; 	// dynamic array of all settings tags
	unsigned int sp_size; 	// 
	//----- These are valid settings lists: ---------------
	 Rs232ParserSettings   	*rs232set;
	 UDPParserSettings     	*udpset;
	 LogFileParserSettings 	*logfileset;
	 SimParserSettings     	*simset;
	 PlotSettings     	*plotset, *plotpset;
	 ChannelSettings       	*ichset, *ichpset;   
	 ChannelSettings       	*ochset, *ochpset;   
	 InputColumnSettings   	*icolset, *icolpset; 
	 WindowSettings 	*winset;
	//-----------------------------------------------------
};

