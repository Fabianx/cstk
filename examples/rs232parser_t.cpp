/***************************************************************************
                         rs232parser_t.cpp  -  v.1.00
                         ----------------------------
    begin                : Mar 05 2004
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

#define COLS  7
#define CHANS 8
  
void smartit(void) { 
 
  int i=0, j=0;
  Rs232Parser prs; 
  char poll[16];
  poll[0] = 'G'; poll[1] = 0;
  
  prs.set_baudrate(B115200);
  prs.set_buff_size(1024);
  prs.set_poll(poll);
  prs.set_device("/dev/ttyS1");
  
  char channel_types[CHANS];  
  DataCell channels[CHANS];
  DataCell columns[COLS];
  uint select[COLS];
  
  // read 8 unsigned bits from the serial line, and store the last seven
  // in this order:
  select[0] = 1;  select[1] = 2;  select[2] = 3;  select[3] = 4;
  select[4] = 5;  select[5] = 6;  select[6] = 7;  
  for (i=0; i<CHANS; i++) channel_types[i] = U8B_TYPE;
  for (i=0; i<COLS; i++) { 
     columns[i].set_type(U8B_TYPE);
  }                    
  // read 100 samples:
  for (i=0; i<100; i++) 
  {
    prs.read(channel_types,CHANS, columns, select, COLS);
    
    for (j=0; j<COLS; j++) printf("%i ",columns[j].get_u8b());
    printf(".\n\r");
  }
  // read 100 times all channels:
  for (i=0; i<CHANS; i++) channels[i].set_type(U8B_TYPE);
  for (i=0; i<100; i++) 
  {
    prs.read(channels,CHANS);
    
    for (j=0; j<CHANS; j++) printf("%i ",channels[j].get_u8b());
    printf(".\n\r");
  }    
  
}

void xbow(void) {
 
  int i=0, j=0;
    
  SensorData* pr=NULL;
  pr = new Rs232Parser(B38400,1024,"G","/dev/ttyS0");

  DataCell channels[2];
  for (i=0; i<2; i++) channels[i].set_type(U16B_TYPE);
  
  for (i=0; i<100; i++) 
  {
    pr->read(channels,2);
    for (j=0; j<2; j++) printf("%i ",channels[j].get_u16b());
    printf(".\n\r");
  }  
  
  if (pr != NULL) delete pr;
  
}
 
 
int main(void) {
  
    /*printf("testing serial parsing routines for a smart-it:\n\r");
    smartit();
    printf("\n\rdone.\n\r");     
    */
    
    printf("testing serial parsing routines for a xbow adxl202eb:\n\r");
    xbow();
    printf("\n\rdone.\n\r");     
    
    return 0;
}
 


