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
 
#include "kprof/tags.h"
#include "kprof/setparse.h"
#include "kprof/rs232setparse.h"
#include "kprof/udpsetparse.h"
#include "kprof/logfilesetparse.h"
#include "kprof/simsetparse.h"
#include "kprof/channelsetparse.h"
#include "kprof/inputcolumnsetparse.h"

#include "sensordata/rs232parser/rs232parser.h"
#include "sensordata/logfileparser/logfileparser.h"
#include "sensordata/udpparser/udpparser.h"
#include "sensordata/simparser/simparser.h"


class IParse 
{
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
