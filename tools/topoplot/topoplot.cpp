/***************************************************************************
                            topoplot.cpp  -  v1.10
                            ----------------------
    begin                : Mon Nov 24 2003
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
#include "algorithms/ksom/ksom.h"   
#include "algorithms/kmeans/kmeans.h"
#include <stdlib.h>  //strcmp()

#define border1 2000  // martin, what's this?

int main(int ac, char **args) {

	ClustPlot kp;                  // plots the cluster prototypes
	IParse iparse;                 // read the profile from an XSD file
	WParse wparse;
	KSOMfct ksom;
	KMeans kmeans;
	
	if (ac<2) {
		printf("\n TopoPlot - by Kristof Van Laerhoven and.");
		printf("\n               Martin Berchtold.");
		printf("\n syntax:");
		printf("\n   %s <xml settings document>",args[0]);
		printf("\n");
		printf("\n\n More info can be found in the CSTK manual");
		printf("\n\n (http://cstk.sf.net/). No really, read it.");
		printf("\n\n");
		return -1;
	}
  
	// parse the input section of the xml:
	FILE* fp = fopen(args[1],"r");
	if (!fp) {printf("Error opening file %s.\n", args[1]); return -1;}
	
	
	
	return 0;
}
