/***************************************************************************
                              simparser.h  -  v.1.0
                              ---------------------
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

#ifndef SIMPARSER_H
#define SIMPARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include "cstk_base/types.h"
#include "sensordata/sensordata.h"

#define MAX_SEP 50 // max number of seperators in a sequence

/**Sensor Simulation Parser
  *@author Kristof Van Laerhoven
  
  A single pseudo random trace is generated, using these parameters:
  
  - bias: this is whether the trace is going up or down, and by how much
  - incr: this number indicates how different the next value can be,
    compared to the previous one. 
  
  The bias acts basically as an offset to the random value that is being
  generated and rescaled using the incr parameter.  
   
  Say our previous value was 42, the incr is 10 and the bias is -5; A 
  random number is generated between -10 and 10, then 5 is substracted 
  and we add (or substract if it is negative) it to 42 to get our next 
  value.
  
  To alter the bias, an upper and lower limit are imposed so that the
  values will 'bounce' back when it hits the floor or ceiling.  
   
  */

class SimParser : virtual public SensorData {
  
public:
  SimParser();
  SimParser(uint num);
  ~SimParser();
  void create(uint num);
  
  int read(char* channel_types, uint numchannels, 
           DataCell* columns,  uint* filter, uint numcolumns);  
  int read(DataCell* channels, uint numchannels);
  int read(DataCell* channels, uint* numchannels);
  int read(char *line);

  void       set_bias(signed int *newbias, uint numdatacells);  
  void       set_bias(signed int newbias, uint datacell);  
  signed int get_bias(uint datacell) {return bias[datacell];}   
  void       set_incr(float new_incr, uint datacell);
  float      get_incr(uint datacell) {return incr[datacell];}    
  void       set_lims(float new_low_lim, float new_upp_lim, uint datacell);
  float      get_low_lim(uint datacell) {return low_lim[datacell];}
  float      get_upp_lim(uint datacell) {return upp_lim[datacell];}
  
public:
  signed int *sim;  

private:
  signed int *bias;
  float *incr;
  float *low_lim;
  float *upp_lim;
  uint size;

}; 

#endif

