/***************************************************************************
                           simparser_t.cpp  -  v.1.00
                           --------------------------
    begin                : Mon Nov 17 2003
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

#include "sensordata/simparser/simparser.h"
#include <stdlib.h>   // exit(), atoi()
 
int main(int ac, char *argv[]) {

  if (ac<2) {
    printf("\n Sensor Sim Illustration - by Kristof Van Laerhoven.");
    printf("\n syntax:");
    printf("\n   %s <nr>", argv[0]);
    printf("\n");
    printf("\n  <nr> is the number of sensors to simulate");
    printf("\n try for instance '%s 5'.", argv[0]);
    printf("\n\n");
    exit(0);
  }

  SensorData *s;
  
  // this is the only place where we treat the sensordata
  // as a specific sim parser object, elsewhere, we don't
  // care where the sensordata came from. Neat, isn't it? 
  int num = atoi(argv[1]);
  s = new SimParser(num);
  
  char buffer[1024];
  
  while(1) {
  
    if (s->read(buffer))
        printf("msg: \"%s\"\n\r", buffer);
  }
  
  delete s;
   
 return 0;
}
