/***************************************************************************
                         kubeviewer.cpp  -  v.1.00
                         ----------------------------
    begin                : Jul 05 2006
    copyright            : (C) 2006 by Kristof Van Laerhoven
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

#include "sensordata/rs232parser/rs232parser.h"
#include "viz/x11/kvplot.h"
#include <stdio.h>  // strings

#define SBUFSIZE 32

#define CONSTANT_PLOT 0
#define CONSTANT_CLAS 1
#define ONEPASS_WE    2
#define ONEPASS_CLAS  3
#define DONOTHING     9

int main(void) {
  
  // plot raw data 'R' from Kube:
  int i=0, j=0;
  Rs232Parser pr(B2400,SBUFSIZE,"R","/dev/ttyUSB1");
  char buffer[SBUFSIZE];
  bool quit=false;
  
  WindowSettings winset;
  winset.xpos   = 100;
  winset.ypos   = 100;
  winset.width  = 250;
  winset.height = 320;
  winset.border = 2;
  winset.framerate=1;
  winset.skip   = 1;
  sprintf(winset.title, "Kube analysis");
  X11Plot *window = new X11Plot(winset);
  window->prepare_colours();
  KVPlot kp(window);
  KVector vectaccx(80); // vector length
  KVector vectaccy(80); // vector length
  KVector vectaccxmin(80); // vector length
  KVector vectaccymin(80); // vector length
  KVector vectaccxmax(80); // vector length
  KVector vectaccymax(80); // vector length
  Peak peakx(80);   // remember the last x peaks
  Peak peaky(80);   // remember the last x peaks
  int evnt;
  
  FILE* fp = NULL;
  int poll=CONSTANT_PLOT; // default
  
  //pr.set_poll("b"); // make sure the comm is going via binary
  //pr.read(buffer);
  //pr.set_poll("R"); // then switch to default again
  
  while (!quit) 
  {
	evnt = window->eventloop();
	switch(evnt) 
	{
		case 24:  /*q*/ quit = true; break;
		case 54:  /*c*/ pr.set_poll("c"); poll = CONSTANT_CLAS; break;
		case 27:  /*r*/ pr.set_poll("R"); poll = CONSTANT_PLOT; break;
		case 46:  /*l*/ pr.set_poll("l"); poll = ONEPASS_WE;  break;
		case 39:  /*s*/ pr.set_poll("s"); poll = ONEPASS_WE;  break;
		case 10:  /*1*/ pr.set_poll("1"); poll = ONEPASS_CLAS;  break;
		case 11:  /*2*/ pr.set_poll("2"); poll = ONEPASS_CLAS;  break;
		case 12:  /*3*/ pr.set_poll("3"); poll = ONEPASS_CLAS;  break;
		case 13:  /*4*/ pr.set_poll("4"); poll = ONEPASS_CLAS;  break;
		case 14:  /*5*/ pr.set_poll("5"); poll = ONEPASS_CLAS;  break;
		case 15:  /*6*/ pr.set_poll("6"); poll = ONEPASS_CLAS;  break;
		//default: printf(">>%i \n\r",evnt); break;
	};
	
	for (i=0; i<SBUFSIZE; i++)
		buffer[i] = 0;
	
	if (poll != DONOTHING) 
		j = pr.read(buffer); 
	
	if ((poll==CONSTANT_PLOT)&&(j==15)) {
		int16_t buff = (unsigned char)buffer[1]*0x100 + (unsigned char)buffer[0];
		char str[SBUFSIZE], str2[SBUFSIZE];
		for (j=0;j<SBUFSIZE;j++) str[j] = ' ';
		for (j=0; j<9; j++) {
			str[j*2+1] = (buff&(0x0001<<(15-j)))?'1':'0';
		}
		str[31] = '\0';
		sprintf(str2, " %03i %03i", (unsigned char)buffer[2], (unsigned char) buffer[3]);
		/*
		fp = fopen("/tmp/kube_read.txt","w");
		for (j=0; j<9; j++) {
			fprintf(fp,"%c", str[j]);
		}
		buffer[0]<<=1;buffer[0]>>=3;
		fprintf(fp," %02i", buffer[0]);
		// accelerometer means:
		fprintf(fp," %03i %03i", (unsigned char)buffer[2], (unsigned char) buffer[3]);
		// accelerometer min:
		fprintf(fp," %03i %03i", (unsigned char)buffer[4], (unsigned char) buffer[5]);
		// accelerometer max:
		fprintf(fp," %03i %03i", (unsigned char)buffer[6], (unsigned char) buffer[7]);
		// accelerometer peak length:
		fprintf(fp," %03i %03i ", (unsigned char)buffer[8], (unsigned char) buffer[9]);
		// accelerometer peak height:
		//if (buffer[10]>128) {buffer[10] = -buffer[10]+1; printf("-");} else printf("+"); 
		fprintf(fp,"%03i ", (signed char)buffer[10]);
		//if (buffer[11]>128) {buffer[11] = -buffer[11]+1; printf("-");} else printf("+"); 
		fprintf(fp,"%03i", (signed char) buffer[11]);
		// accelerometer peak size:
		fprintf(fp," %03i %03i", (unsigned char)buffer[12], (unsigned char) buffer[13]);
		fprintf(fp,"\n\r");
		fclose(fp);*/
		
		vectaccx.add_comp(buffer[2]);   
		vectaccy.add_comp(buffer[3]);   
		vectaccxmin.add_comp(buffer[4]);
		vectaccymin.add_comp(buffer[5]);
		vectaccxmax.add_comp(buffer[6]);
		vectaccymax.add_comp(buffer[7]);
		peakx.add_peak( buffer[12],  buffer[8], abs((signed char)buffer[10]), ((signed char)buffer[10])>0);
		peaky.add_peak( buffer[13],  buffer[9], abs((signed char)buffer[11]), ((signed char)buffer[11])>0);
		kp.histogram(1,6, vectaccx, 0, 58, "histogram X Y");
		kp.histogram(1,6, vectaccy, 1, 58, " ");
		kp.timeseries(2,6, vectaccx, 2, "Acc X", false);
		kp.timeseries(3,6, vectaccy, 2, "Acc Y", false);
		kp.timeseries(2,6, vectaccxmin, 0, " ", false);
		kp.timeseries(3,6, vectaccymin, 0, " ", false);
		kp.timeseries(2,6, vectaccxmax, 1, " ", false);
		kp.timeseries(3,6, vectaccymax, 1, " ", false);
		kp.peakplot(4,6, peakx,"Peaks X");
		kp.timeseries(4,6, vectaccx, 0, " ", true);
		kp.peakplot(5,6, peaky,"Peaks Y");
		kp.timeseries(5,6, vectaccy, 0, " ", true);
		kp.labelplot(6,6, "tilt states:", 0, 0, 0);
		kp.labelplot(6,6, "acceleration:", 0, 15, 0);
		kp.labelplot(6,6, str, 3, 0, 0);
		kp.labelplot(6,6, str2, 3, 18, 1);
		window->swap_buffers();
	}
	else if (poll == CONSTANT_CLAS) 
	{
		char str[SBUFSIZE], str2[SBUFSIZE], str3[SBUFSIZE];
		sprintf(str,  "   %i (%i)", buffer[0], buffer[1]);
		sprintf(str2, "   %i (%i)", buffer[2], buffer[3]);
		sprintf(str3, "   %i", buffer[4]);
		kp.labelplot(1,2, "tilt-based:", 0, 0);
		kp.labelplot(1,2, "acceleration-based:", 0, 15);
		kp.labelplot(2,2, "prediction:", 0, 0);
		kp.labelplot(1,2, str, 7, 0, 1);
		kp.labelplot(1,2, str2, 7, 18, 1);
		kp.labelplot(2,2, str3, 10, 8, 3);
		window->swap_buffers();
	} 
	else if (poll == ONEPASS_WE ) 
	{
		
		poll = DONOTHING;
	}
	else if (poll == ONEPASS_CLAS ) 
	{
		char str[SBUFSIZE], str2[SBUFSIZE], str3[SBUFSIZE];
		sprintf(str,  "   %i (T)", evnt-9);
		sprintf(str2, "   %i (T)", evnt-9);
		sprintf(str3, "   %i", evnt-9);
		kp.labelplot(1,2, "tilt-based:", 0, 0);
		kp.labelplot(1,2, "acceleration-based:", 0, 15);
		kp.labelplot(2,2, "training for:", 0, 0);
		kp.labelplot(1,2, str, 7, 0, 1);
		kp.labelplot(1,2, str2, 7, 18, 1);
		kp.labelplot(2,2, str3, 10, 8, 3);
		window->swap_buffers();
		poll = DONOTHING;
	}
	window->delay(100000);
  } 
  
  return 0;
  
}


/*
			if ((unsigned char)buffer[i] == 0xFF) {
				pre1 = true;
				pre = false;
			}
			else if (pre1)  
			{
				if  ((unsigned char)buffer[i] == 0xFF)
				{
					pre = true;
					printf("\n\r");
				}
				else printf("ff %0x ", (unsigned char)buffer[i] );
				pre1 = false;
			}
			else if ((pre) && ((unsigned char)buffer[i]!=0))
				printf("%x ", (unsigned char)buffer[i] );
*/

