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
	err = 0;
}

UDPParser::UDPParser(int port, unsigned int timeout) {
	this->udp_param.port = port;   
	this->udp_param.timeout = timeout;
	createGet();
	err=0;
}

UDPParser::UDPParser(int port, unsigned int timeout, char* targethost) {
	this->udp_param.port = port;   
	this->udp_param.timeout = timeout;
	int i=0; while(targethost[i]!='\0'){ 
		this->udp_param.targethost[i]=targethost[i];i++;}
	createSend(targethost);
	err=0;
}

UDPParser::UDPParser(UDPParserSettings udp_param) {
	this->udp_param = udp_param; // bitwise copy of all
	if (udp_param.targethost[0]!='\0')
		createSend(udp_param.targethost);
	else    createGet();
	err=0;
}

UDPParser::~UDPParser() {
}                  

char UDPParser::createGet() {
	int rc;
	// socket creation
	sd=socket(AF_INET, SOCK_DGRAM, 0);
	if(sd<0) {
		err = UDPERR_CANNOTOPEN;
		return err;
	}
	// bind local server port
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(this->udp_param.port);
	rc = bind (sd, (struct sockaddr *) &serverAddr,sizeof(serverAddr));
	if(rc<0) {
		err = UDPERR_CANNOTBIND;
		return err;
	}
	return 0;
}
          
int UDPParser::getmsg() 
{
	int n;
	socklen_t clientLen; // length of client's addr.    
	memset(msg,0x0,MAX_MSG); // init buffer
	// receive message 
	clientLen = sizeof(clientAddr);
	n = recvfrom(sd, msg, MAX_MSG, this->udp_param.timeout, 
                 (struct sockaddr *) &clientAddr, &clientLen);
	msgClient = inet_ntoa(clientAddr.sin_addr);
	msgPort = ntohs(clientAddr.sin_port);
	if(n<0) {
		return 0;
	} 
	return n;
}

char UDPParser::createSend(char* targethost) 
{
	int rc;  
	// get server IP address (no check if IP address or DNS name 
	h = gethostbyname(targethost);
	if(h==NULL) {
		err = UDPERR_WRONGHOSTN;
		return err;
	}
	remoteServerAddr.sin_family = h->h_addrtype;
	memcpy((char *) &remoteServerAddr.sin_addr.s_addr,
			h->h_addr_list[0], h->h_length);
	remoteServerAddr.sin_port = htons(this->udp_param.port);
	// socket creation
	sdd = socket(AF_INET,SOCK_DGRAM,0);
	if(sdd<0) {
		err = UDPERR_CANNOTOPEN;
		return err;
	}
	// bind any port
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clientAddr.sin_port = htons(0);
	rc = bind(sdd, (struct sockaddr *) &clientAddr, sizeof(clientAddr));
	if(rc<0) {
		err = UDPERR_CANNOTBIND;
		return err;
	}
	return 0;
}
  
char UDPParser::sendmsg() 
{
	int rc;
	rc = sendto(sdd, msg, strlen(msg), 0,
		(struct sockaddr *) &remoteServerAddr, 
		sizeof(remoteServerAddr));
	if(rc<0) {
		err = UDPERR_CANNOTSEND;
		return err;
	}
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

