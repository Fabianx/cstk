/***************************************************************************
                             simparser.cpp  -  v.1.0
                             -----------------------
    begin                : Mon Aug 18 2003
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

#include "simparser.h"

/**Sensor Simulation Parser
  *@author Kristof Van Laerhoven
  */

SimParser::SimParser()
{
  size=0;
  bias = NULL;
  incr = NULL;
  low_lim = NULL;
  upp_lim = NULL;
    
  sim = NULL;
}
  
SimParser::SimParser(unsigned int num)
{
  create(num);
}

SimParser::~SimParser()
{
  if (sim!=NULL) delete[] sim;
  if (bias!=NULL) delete[] bias;
  if (incr!=NULL) delete[] incr;
  if (low_lim!=NULL) delete[] low_lim;
  if (upp_lim!=NULL) delete[] upp_lim;
}

/* create the right array */
void SimParser::create(unsigned int num)
{
  size = num;
  sim = new signed int[size];
  bias = new signed int[size];
  incr = new float[size];
  low_lim = new float[size];
  upp_lim = new float[size];
  
  for (uint i=0; i<size; i++) {
    sim[i] = 0; 
    bias[i] = 0;
    incr[i] = 0; 
    low_lim[i] = 0;
    upp_lim[i] = 0;
  }
}

int SimParser::read(char* channel_types, uint numchannels, 
                DataCell* columns,  uint* filter, uint numcolumns) {

  // woopety woo: we can ignore the channels, only the columns count
  if (numcolumns != size) 
        create(numcolumns);
  // then serialise everything:
  unsigned int i;
  int lower = 50;
  int upper = 200;
  int baserate = 2;
  float incr = 20;
  for (i=0; i<size; i++) {
     if (sim[i] < lower) sim[i] += baserate;
     if (sim[i] > upper) sim[i] -= baserate;
     bias[i] = (signed int)((2*incr*rand())/RAND_MAX)-incr;
     if ((sim[i]+bias[i])<0)   sim[i] = 50+(127*rand())/RAND_MAX; else
     if ((sim[i]+bias[i])>255) sim[i] = 50+(127*rand())/RAND_MAX; else
     sim[i] += bias[i];
     columns[i].set_u8val( sim[i] );
   }
  return size;
}  

int SimParser::read(DataCell* channels, uint numchannels) {

  for (uint i=0; i<numchannels; i++) {
     // revert the signs of the bias when the values come too close:
     if (channels[i]>(upp_lim[i]-incr[i]-bias[i]))  {
              bias[i] -= abs(bias[i]); 
              channels[i] += -((incr[i]*rand())/RAND_MAX); 
     }
     else if (channels[i]<(low_lim[i]+incr[i]+bias[i])) {
              bias[i] = abs(bias[i]); 
              channels[i] += ((incr[i]*rand())/RAND_MAX);
     } else
     channels[i] += (((2*incr[i]*rand())/RAND_MAX)-incr[i])+bias[i];
  }
  
  return numchannels;
}

int SimParser::read(DataCell* channels, uint* numchannels) {

  for (uint i=0; i<*numchannels; i++) {
     // invert the signs of the bias when the values come too close:
     if (channels[i]>(upp_lim[i]-incr[i]-bias[i])) bias[i] -= abs(bias[i]); 
     if (channels[i]<(upp_lim[i]+incr[i]+bias[i])) bias[i] =abs(bias[i]);
     channels[i] += (((2*incr[i]*rand())/RAND_MAX)-incr[i])+bias[i];
  }
  return *numchannels;
}

int SimParser::read(char *line) {
  if (size==0)
        create(5);
  // then serialise everything:
  char tempstr[10];  // temporary buffer for each channel
  line[0] = '\0';
  unsigned int i;
  int lower = 20;
  int upper = 230;
  int baserate = 4;
  float incr = 5;
  for (i=0; i<size; i++) {
     if (sim[i] < lower) bias[i] = baserate;
     if (sim[i] > upper) bias[i] = -baserate;
     sim[i] += (int)(((incr*rand())/(RAND_MAX)) - ((incr-bias[i])/2) );
     sprintf(tempstr,"%6i ",sim[i]);
     strcat(line,tempstr);
   }
  return strlen(line);
}

void SimParser::set_bias(signed int *newbias, uint numdatacells) {
  for (uint i=0; i<numdatacells; i++) bias[i] = newbias[i];
}  

void SimParser::set_bias(signed int newbias, uint datacell) {
  bias[datacell] = newbias;
}  

void SimParser::set_incr(float new_incr, uint datacell) {
  incr[datacell] = new_incr;
}
  
void SimParser::set_lims(float new_low_lim, float new_upp_lim, uint datacell) {
  low_lim[datacell] = new_low_lim;
  upp_lim[datacell] = new_upp_lim;
}  
