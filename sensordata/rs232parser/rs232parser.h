/***************************************************************************
                           rs232parser.h  -  v.0.2
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
#include "sensordata/bufferparser.h"

#ifndef BIN_MODE
#define BIN_MODE 1
#endif
#ifndef ASC_MODE
#define ASC_MODE 2
#endif

#define PAR_NO    0
#define PAR_ODD   1
#define PAR_EVEN  2
#define PAR_SPACE 3
#define PAR_MARK  4

#define RS232ERR_CANTOPEN  -110
#define RS232ERR_CANTWRITE -111
#define RS232ERR_CANTREAD  -112
#define RS232ERR_CANTFLUSH -113
#define RS232ERR_CANTSET   -114
#define RS232ERR_INVTYPE   -120   // invalid data type
#define RS232ERR_INVDATAB  -121   // invalid # of databits
#define RS232ERR_INVPAR    -122   // invalid parity type

/**Class for parsing rs232 output
  *@author Kristof Van Laerhoven
  */
  
struct Rs232ParserSettings {
  Rs232ParserSettings() { 
	device[0]='\0';
	baudrate=0;
	poll_char[0]='\0';
	poll_wait=0;
	buff_size=0;
	databits=0;
	stopbits=0;
	parity=0;
	mode=0;
  }
  char  device[256];              // the name of the rs232 device
  int   baudrate;              
  char  poll_char[256];           // what to send to ask for data
  unsigned int poll_wait;         // how long to wait after poll sent
  unsigned int buff_size;
  short databits;
  short stopbits;
  bool  parity;
  short mode;
};

class Rs232Parser : virtual public SensorData {

 public:
  Rs232Parser();
  Rs232Parser(int baud, int buff, char* poll, char* dev, short mod=BIN_MODE);
  Rs232Parser(Rs232ParserSettings rs232_param);
  ~Rs232Parser();

  int read(char* channel_types, uint numchannels, 
           DataCell* columns,  uint* filter, uint numcolumns);
  int read(DataCell* channels, uint numchannels);
  int read(DataCell* channels, uint* numchannels);
  int read(char *line);

  void  set_poll(char* poll_char);
  void  get_poll(char* poll_char);
  void  set_baudrate(int baudrate);
  int   get_baudrate();
  void  set_buff_size(int buff_size);
  int   get_buff_size();
  void  set_device(char* dev);
  void  get_device(char* dev);
  void  set_mode(short mode);
  short get_mode();
  void  set_rs232(Rs232ParserSettings rs232_param);
  void  get_rs232(Rs232ParserSettings& rs232_param);
  int   get_err(); // get last error identifier
  
 private:
	int open_rs232(char* devicename);
	void close_rs232();

	BufferParser bp;
	
	Rs232ParserSettings rs232_param;
	int fd;
	struct termios oldtio,newtio;
	char *buf;
	unsigned int bufcounter; // an iterator for buffers that weren't polled
	int err;
};

#endif
