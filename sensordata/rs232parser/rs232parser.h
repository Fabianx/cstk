/***************************************************************************
                           rs232parser.h  -  v.0.1
                             -------------------
    begin                : Sun Feb 29 2004
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

#ifndef RS232PARSER_H
#define RS232PARSER_H

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <sys/signal.h>
#include <sys/types.h>
#include "cstk_base/types.h"
#include "sensordata/sensordata.h"

/**Class for parsing rs232 output
  *@author Kristof Van Laerhoven
  */

class Rs232Parser : virtual public SensorData {

public:
  Rs232Parser();
  Rs232Parser(int baud, int buff, char* poll, char* dev);
  ~Rs232Parser();

  /* read values from rs232 port, see SensorData class for more info */
    int read(char* channel_types, uint numchannels, 
             DataCell* columns,  uint* filter, uint numcolumns);

  /* read all from rs232 port, see SensorData class for more info */
    int read(DataCell* channels, uint numchannels);

  /* read all from rs232 port, see SensorData class for more info */
    int read(DataCell* channels, uint* numchannels);

    int read(char *line);

  /* set/get poll character */
  void set_poll(char* new_poll_char);
  void get_poll(char* ret_poll_char);

  /* set/get baud rate */
  void set_baudrate(int new_baudrate) {baudrate = new_baudrate;};
  int get_baudrate() {return baudrate;};

  /* set/get buffer size */
  void set_buff_size(int new_buff_size) {buff_size = new_buff_size;};
  int get_buff_size() {return buff_size;};

  /* set/get device */
  void set_device(char* new_device);
  void get_device(char* ret_device);

private:

  int open_rs232(char* devicename);
  void close_rs232();

private:

  char* poll_char;
  int fd;
  int baudrate;
  unsigned int buff_size;
  char* device;
  
  struct termios oldtio,newtio;

  unsigned char *buf;
  unsigned int bufcounter; // an iterator for buffers that weren't polled
}; 

#endif

