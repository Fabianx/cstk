/***************************************************************************
                           udp_rec_t.cpp  -  v.1.00
                           ------------------------
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

#include "sensordata/udpparser/udpparser.h"
#include <stdlib.h>   // exit(), atoi()
 
int main(int ac, char *argv[]) {

  if (ac<2) {
    printf("\n UDP Receiver Illustration - by Kristof Van Laerhoven.");
    printf("\n syntax:");
    printf("\n   %s <portnr>", argv[0]);
    printf("\n");
    printf("\n  <portnr> is the port number to listen to");
    printf("\n try for instance '%s 1501'.", argv[0]);
    printf("\n\n");
    exit(0);
  }

  SensorData *s;
  
  // this is the only place where we treat the sensordata
  // as a specific UDP parser object, elsewhere, we don't
  // care where the sensordata came from. Neat, isn't it? 
  int port = atoi(argv[1]);
  s = new UDPParser(port, 100);
  
  char buffer[1024];
  
  while(1) {
  
    if (s->read(buffer))
        printf("msg: \"%s\"\n\r", buffer);
  }
  
  delete s;
   
 return 0;
}
