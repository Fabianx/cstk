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

int main(int ac, char **args) 
{
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
  
	ClustPlot kp;                 // plots the cluster prototypes
	IParse input;                 // read the profile from an XSD file   
	WParse window;
	KSOMfct ksom;
	KMeans kmeans;
	bool quit=false;
	int ret=0;                    // generic return variable
	char buff[0x10000];           // buffer for error msgs
	
	// parse the input section of the xml:
	FILE* fp = fopen(args[1],"r");
	if (!fp) {printf("Error opening file %s.\n", args[1]); return -1;}
	
	input.init(fp);   // parse file and setup inputcolumns

	if (input.error()) 
		{ input.export_err(buff); printf("Input error: %s\n",buff); return -1;}

	fp = freopen(args[1],"r",fp);
	window.init(fp);  // parse file for visualisation settings & display

	if (window.error()) 
		{ window.export_err(buff); printf("Window error: %s\n",buff); return -1;}

	
	KSOMSettings ksomset;
	ksomset.autol = false;
	ksomset.nfct = EUCLNB;
	ksomset.nb_radius = 1.0;
	ksomset.dist = DIS_EUCL;
	ksomset.max_x = 56;
	ksomset.max_y = 68;
	ksomset.vecdim= 2;

	ksom.create(&ksomset);
	
	KVector kvect(2, false); // false: no stats-keeping 

	DVector *vect = new DVector(2);
	for (int i=0; i<2; i++) 
		vect->set_comp(0,U8B_TYPE,i);
	ksom.initRandom(*vect);
	delete vect;

	while (!quit)
	{
		ret = input.read_dvect();
		if ( ret <= 0 ) 
			printf("Input error(%i)\n",ret);
		else
		{
			//printf("%s\n", input.kvect->to_string("%i "));
			ksom.feed( *(input.dvect), 0.5);
			// plot each cell
			for (vei_t x=0; x<ksomset.max_x; x++) 
				for (vei_t y=0; y<ksomset.max_y; y++) 
				{
					for (ve_t h=0; h<2; h++)
						kvect.add_comp(ksom.getCell(x,y,h),h);
					window.ktplot( x, y, ksomset.max_x, ksomset.max_y, 
					   ((ksom.winner_x==x)&&(ksom.winner_y==y))?8:15,
					   &kvect);
				}
		}
		if (!window.check_events()) quit=true;
	}

	if (fp) fclose(fp);
	return 0;
}
