/***************************************************************************
                         porc2viewer.cpp  -  v.1.00
                         ----------------------------
    begin                : Dec 01 2006
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
#include <dirent.h> // directories

#define SBUFSIZE 512
#define BUFSIZE  500 

int main(void) {
  
  int i=0, j=0;
  char filename[1024]; // name of the usb device
  DIR* devs;
  struct dirent *pent;
  devs = opendir("/dev/");
  if (!devs)
  {
      printf ("cannot open /dev directory\n");
      exit(1);
  }
  int errno=0;
  filename[0]=0;
  while ((pent=readdir(devs)))
  {
      if ((pent->d_name[0]=='t')&&(  // quick check for tty...
              ((pent->d_name[3]=='.')&&(pent->d_name[4]=='u')) ||  // tty.usb... 
              ((pent->d_name[3]=='U')&&(pent->d_name[4]=='S'))     // ttyUSB...
          ))
      {
          sprintf(filename, "/dev/%s", pent->d_name );
          printf("%s\n", filename);
      }
  }
  if ((errno)||(filename[0]==0))
  {
      printf ("cannot find a porcupine2 via any usb\n");
      exit(1);
  }
  closedir(devs);

  Rs232Parser pr(B115200,SBUFSIZE,"",filename);
  char ser_buffer[SBUFSIZE];
  char line_buffer[SBUFSIZE];
  bool quit=false;
  
  WindowSettings winset;
  winset.xpos   = 100;
  winset.ypos   = 100;
  winset.width  = 590;
  winset.height = 600;
  winset.border = 2;
  winset.framerate= 1;
  winset.skip   = 1;
  sprintf(winset.title, "Porcupine2 Viewer");
  X11Plot *window = new X11Plot(winset);
  window->prepare_colours();

  KVPlot kp(window);
  KVector vectx(BUFSIZE), vecty(BUFSIZE), vectz(BUFSIZE);
  KVector vectt(BUFSIZE), vectl1(BUFSIZE), vectl2(BUFSIZE);
  KVector vectt1(BUFSIZE), vectt2(BUFSIZE), vectt3(BUFSIZE);
  KVector vectt4(BUFSIZE), vectt5(BUFSIZE), vectt6(BUFSIZE);
  KVector vectt7(BUFSIZE), vectt8(BUFSIZE), vectt9(BUFSIZE);

  int x,y,z,t,l1,l2;
  char t1, t2, t3, t4, t5, t6, t7, t8, t9;

  int evnt;
  int line_iter = 0;

  while (!quit) 
  {
	evnt = window->eventloop();
	if (evnt == 24) quit = true;	 

	for (i=0; i<SBUFSIZE; i++) 
	{
		ser_buffer[i] = 0; 
	}

	j = pr.read(ser_buffer); 
	ser_buffer[j-1]=0;

	for (i=0;i<j-1;i++) 
	{
		if (ser_buffer[i]=='\n')
			line_iter = 0;
		else {
			line_buffer[line_iter] = ser_buffer[i];
			if (line_iter<SBUFSIZE-1) 
				line_iter++;
		}
	}

	// cheap sanity check 
	if (line_buffer[0] == 'A') 
	{
		sscanf(line_buffer,
                       "Acc[%3i %3i %3i]  Tmp[%3i]  Lgt[%3i %3i] Tlt[%c%c%c%c%c%c%c%c%c]",
                       &x,&y,&z, &t, &l1,&l2, &t1,&t2,&t3,&t4,&t5,&t6,&t7,&t8,&t9);
		vectx.add_comp(x);
		vecty.add_comp(y);
		vectz.add_comp(z);
		vectt.add_comp(t);
		vectl1.add_comp(l1);
		vectl2.add_comp(l2);
		vectt1.add_comp( (t1=='0')?0:20 );
		vectt2.add_comp( (t2=='0')?30:50 );
		vectt3.add_comp( (t3=='0')?60:80 );
		vectt4.add_comp( (t4=='0')?90:110 );
		vectt5.add_comp( (t5=='0')?120:140 );
		vectt6.add_comp( (t6=='0')?150:170 ); 
		vectt7.add_comp( (t7=='0')?180:200 );
		vectt8.add_comp( (t8=='0')?210:230 );
		vectt9.add_comp( (t9=='0')?240:255 );

		kp.timeseries(1, 7, vectx, 1, "AccX", false);
		kp.timeseries(2, 7, vecty, 2, "AccY", false);
		kp.timeseries(3, 7, vectz, 0, "AccZ", false);
		kp.timeseries(4, 7, vectt, 4, "Temp", true);
		kp.timeseries(5, 7, vectl1, 0, "Light Sensors", false);
		kp.timeseries(5, 7, vectl2, 1, "", false);
		kp.timeseries(6, 7, vectt1, 0, "", false);
		kp.timeseries(6, 7, vectt2, 1, "", false);
		kp.timeseries(6, 7, vectt3, 2, "", false);
		kp.timeseries(6, 7, vectt4, 3, "", false);
		kp.timeseries(6, 7, vectt5, 4, "", false);
		kp.timeseries(6, 7, vectt6, 9, "", false);
		kp.timeseries(6, 7, vectt7, 6, "", false);
		kp.timeseries(6, 7, vectt8, 7, "", false);
		kp.timeseries(6, 7, vectt9, 8, "Tilt Switches", false);
		kp.labelplot(7,7, line_buffer, 3, 2, 0);
		window->swap_buffers();
	}
	window->delay(1000);
  } 
  
  return 0;
  
}
