/***************************************************************************
                            2Dplot.cpp  -  v1.10
                            ----------------------
    begin                : Fri Apr 08 2005
    copyright            : (C) 2003 by Martin Berchtold, Kristof Van Laerhoven
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
#include "kprof/pparse.h"
#include "algorithms/gng/gng.h"
#include <stdlib.h>  //strcmp()

int main(int ac, char **args) 
{

	if (ac<2) {
		printf("\n 2DPlot - by Kristof Van Laerhoven and.");
		printf("\n               Martin Berchtold.");
		printf("\n syntax:");
		printf("\n   %s <xml settings document>",args[0]);
		printf("\n");
		printf("\n\n More info can be found in the CSTK manual");
		printf("\n\n (http://cstk.sf.net/). No really, read it.");
		printf("\n\n");
		return -1;
	}
  
	IParse input;                 // parse XML file
	WParse window;
	PParse params;
	
	GNG gng;
	bool quit=false;
	int ret=0,ret1=0,ret2=0;                    // generic return variable
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

	fp = freopen(args[1],"r",fp);
	params.init(fp);  // parse file for gng and kmeans parameters

	if (params.error()) 
		{ params.export_err(buff); printf("Params error: %s\n",buff); return -1;}
	
		
	params.init(fp);
	GNG_PARAM gngset;
	gngset.age_max    	= params.get_int("maxage");
	gngset.d      		= params.get_float("d");
	gngset.alpha 		= params.get_float("alpha");
	gngset.epsilon_b      	= params.get_float("epsilonb");
	gngset.epsilon_n     	= params.get_float("epsilonn");
	
	// initialize by giving two start vectors:
	ret1=ret2=0;
	
	while ( (ret1<=0) || (ret2<=0) ) {
		ret1 = input.read_dvect();
		ret2 = input.read_dvect();
	}
	DVector *vect1 = new DVector(*(input.dvect));
	DVector *vect2 = new DVector(*(input.dvect));
	gngset.vecdim = 2;
	gng.create(*vect1,*vect2,gngset);
	delete vect1; delete vect2;
	
	KVector kvect(input.num_icols, false); // false: no stats-keeping 

	// counter for amount of nodes
	int nodes;
	// current node and edge pointers
	DVector *nodeT, *edgeT;
	// color counter for the edges
	int clr=0;
	int x_max = 10024;
	int y_max = 10024;
	while (!quit)
	{
		ret = input.read_dvect();
		if ( ret <= 0 ) 
			printf("Input error(%i)\n",ret);
		else
		{
			//printf("%s\n", input.dvect->to_string());
			gng.feed( *(input.dvect));
			// get the first node in the list
			if (gng.getFirst_node() != NULL)
				nodeT = new DVector(*(gng.getFirst_node()));
			else
				nodeT = NULL;
			// get the first edge in the list
			if (gng.getFirst_edge() != NULL)
				edgeT = new DVector(*(gng.getFirst_edge()));
			else
				edgeT = NULL;
			nodes=0;
			
			// plotting the whole lists (node/edge)
			while (nodeT != NULL)
			{
				nodes++;
				window.wn->drawframe(
					(int)(((float)(nodeT->get_comp(0))/x_max)*window.wn->win_width),
					(int)(((float)(nodeT->get_comp(1))/y_max)*window.wn->win_height), 5, 5);
				
				if (clr<14)
					clr++;
				else
					clr=0;
					
				edgeT = gng.getFirst_edge();
				while (edgeT != NULL)
				{
					window.wn->drawline(
						(int)(((float)(nodeT->get_comp(0))/x_max)*window.wn->win_width), 
						(int)(((float)(nodeT->get_comp(1))/y_max)*window.wn->win_height),
						(int)(((float)(edgeT->get_comp(0))/x_max)*window.wn->win_width), 
						(int)(((float)(edgeT->get_comp(1))/y_max)*window.wn->win_height),
						clr);
					edgeT = gng.getNext_edge();;
				}
				nodeT = gng.getNext_node();
			}
			if (nodeT != NULL) delete nodeT;
			if (edgeT != NULL) delete edgeT;
			window.wn->swap_buffers();
		}
		if (!window.check_events()) quit=true;
	}

	if (fp) fclose(fp);
	return 0;
}
