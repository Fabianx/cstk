/***************************************************************************
                            topoplot.cpp  -  v1.00
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

#include "cstk_base/vector/kvector.h"
#include "cstk_base/vector/dvector.h"
#include "viz/x11/clustplot.h"
#include "kprof/kprof.h"
#include "sensordata/udpparser/udpparser.h"
#include "sensordata/rs232parser/rs232parser.h"
#include "sensordata/logfileparser/logfileparser.h"
#include "sensordata/simparser/simparser.h"
#include <stdlib.h>  //strcmp()
#include "algorithms/gng/gng.h"   

#define border1 8

int main(int ac, char **args) {

  char window_name[255];
  ClustPlot kp;                  // plots the cluster prototypes
  SensorData *sd = NULL;         // generic sensor data class
  KProf kprof;                   // read the profile from an XSD file
  GNG gng;
  int ret=0;
  bool quit=false;
  unsigned int i;
  char* channel_types = NULL;    // these variables are for passing 
  DataCell* columns=NULL;        // information along to the read methods
  uint* select=NULL;             // of the various types of sensordata
    
  if (ac<2) {
    printf("\n 2DPlot - Martin Berchtold.");
    printf("\n syntax:");
    printf("\n   %s <xml settings document>",args[0]);
    printf("\n");
    printf("\n\n More info can be found in the CSTK manual");
    printf("\n\n (http://cstk.sf.net/). No really, read it.");
    printf("\n\n");
    exit(0);
  }
 
  
 	// parse the xml file:
	if (kprof.parse(args[1],SETTP_TPP)) {printf("parse error\n\r");exit(1);};
	sprintf(window_name,"2Dplot:%s,%s", imodes[kprof.input_mode],
		omodes[kprof.output_mode]);
	sprintf(window_name,"%s (%ix%i,%1.3f,%1.2f)", window_name, 
		kprof.par.gridsize[0], kprof.par.gridsize[1], kprof.par.lr, 
		kprof.par.nbr);
	
	// Create the window..
	kp.create(kprof.win.xpos, kprof.win.width, kprof.win.ypos, 
		kprof.win.height, kprof.win.border, window_name);
	kp.prepare_colours();
   
	channel_types = new char[kprof.is.numchs];
	select = new uint[kprof.is.numcols];
	columns = new DataCell[kprof.is.numcols];
	kprof.is.firstch(); i=0;
	do 
	{
		channel_types[i] = DC_typecast(kprof.is.get_sign(), kprof.is.get_bits(), kprof.is.get_format());
		i++;
	} 
	while (kprof.is.nextch());
	
   	kprof.is.firstcol(); i=0;
	do 
	{
		select[i] = kprof.is.get_col_channel();
		columns[i].set_type(channel_types[select[i]]);
		columns[i].set_bits(kprof.is.get_col_bits());
		i++; 
	} 
	while (kprof.is.nextcol());

	// set up the input:
	switch (kprof.input_mode) {
		case IMODE_RS232 : 
			sd = new Rs232Parser(*kprof.is.rs232);
			break;
		case IMODE_FILE  : 
			sd = new LogFileParser(kprof.is.filename); 
			break;
		case IMODE_UDP   : 
			sd = new UDPParser(kprof.is.port, 100);
			break;
		case IMODE_SIM   : 
			sd = new SimParser(kprof.is.numcols);
			break;
		case IMODE_PIPE  : break;
		case IMODE_INT   : break;
	}
   
	// set up the output:
	switch (kprof.output_mode) 
	{
		case OMODE_RS232 : break;
		case OMODE_FILE  : break;
		case OMODE_PIPE  : break;
		case OMODE_UDP   : break;
	}
	// prepare the SOM:
	//DVector vect(kprof.is.numcols); // no stats-keeping please
	DVector *vect, *vec1, *vec2;
	KVector kvect(kprof.is.numcols); // no stats-keeping please
	
	vec1 = new DVector(kprof.is.numcols);	
	if ( sd->read(channel_types, kprof.is.numchs, columns, select, kprof.is.numcols)) 
		for (i=0; i<kprof.is.numcols; i++) 
				vec1->set_comp(columns[i].get_u8b(),U8B_TYPE,i);
	 
	vec2 = new DVector(kprof.is.numcols);
	if ( sd->read(channel_types, kprof.is.numchs, columns, select, kprof.is.numcols)) 
		for (i=0; i<kprof.is.numcols; i++) 
				vec2->set_comp(columns[i].get_u8b(),U8B_TYPE,i);
				
	/*vec1->set_comp(345,U8B_TYPE,0);	
	vec1->set_comp(456,U8B_TYPE,1);		
	vec2->set_comp(569,U8B_TYPE,0);	
	vec2->set_comp(455,U8B_TYPE,0);	*/
	gng.create(*vec1,*vec2,25,0.1,0.3,0.006,0.0008);
	delete vec1;
	delete vec2;	
	
	DVector *nodeT, *edgeT;
	int x_max=255, y_max=255;
	
	int count=0, countlin=border1,countlin2=1;
	int clr=0;
	int nodes, nodes_old;
  	// Loop until the task is interrupted:
   	while (quit==false) {
       		// do this several times to speed up viz:
		//usleep(100000);
        	for (int i_step=0; i_step<kprof.win.skip; i_step++) 
		{
			// read the sensordata:
			vect = new DVector(kprof.is.numcols);
			if ( sd->read(channel_types, kprof.is.numchs, columns, select, kprof.is.numcols)) 
			{ 
				for (i=0; i<kprof.is.numcols; i++) 
				{
					vect->set_comp(columns[i].get_u8b(),U8B_TYPE,i);
				} 
				if ((int)(columns[0].get_u8b())>x_max)
					x_max = (int)(columns[0].get_u8b()); //printf("x_max=%i\n",x_max);
				if ((int)(columns[1].get_u8b())>y_max)
					y_max = (int)(columns[1].get_u8b()); //printf("y_max=%i\n",y_max);
					
				//printf("%7.3e %7.3e\n",vect->get_comp(0),vect->get_comp(1));
				//printf("%i %i\n",columns[0].get_u8b(),columns[1].get_u8b());
				kp.drawframe((int)(((float)(columns[0].get_u8b())/x_max)*kprof.win.width), (int)(((float)(columns[1].get_u8b())/y_max)*kprof.win.height), 3, 3);
			}  
			if ((vect->get_comp(0)!=0) and (vect->get_comp(1)!=0))
				gng.feed(*vect);
			delete vect;
		}
		if (gng.getFirst_node() != NULL)
			nodeT = new DVector(*(gng.getFirst_node()));
		else
			nodeT = NULL;
		if (gng.getFirst_edge() != NULL)
			edgeT = new DVector(*(gng.getFirst_edge()));
		else
			edgeT = NULL;
		nodes=0;
		while (nodeT != NULL)
		{	
			nodes++;
			kp.drawframe((int)(((float)(nodeT->get_comp(0))/x_max)*kprof.win.width), (int)(((float)(nodeT->get_comp(1))/y_max)*kprof.win.height), 5, 5);
			if (clr<14)
				clr++;
			else
				clr=0;
			if (gng.getFirst_edge() != NULL)
				edgeT = gng.getFirst_edge();
			else
				edgeT = NULL;
			while (edgeT != NULL)
			{
				kp.drawline((int)(((float)(nodeT->get_comp(0))/x_max)*kprof.win.width), (int)(((float)(nodeT->get_comp(1))/y_max)*kprof.win.height), (int)(((float)(edgeT->get_comp(0))/x_max)*kprof.win.width), (int)(((float)(edgeT->get_comp(1))/y_max)*kprof.win.height),clr);
				edgeT = gng.getNext_edge();;
			}
			nodeT = gng.getNext_node();
		}
		if (nodeT != NULL) delete nodeT;
		if (edgeT != NULL) delete edgeT;
		if (nodes!=nodes_old)
		{
			printf("#nodes=%i\n",nodes);
			nodes_old = nodes;
		}

		kp.swap_buffers();
		kp.delay(1000000/kprof.win.framespersec);

		// interruption?
		switch (ret=kp.eventloop()) 
		{ 
			// press 'q' for quit
			case 24: quit = true;
				break;
			case 82: if (kprof.win.skip>0) kprof.win.skip--;
				break;
			case 86: if (kprof.win.skip<255) kprof.win.skip++;
				break;
			case 33: do {  //kp.swap_buffers();
					kp.delay(kprof.win.framespersec);
				} while (kp.eventloop()!=54);
				break;
			case 80: usleep(200000);
				break;
			case 39:gng.savetoFile();
				printf("GNG web saved to file web.sav\n");
				break;
			case 41:if (!gng.restorefromFile())
					printf("KSOM grid restored from file grid.sav\n");
				break;
		}
		if (count<(countlin*countlin2))
			count++;
		else
		{
			count=0;
			countlin2++;
			countlin = countlin * 1.2;
			gng.newNode();
		}
	}
	if (sd!=NULL) delete sd;
   	if (channel_types!=NULL) delete []channel_types;
   	if (select!=NULL) delete []select;
   	if (columns!=NULL) delete []columns;
	return 0;
}
