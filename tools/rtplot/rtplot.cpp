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

#include "kprof/kprof.h"

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
  
	char buff[0x10000]; // for error msgs
	KProf kp;           // reads xml file and plots
	
	FILE* fp = fopen(args[1],"r");
	if (!fp) {printf("Error opening file %s.\n", args[1]); return -1;}
	
	kp.init(fp);
	if (kp.error()) { kp.export_err(buff); printf("%s",buff); exit(-1);}
	
	bool quit = false;
	while (!quit) {
		kp.read_kvects();
		kp.kvplot();
		if (kp.check_events()==0) 
			quit = true; 
	}

	return 0; // no error
}

