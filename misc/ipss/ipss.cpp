/***************************************************************************
                        main.cpp  -  ipaq sound sampler
                             -------------------
    begin                : Sun Jan 26 2003
    copyright            : (C) 2003 by Kristof Van Laerhoven
    email                : kristof@comp.lancs.ac.uk
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <unistd.h> // sleep
#include <stdio.h>  // strings

#include <sys/ioctl.h> // dsp
#include <fcntl.h>
#include <linux/soundcard.h>
#include <sys/time.h>
#include <math.h>

#define BSIZE 512

int main() {
  
  int sample = AFMT_S16_LE;      // AFMT_S8
  int channels = 2, speed=16000;
  
  int ret, vol=50;
  int recsrc = 0;
  char buf[BSIZE];
  int blksize;
  int mix = open("/dev/mixer", O_WRONLY);
  if (mix!=-1) {
      if (ioctl(mix, SOUND_MIXER_WRITE_RECSRC, &recsrc)==-1)
        printf("error setting mic\n\r");  // set input to microphone
      if (ioctl(mix, MIXER_WRITE(SOUND_MIXER_MIC), &vol)==-1)
        printf("error setting mic level\n\r");  // set microphone volume
      close(mix);
  } else {
       printf("Error writing to mixer\n");
  }
  int dsp = open("/dev/dsp", O_RDONLY | O_NONBLOCK);
  if (dsp!=-1) {
      if (ioctl(dsp, SNDCTL_DSP_SETFMT, &sample)==-1)
          printf("error set sample format\n");
      if (ioctl(dsp, SNDCTL_DSP_CHANNELS, &channels)==-1)
          printf("error setting channels\n");
      if (ioctl(dsp, SNDCTL_DSP_SPEED, &speed)==-1)
          printf("error setting speed\n");
      if (ioctl(dsp, SNDCTL_DSP_GETBLKSIZE, &blksize)==-1)
          printf("error getting blocksize\n");
      close(dsp);
  } else {
      printf("Error reading dsp\n\r");
  };

  dsp = open("/dev/dsp", O_RDONLY);
      
  unsigned int i;
  float sum;
  float newvar;
  FILE *procf;
  int mean;
  int var;
  
  while(1) { 

     // read microphone
      usleep(20000);
      ret = read(dsp,buf,sizeof(buf));
      sum=0;
      int tmp, tmP;
      
      for (i=0; i<sizeof(buf)/4; i++) {
        tmp = buf[4*i];
        if (tmp>127) tmp -=256;
        tmp += 128;
        tmP = buf[4*i+1];
        if (tmP>127) tmP -=256;
        tmP += 128;

        tmp = tmP*0xFF+tmp;
        
        sum += tmp;
      }
      mean = (int)((float)sum/(sizeof(buf)/4));
      var=0;
      for (i=0; i<sizeof(buf)/4; i++) {
        tmp = buf[4*i];        if (tmp>127) tmp -=256;        tmp += 128;
        tmP = buf[4*i+1];      if (tmP>127) tmP -=256;        tmP += 128;
        tmp = tmP*0xFF+tmp;
                        
        var += (tmp-mean)*(tmp-mean);
      }

      newvar = sqrt(var/(sizeof(buf)/4));
      {
        procf=fopen("/mnt/ramfs/var/mic_level","w");
        fprintf(procf,"%i ", (int)newvar);
        fclose(procf);
      }         
  }
  close(dsp);  

  return(0); 
}
