/***************************************************************************
                              iparse.h  -  v1.10
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
#ifndef IPARSE_H
#define IPARSE_H

#include "cstk_base/vector/binvector.h"
#include "cstk_base/vector/kvector.h"
#include "cstk_base/vector/dvector.h"
 
#include "kprof/setparse.h"
#include "kprof/rs232setparse.h"
#include "kprof/udpsetparse.h"
#include "kprof/logfilesetparse.h"
#include "kprof/simsetparse.h"
#include "kprof/channelsetparse.h"
#include "kprof/inputcolumnsetparse.h"
#include "kprof/windowsetparse.h"
#include "kprof/plotsetparse.h"

#include "sensordata/rs232parser/rs232parser.h"
#include "sensordata/logfileparser/logfileparser.h"
#include "sensordata/udpparser/udpparser.h"
#include "sensordata/simparser/simparser.h"

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

#define NUM_KPERRS       12
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
	  "No inputcolumns found." };

// CSTK tags linked to settings:
#define NUM_A_ITAGS 7
const char input_att_tags[NUM_A_ITAGS][16] = {
	"channel", "inputcolumn", "poll",
	"rs232", "udp", "logfile", "sim"
	};
// valid but empty CSTK tags:
#define NUM_SUBTAGS 2
const char sub_tags[NUM_SUBTAGS][16] = {
	"!--", "packet" };

class IParse {
 public:
	IParse();
	~IParse();

	void init(FILE* fp);
	int parse(FILE* fp);

	int read_icols(void);
	int read_binvect();
	int read_kvect();
	int read_dvect();
	int read_buffer(char* buff);

	int export_dtd(char* buffer);
	int export_xsd(char* buffer);
	void export_err(char* buffer); // print error string in buffer
	bool error() {return err;}

	SensorData *sd;    	// object that is generating data
	BinVector *binvect;	// binary vector
	KVector *kvect;    	// unsigned 8-bit vector
	DVector *dvect;    	// generic vector

	DataCell *icols;       	// datacell array of all input columns 
	unsigned int num_icols;	// number of input columns

 private:
	int setup_sensordata_parser();
	int setup_inputchannels();
	int setup_inputcolumns();

	void parse_input(FILE* fp, char* tmpstr, unsigned int line, 
	                 bool valid_sub_tag, bool valid_att_tag);

	char *ichs;             	// array of types in channels
	unsigned int num_ichs;  	// number of channels
	unsigned int *filter;   	// array of mapping filter

	int err;
	unsigned int errline;

	SetParse* *sp; 	// dynamic array of all settings tags
	unsigned int sp_size; 	// & its size
	
	//----- These are valid settings lists: ---------------
	 Rs232ParserSettings   	*rs232set;
	 UDPParserSettings     	*udpset;
	 LogFileParserSettings 	*logfileset;
	 SimParserSettings     	*simset;
	 ChannelSettings       	*ichset, *ichpset;   
	 ChannelSettings       	*ochset, *ochpset;   
	 InputColumnSettings   	*icolset, *icolpset; 
	//-----------------------------------------------------
};

#endif // IPARSE_H
