/***************************************************************************
                         sensordata_t.cpp  -  v.1.00
                         ---------------------------
    begin                : Apr 05 2004
    copyright            : (C) 2004 by Kristof Van Laerhoven
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
#include "sensordata/logfileparser/logfileparser.h"
#include "sensordata/udpparser/udpparser.h"
#include "sensordata/simparser/simparser.h"

int main(int ac, char *argv[]) {

  if (ac<2) {
    printf("\n  This demo will show how sensordata gets abstracted \n");
    printf("  throughout most of the program, no matter where it \n");
    printf("  comes from (0:serial port, 1:simulation, 2:udp, and\n");
    printf("  3:file - see the sourcecode).\n\r");
    printf("\n   syntax:");
    printf("\n     %s <nr>", argv[0]);
    printf("\n");
    printf("\n    <nr> is the id associated with the input (0-3).\n");   
    printf("\n    try for instance '%s 2'.", argv[0]);
    printf("\n\n");
    exit(0);
  }
  
  SensorData *sd;
    
  char choice = atoi(argv[1]);
  switch (choice) 
  {
        case 0: sd = new Rs232Parser(B38400,1024,"G","/dev/ttyS0");  
                break;
        case 1: sd = new SimParser(2);
                break;
        case 2: sd = new UDPParser(2221,100);
                break;
        case 3: sd = new LogFileParser("./logfile.txt");   
                break; 
        default:sd = new SimParser(2);    
  }    
    
  char buffer[1024];      
  while (1)
  {
      if (sd->read(buffer))
        printf("msg: \"%s\"\n\r", buffer);
      else
        printf("msg: <nothing>\n\r");
  }
    
  if (sd != NULL) delete sd;
    
  return 0;
}

