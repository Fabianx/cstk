/***************************************************************************
                          rtplot.cpp  -  v.1.10
                          ---------------------
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

#include "cstk_base/vector/datacell.h"
#include "cstk_base/vector/kvector.h"
#include "cstk_base/vector/dvector.h"
#include "kprof/iparse.h"
#include "kprof/wparse.h"

int main(int ac, char **args) {
  
	if (ac<2) {
		printf("\n RTPlot - by Kristof Van Laerhoven.");
		printf("\n syntax:");
		printf("\n   %s <xml settings document>",args[0]);
		printf("\n");
		printf("\n\n More info can be found in the CSTK manual");
		printf("\n\n (http://cstk.sf.net/). No really, read it.");
		printf("\n\n");
		exit(0);
	}
  
	IParse input;                 // read the profile from an XSD file
	WParse window;
	
	char buff[0x10000];           // buffer for error msgs
	bool quit=false;              // wanna quit?
	int ret=0;                    // generic return variable
	
	FILE* fp = fopen(args[1],"r");
	if (!fp) {printf("Error opening file %s.\n", args[1]); return -1;}

	input.init(fp);   // parse file and setup inputcolumns

	if (input.error()) 
		{ input.export_err(buff); printf("Input: %s\n",buff); return -1;}

	fp = freopen(args[1],"r",fp);
	window.init(fp);  // parse file for visualisation settings & display

	if (window.error()) 
		{ window.export_err(buff); printf("Window: %s\n",buff); return -1;}

	while (!quit)
	{
		ret = input.read_kvect();
		if ( ret <= 0 ) 
			printf("Input error(%i)\n",ret);
		else
		{
			//printf("%s\n", input.kvect->to_string("%i "));
			window.kvplot(input.kvect);
		}
		
		if (!window.check_events()) quit=true;
	}

	if (fp) fclose(fp);
	return 0;
}

