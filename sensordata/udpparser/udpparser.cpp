/***************************************************************************
                             udpparser.cpp  -  v.1.00
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
               
#include "udpparser.h"
               
UDPParser::UDPParser() {
}

UDPParser::UDPParser(int port, unsigned int timeout) {
  this->port = port;   
  this->timeout = timeout;
  createGet();
}

UDPParser::UDPParser(int port, unsigned int timeout, char* targethost) {
  this->port = port;   
  this->timeout = timeout;
  createSend(targethost);
}

UDPParser::~UDPParser() {
}                  

char UDPParser::createGet() {
  int rc;
 
  // socket creation
  sd=socket(AF_INET, SOCK_DGRAM, 0);
  if(sd<0) {
    return (UDPERR_CANNOTOPEN);
  }

  // bind local server port
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddr.sin_port = htons(port);
  rc = bind (sd, (struct sockaddr *) &serverAddr,sizeof(serverAddr));
  if(rc<0) {
    return (UDPERR_CANNOTBIND);
  }

  return 0;
}
          
int UDPParser::getmsg() 
{
    int n;
    socklen_t clientLen; // length of client's addr.
    
    // init buffer 
    memset(msg,0x0,MAX_MSG);
    // receive message 
    clientLen = sizeof(clientAddr);
    n = recvfrom(sd, msg, MAX_MSG, timeout, 
                 (struct sockaddr *) &clientAddr, &clientLen);
    msgClient = inet_ntoa(clientAddr.sin_addr);
    msgPort = ntohs(clientAddr.sin_port);
    if(n<0) {
      return 0;
    } else {
       return n;
    }    
}

char UDPParser::createSend(char* targethost) 
{
  int rc;
  
  // get server IP address (no check if IP address or DNS name 
  h = gethostbyname(targethost);
  if(h==NULL) {
    return UDPERR_WRONGHOSTN;
  }

  remoteServerAddr.sin_family = h->h_addrtype;
  memcpy((char *) &remoteServerAddr.sin_addr.s_addr,
         h->h_addr_list[0], h->h_length);
  remoteServerAddr.sin_port = htons(port);

  // socket creation
  sdd = socket(AF_INET,SOCK_DGRAM,0);
  if(sdd<0) {
    return UDPERR_CANNOTOPEN;
  }

  // bind any port
  clientAddr.sin_family = AF_INET;
  clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  clientAddr.sin_port = htons(0);

  rc = bind(sdd, (struct sockaddr *) &clientAddr, sizeof(clientAddr));
  if(rc<0) {
    return UDPERR_CANNOTBIND;
  }
  
  return 0;
}
  
char UDPParser::sendmsg() 
{
  int rc;
  
  rc = sendto(sdd, msg, strlen(msg), 0,
              (struct sockaddr *) &remoteServerAddr, 
              sizeof(remoteServerAddr));

  if(rc<0) //can't send data
      return UDPERR_CANNOTSEND;

  return 0;
}

int UDPParser::read(char* channel_types, uint numchannels, 
                    DataCell* columns,  uint* filter, uint numcolumns)
{
  
  return 0;
}

int UDPParser::read(DataCell* channels, uint numchannels)
{
  return 0;
}

int UDPParser::read(DataCell* channels, uint* numchannels)
{
  return 0;
}

int UDPParser::read(char *line)
{
  int res = getmsg();
  int i=0; 
  if (res>0) {
    while ((msgClient[i]!='\0')&&(i<MAX_MSG))
    {
          line[i] = msgClient[i];
          i++;
    }
    line[i] = '\t'; i++;
    char portstr[6];
    sprintf(portstr, "%i\t", msgPort);
    int j=0;
    while ((portstr[j]!='\0')&&(i<MAX_MSG))
    {
          line[i] = portstr[j];
          i++;j++;
    }
    j=0;
    while ((msg[j]!='\0')&&(i<MAX_MSG)&&(j<res))
    {
          line[i] = msg[j];
          i++;j++;
    }
    line[i] = '\0';
  }
  return i;
}

