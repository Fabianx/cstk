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
#ifndef TAGS_H
#define TAGS_H

// types of errors that can occur while parsing the tags:
enum { ERR_INVATTR, ERR_INVTAG, ERR_INVCH, ERR_TAGOVERFLOW, ERR_NOINPUT,
       ERR_UPDATE,  ERR_NOFILE, ERR_NOSET, ERR_CHSET,       ERR_ICOLSET,
       ERR_NOICOLS, ERR_NOWINDOW };

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
#define NUM_A_PTAGS 1
const char params_att_tags[NUM_A_WTAGS][16] = {
	"param"
	};
// valid but empty CSTK tags:
#define NUM_SUBTAGS 3
const char sub_tags[NUM_SUBTAGS][16] = {
	"!--", "packet", "cstksettings" };
// main section tags:
#define NUM_S_TAGS 4
const char section_tags[NUM_S_TAGS][16] = {
	"input", "output", "window", "params" };

#endif  // TAGS_H
