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

#define UDPERR_CANNOTOPEN 0x01
#define UDPERR_CANNOTBIND 0x02
#define UDPERR_WRONGHOSTN 0x03
#define UDPERR_CANNOTSEND 0x11

#define MAX_MSG 1024

class UDPParser : virtual public SensorData {
  
public:
  UDPParser();
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
  void setPort(int port) {this->port = port;};
  int  getPort() {return port;};
  void setTimeOut(unsigned int to) {timeout = to;};
  unsigned int  getTimeOut() {return timeout;};
  
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
  int port;   // local server port
  struct sockaddr_in clientAddr, serverAddr; // client and server adresses
  struct sockaddr_in remoteServerAddr; // remote Server to send things to
  struct hostent *h;
  unsigned int timeout;
};
