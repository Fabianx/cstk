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
#include "sensordata/sensordata.h"
#include "sensordata/rs232parser/rs232parser.h"
#include "sensordata/logfileparser/logfileparser.h"
#include "sensordata/udpparser/udpparser.h"
#include "sensordata/simparser/simparser.h"

 
#define ERR_INVATTR      12
#define ERR_INVTAG       13
#define ERR_INVCH        14
#define ERR_TAGOVERFLOW  15
#define ERR_NOINPUT      16
#define ERR_UPDATE       17
#define ERR_NOFILE       20
#define ERR_NOSET        21
#define ERR_CHSET        30
#define ERR_ICOLSET		 31

#define MAX_TAG_LENGTH   255
#define MAX_DTD_LENGTH   2048
#define MAX_XSD_LENGTH   1024

// CSTK tags linked to settings:
#define NUM_A_ITAGS 7
const char input_att_tags[NUM_A_ITAGS][16] = {
	"rs232", "udp", "logfile", "sim", 
	"channel", "inputcolumn", "poll"
	};
// valid but empty CSTK tags:
#define NUM_S_ITAGS 2
const char input_sub_tags[NUM_S_ITAGS][16] = { "!--", "packet" };

class KProf {
 public:	
	KProf();
	~KProf();
	int parse(FILE* fp);
	int export_dtd(char* buffer);
	int export_xsd(char* buffer);
	int setup_sensordata_parser();
	
	SensorData *sd; // this will point at the device!
	int err;
	unsigned int errline;
	unsigned int line;
	
	char *chs;             	// array of types (char) in channels
	unsigned int num_chs;  	// number of channels
	DataCell *icols;       	// datacell array of all input columns 
	unsigned int *filter;   // array of mapping filter
	unsigned int num_icols;	// number of input columns
 private:
	SetParse* *sp; 	// dynamic array of all settings tags
	unsigned int sp_size; 	// 
	//----- These are valid settings lists: ---------------
	 Rs232ParserSettings   	*rs232set;
	 UDPParserSettings     	*udpset;
	 LogFileParserSettings 	*logfileset;
	 SimParserSettings     	*simset;
	 ChannelSettings       	*chset; 
	 InputColumnSettings   	*icolset;
	//-----------------------------------------------------
};
