/***************************************************************************
                           windowsetparse.h  -  v.0.1
                             -------------------
    begin                : Fri Jan 14 2005
    copyright            : (C) 2005 by Kristof Van Laerhoven
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

#ifndef WINDOWSETPARSE_H
#define WINDOWSETPARSE_H

#include "setparse.h"
#include <stdlib.h>  // atoi
#include "viz/x11/x11plot.h" // windowsettings

#define _SET_WINDOWSET_DEF(a) strcpy(windowset_val[(a)],windowset_defs[(a)])

#define NUM_WINDOW_ATTR 8
const char windowset_attr[NUM_WINDOW_ATTR][16] = { 
	"xpos", "ypos", "width", "height", "border", "framerate", "skip", "title" };
const char windowset_type[NUM_WINDOW_ATTR][256] = { 
	"CDATA", "CDATA", "CDATA", "CDATA", "CDATA", "CDATA", "CDATA", "CDATA" };
const char windowset_defs[NUM_WINDOW_ATTR][16] = { 
	"1", "1", "240", "320", "1", "100", "3", "" };

/**Class for parsing window settings 
  *@author Kristof Van Laerhoven
  */

class WindowSetParse: public SetParse {
 public:
	WindowSetParse(WindowSettings* window_set)
	{
		this->window_set = window_set;
		if (window_set->xpos<0) 	_SET_WINDOWSET_DEF(0);
		if (window_set->ypos<0) 	_SET_WINDOWSET_DEF(1);
		if (window_set->width<0)	_SET_WINDOWSET_DEF(2);
		if (window_set->height<0) 	_SET_WINDOWSET_DEF(3);
		if (window_set->border<0) 	_SET_WINDOWSET_DEF(4);
		if (window_set->framerate<0) 	_SET_WINDOWSET_DEF(5);
		if (window_set->skip<0) 	_SET_WINDOWSET_DEF(6);
		if (!window_set->title) 	_SET_WINDOWSET_DEF(7);
	};
	~WindowSetParse() {};
	int read_set(char* attr, char* value);  // read <attribute> <value> pair
	int update_set();                       // flush to window
	int write_set(char* buffer);            // write to xsd 
	int write_dtd(char* buffer);            // write to dtd
	int write_tag(char* buffer) {strcpy(buffer,"window");return 0;}

 private:
	WindowSettings* window_set;
	char windowset_val[NUM_WINDOW_ATTR][256];
};

#endif
