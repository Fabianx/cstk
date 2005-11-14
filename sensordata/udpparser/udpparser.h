/***************************************************************************
                              udpparser.h  -  v.1.00
                              ----------------------
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

#ifndef UDPPARSER_H
#define UDPPARSER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */
#include "cstk_base/types.h"
#include "sensordata/sensordata.h"
#include "sensordata/bufferparser/bufferparser.h"

#define UDPERR_CANNOTOPEN -11
#define UDPERR_CANNOTBIND -12
#define UDPERR_WRONGHOSTN -13
#define UDPERR_CANNOTSEND -21
#define UDPERR_CANNOTREAD -30
#define UDPERR_INVTYPE    -40

#ifndef BIN_MODE
#define BIN_MODE 1
#endif
#ifndef ASC_MODE
#define ASC_MODE 2
#endif

#define MAX_MSG 1024

struct UDPParserSettings {
  UDPParserSettings() { 
	port=0;
	timeout=0;
	targethost[0]='\0';
	mode=0;
  }
  int port;
  unsigned int timeout;
  char  targethost[1024];        
  short mode;
};

class UDPParser : virtual public SensorData {

 public:
  UDPParser();
  UDPParser(UDPParserSettings udp_param);
  UDPParser(int port, unsigned int timeout);
  UDPParser(int port, unsigned int timeout, char* targethost);
  ~UDPParser();
  // read & filter values from the udp port, see SensorData 
  int read(char* channel_types, uint numchannels, 
           DataCell* columns,  uint* filter, uint numcolumns);
  // read all from the udp port, see SensorData 
  int read(DataCell* channels, uint numchannels);
  // read all from the udp port, see SensorData 
  int read(DataCell* channels, uint* numchannels);
  // read all from the udp port, see SensorData 
  int read(char *line);
  void setMsg(char *msg) {strcpy(this->msg,msg);};
  void setPort(int port) {this->udp_param.port = port;};
  int  getPort() {return this->udp_param.port;};
  void setTimeOut(unsigned int to) {this->udp_param.timeout = to;};
  unsigned int  getTimeOut() {return this->udp_param.timeout;};
  char createGet();
  int  getmsg();
  char createSend(char* targethost);
  char sendmsg();

 public:
  char  msg[MAX_MSG];
  char*  msgClient;
  unsigned int msgPort;
  
 private:
  int sd,sdd; // sockets
  struct sockaddr_in clientAddr, serverAddr; // client and server adresses
  struct sockaddr_in remoteServerAddr; // remote Server to send things to
  struct hostent *h;
  BufferParser bp;
  UDPParserSettings udp_param;
  int err;
};

#endif

