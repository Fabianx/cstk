/***************************************************************************
                            udp_snd_t.cpp  -  v1.00
                            -----------------------
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
#include <stdlib.h>  // exit(), atoi()
#include <string.h>  // strlen()
 
int main(int ac, char *argv[]) {

  if (ac<4) {
    printf("\n UDP Sender Illustration - by Kristof Van Laerhoven.");
    printf("\n syntax:");
    printf("\n   %s <target host> <portnr> <data>", argv[0]);
    printf("\n");
    printf("\n  <target host> is the target IP number or hostname");
    printf("\n  <portnr> is the port number to send the data to");
    printf("\n  <data> is a textstring that needs to be sent");
    printf("\n try for instance '%s 1501 localhost testing..'.", argv[0]);
    printf("\n\n");
    exit(0);
  }

  int port = atoi(argv[2]);

  UDPParser *udpp = new UDPParser(port,100, argv[1]);

  udpp->setMsg(argv[3]);
  char i = udpp->sendmsg();
  
  if (!i)
    printf("Successfully sent '%s' to %s.\n", udpp->msg, argv[2]);
  else 
    printf("Got error %i when sending message '%s' via port %s.\n", 
           i, udpp->msg, argv[2]);

  delete udpp;
       
 return 0;
 
}
