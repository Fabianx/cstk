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
#include "misc/conio.h" // keyboard input
#include <stdio.h>  // strings
#include <dirent.h> // directories
#include <sys/time.h> // time adjusting

#define SBUFSIZE 512
#define BUFSIZE  400
#define PLOTFREQ 3

int main(void) {
  
  int i=0, j=0;
  char filename[1024]; // name of the usb device
  char c; 
  DIR* devs;
  struct dirent *pent;
  ConIO conio;

  devs = opendir("/dev/");
  if (!devs)
  {
      printf ("Cannot open the /dev directory!\nWhat kind of OS is this?\n");
      return 1;
  }
  int errno=0;
  filename[0]=0;
  while ((pent=readdir(devs)))
  {
      c = 0;
      if ((pent->d_name[0]=='t')&&(  // quick check for tty...
              ((pent->d_name[3]=='.')&&(pent->d_name[4]=='u')) ||  // tty.usb... 
              ((pent->d_name[3]=='A')&&(pent->d_name[4]=='C'))     // ttyACM...
          ))
      {
          sprintf(filename, "/dev/%s", pent->d_name );
          printf("\nFound potential porcupine2 at: %s\nUse this one? [Y/n]\n",
                 filename);
          while (c==0)
          {
              c = conio.kb_getc();
          }
      }
      if ((c=='Y')||(c=='y')||(c==10))
          break; // go with the last found serusb
  }
  if ((errno)||(filename[0]==0)||(c==0)||(c=='n'))
  {
      printf ("\nNo porcupine2 found.\nIs it connected properly?\n");
      return 1;
  }
  closedir(devs);

  Rs232Parser pr(B38400,SBUFSIZE,"",filename, ASC_MODE);
  char ser_buffer[SBUFSIZE];
  char line_buffer[SBUFSIZE];
  bool quit=false;

  i=0;
  while ((i++<20)&&(j!=13))
  {
      pr.set_poll("t");
      j = pr.read(ser_buffer);
      usleep(1000*i); // wait longer to accomodate faster computers
  }
  if (j==13)
  {
      printf("\nPorcupine2's current date and time are: %c%c/%c%c/20%c%c",
             ser_buffer[0],ser_buffer[1],ser_buffer[2],
             ser_buffer[3],ser_buffer[4],ser_buffer[5] );
      printf(" %c%c:%c%c:%c%c\n",
             ser_buffer[6],ser_buffer[7],ser_buffer[8],ser_buffer[9],
             ser_buffer[10],ser_buffer[11]);
      printf("Update with the host's current time and date? [Y/n]\n");
      c=0;
      while (c==0)
      {
          c = conio.kb_getc();
      }
      if ((c=='y')||(c=='Y')||(c==10))
      {
          time_t tloc;
          tm *tms;
          time(&tloc);
          tms = localtime(&tloc);
          
          // write current date&time
          pr.writechar('T');          
          pr.writechar(tms->tm_mday);
          pr.writechar(tms->tm_mon+1);
          pr.writechar(tms->tm_year-100);
          pr.writechar(tms->tm_wday);
          pr.writechar(tms->tm_hour);
          pr.writechar(tms->tm_min);
          pr.writechar(tms->tm_sec);
          printf("Porcupine2's time set to %i/%i/20%i %02i:%02i:%02i\n",
                 tms->tm_mday, tms->tm_mon+1, tms->tm_year-100,
                 tms->tm_hour, tms->tm_min, tms->tm_sec);
      }
  }
  else
  {
      printf("\nCould not access the porcupine2's realtime clock.\n");
  }

  pr.set_poll("g");
  j = pr.read(ser_buffer);
  pr.set_poll("");
  
  WindowSettings winset;
  winset.xpos   = 10;
  winset.ypos   = 10;
  winset.width  = 650;
  winset.height = 550;
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

  char tmpstr[127];

  int evnt;
  int line_iter = 0;

  bool line_done = false;
  
  int counter = 0;
  int sec_counter = 0; // for calculating Hz
  int max_sec_counter = 0;
  char sec_char = 0;
  int plot_counter = 0;
      
  FILE *log;

  log = fopen("log.txt","w");
  
  while (!quit) 
  {
	evnt = window->eventloop();
	if (evnt == 24) quit = true;

	for (i=0; i<SBUFSIZE; i++) 
		ser_buffer[i] = 0; 

	j = pr.read(ser_buffer); 
	ser_buffer[j-1]=0;

	for (i=0;i<j-1;i++) 
	{
		if (ser_buffer[i]=='\n')
                {
			line_iter = 0;
                        line_done = true;
		}
                else {
			line_buffer[line_iter] = ser_buffer[i];
			if (line_iter<SBUFSIZE-1) 
				line_iter++;
                        line_done = false;
		}
	}

        plot_counter++;
        if (plot_counter==PLOTFREQ)
        {
            plot_counter=0;
        }
        
	// cheap sanity check 
	if ((line_done) && (j>0))
	{
		sscanf(line_buffer,
                       "Acc[ %3i %3i %3i] Tmp[%3i] Lgt[%3i%3i] Tlt[%c%c%c%c%c%c%c%c%c]",
                       &x,&y,&z, &t, &l1,&l2, &t1,&t2,&t3,&t4,&t5,&t6,&t7,&t8,&t9);

                    vectx.add_comp(x);
                    vecty.add_comp(y);
                    vectz.add_comp(z);
                    counter++;
                    if (counter%10==0)
                    {
                        vectl1.add_comp(l1);
                        vectl2.add_comp(l2);
                    }
                    if (counter==100)
                    {
                        vectt.add_comp(t);
                        counter=0;
                    }
                    vectt1.add_comp( (t1=='0')?0:20 );
                    vectt2.add_comp( (t2=='0')?30:50 );
                    vectt3.add_comp( (t3=='0')?60:80 );
                    vectt4.add_comp( (t4=='0')?90:110 );
                    vectt5.add_comp( (t5=='0')?120:140 );
                    vectt6.add_comp( (t6=='0')?150:170 ); 
                    vectt7.add_comp( (t7=='0')?180:200 );
                    vectt8.add_comp( (t8=='0')?210:230 );
                    vectt9.add_comp( (t9=='0')?240:255 );

                if (plot_counter==PLOTFREQ-1)
                {
                    sprintf( tmpstr, "AccX [ %i %i %i ] @ %iHz", vectx.min(), 
                             vectx.mean(), vectx.max(), max_sec_counter);
                    kp.timeseries(1, 5, vectx, 1, tmpstr, false);
                    sprintf( tmpstr, "AccY [ %i %i %i ]", vecty.min(), 
			vecty.mean(), vecty.max());
                    kp.timeseries(2, 5, vecty, 2, tmpstr, false);
                    sprintf( tmpstr, "AccZ [ %i %i %i ]", vectz.min(), 
			vectz.mean(), vectz.max());
                    kp.timeseries(3, 5, vectz, 0, tmpstr, false);
                    sprintf( tmpstr,
                             "Temp [ %i %i %i ]  Light [ %i %i %i ] [ %i %i %i ]",
                             vectt.min(), vectt.mean(), vectt.max(), vectl1.min(),
                             vectl1.mean(), vectl1.max(), vectl2.min(), vectl2.mean(),
                             vectl2.max() );
                    kp.labelplot(4, 5, tmpstr, 0, 0, 0);
                    kp.timeseries(4, 5, vectt, 4, NULL, false);
                    kp.timeseries(4, 5, vectl1, 0, NULL, false);
                    kp.timeseries(4, 5, vectl2, 1, NULL, false);
                    kp.timeseries(5, 5, vectt1, 0, NULL, false);
                    kp.timeseries(5, 5, vectt2, 1, NULL, false);
                    kp.timeseries(5, 5, vectt3, 2, NULL, false);
                    kp.timeseries(5, 5, vectt4, 3, NULL, false);
                    kp.timeseries(5, 5, vectt5, 4, NULL, false);
                    kp.timeseries(5, 5, vectt6, 9, NULL, false);
                    kp.timeseries(5, 5, vectt7, 6, NULL, false);
                    kp.timeseries(5, 5, vectt8, 7, NULL, false);
                    kp.timeseries(5, 5, vectt9, 8, NULL, false);
                    line_buffer[80]=0;
                    kp.labelplot(5,5, line_buffer, 0, 0, 0);
                    window->swap_buffers();  
                    window->delay(1000);
                }
        }
        
        if ((line_done)&&(j>0))
        {
            line_buffer[62]=0;
            fprintf(log,"%c%c.%c%c.%c%c %c%c:%c%c:%c%c %s\n",
                        line_buffer[67], line_buffer[68],
                        line_buffer[69], line_buffer[70],
                        line_buffer[71], line_buffer[72],
                        line_buffer[73], line_buffer[74],
                        line_buffer[75], line_buffer[76],
                        line_buffer[77], line_buffer[78],
                    line_buffer);
        }

        if ((line_done)&&(j>0))
        {
            // update Hz calculations:
            sec_counter++;
            if (line_buffer[78]!=sec_char)
            {
                sec_char = line_buffer[78];
                max_sec_counter = sec_counter;
                sec_counter=0;
            }
        }
        
  } 
  fclose(log);
  return 0;
  
}
