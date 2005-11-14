/***************************************************************************
                          rs232parser.cpp  -  v.0.2
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

#include "rs232parser.h"

/**Class for parsing rs232 output
  *@author Kristof Van Laerhoven
  */

Rs232Parser::Rs232Parser() 
{
   buf = NULL;
   tbuf = NULL;
   fd = 0;
   bufcounter = 0;
   err = 0;
   sep = false;
   tbuf_counter = 0;
}

Rs232Parser::Rs232Parser(int baud, int buff, char* poll, char* dev, short mod) 
{
   buf = NULL;
   tbuf = NULL;
   fd = 0;
   bufcounter = 0;
   err = 0;
   sep = false;
   tbuf_counter = 0;
   rs232_param.baudrate = baud;
   rs232_param.buff_size = buff;
   int i=-1;
   do { i++;rs232_param.poll_char[i] = poll[i];} while (poll[i]!='\0');
   i=-1; do { i++;rs232_param.device[i] = dev[i]; } while (dev[i]!='\0');
   rs232_param.mode = mod;
}

Rs232Parser::Rs232Parser(Rs232ParserSettings rs232_param) 
{
   fd = 0;
   buf = NULL;   
   tbuf = NULL;   
   bufcounter = 0;
   err = 0;
   sep = false;
   tbuf_counter = 0;
   this->rs232_param = rs232_param; // bitwise copy of all
}

Rs232Parser::~Rs232Parser() 
{ 
   close_rs232();
}
  
void Rs232Parser::set_poll(char* poll) 
{
	int i=-1;
	do {i++;rs232_param.poll_char[i]=poll[i];} while (poll[i]!='\0');
}

void Rs232Parser::get_poll(char* poll)
{
	int i=-1;
	do {i++;poll[i] = rs232_param.poll_char[i];} while (poll[i]!='\0');
}

void Rs232Parser::set_baudrate(int baudrate) 
{
	rs232_param.baudrate = baudrate;
}

int  Rs232Parser::get_baudrate() 
{
	return rs232_param.baudrate;
}
  
void Rs232Parser::set_buff_size(int buff_size) 
{
	rs232_param.buff_size = buff_size;
}

int Rs232Parser::get_buff_size() 
{
	return rs232_param.buff_size;
}
  
void Rs232Parser::set_device(char* dev) 
{
	int i=-1;
	do {i++;rs232_param.device[i]=dev[i];} while (dev[i]!='\0');
}

void Rs232Parser::get_device(char* dev) 
{
	int i=-1;
	do {i++;dev[i]=rs232_param.device[i];} while (dev[i]!='\0');
}

void Rs232Parser::set_mode(short mode) 
{
	rs232_param.mode = mode;
}

short Rs232Parser::get_mode()
{
	return rs232_param.mode;
}

void Rs232Parser::set_rs232(Rs232ParserSettings rs232_param)
{
	this->rs232_param = rs232_param;
}

void Rs232Parser::get_rs232(Rs232ParserSettings& rs232_param)
{
	rs232_param = this->rs232_param;
}
  
int Rs232Parser::get_err()
{
	return err;
}  

// read specific values from rs232 port, see SensorData class for more 
int Rs232Parser::read(char* channel_types, uint numchannels, 
                      DataCell* columns,  uint* filter, uint numcolumns) 
{
	int res=-2;
	uint i;
	// if rs232=closed, open it:
	if (fd==0) {
		err = open_rs232(rs232_param.device);
		if (err!=0) 
			return err;
	}
    
	if (rs232_param.poll_char[0]!=0) 
	{   
		res = write(fd,rs232_param.poll_char,1);
		if (res==-1) {
			err = RS232ERR_CANTWRITE;
			return err;
		}
	}
	
	usleep(rs232_param.poll_wait);
	res = ::read(fd,buf,rs232_param.buff_size);  // read buffer
	
	buf[res]=0;   
	if (res==-1) {
		err = RS232ERR_CANTREAD;
		return err;
	}
	else {
		if (rs232_param.mode==ASC_MODE) {
			i = bp.parse_asc( buf, channel_types,
			              numchannels, columns, filter, numcolumns);
		}
		else if (rs232_param.mode==BIN_MODE)
			i = bp.parse_bin( buf, res, channel_types, 
			              numchannels, columns, filter, numcolumns);
		if (i<0) {
			err = RS232ERR_INVTYPE;
			return err;
		}
	} //if..else 
	return numcolumns; // if no errors
}

int Rs232Parser::read(DataCell* channels, uint numchannels)
{
	// stub
	return 0;
}

int Rs232Parser::read(DataCell* channels, uint* numchannels)
{
	// stub
	return 0;
}

int Rs232Parser::read(char *line)
{
	int res=-2;
	// if rs232=closed, open it:
	if (fd==0) {
		err = open_rs232(rs232_param.device);
		if (err!=0)
			return err;
	}
   
	if (rs232_param.poll_char[0]!='\0') // is there a poll character/string?
	{
		res = write(fd,rs232_param.poll_char,1);
		if (res==-1) {
			return RS232ERR_CANTWRITE;
		}
	}
	usleep(rs232_param.poll_wait); 
	
	res = ::read(fd,buf,rs232_param.buff_size);  
	if (res==-1) {
		return RS232ERR_CANTREAD;
	}
	else {
		int i=0;  // counter for the buffer array
		while (i<res) {
			line[i] = buf[i]; 
			i++;
		}
	}
	return res+1;
}

int Rs232Parser::open_rs232(char* devicename)
{  
	if (fd!=0) {  // close previous connection if there is one
		close_rs232();
	}
	// reset read buffer:
	if (buf!=NULL)
		delete[] buf;
	char *test = NULL;
	test = new char[1200];
	buf = new char [rs232_param.buff_size];
	tbuf = new char [rs232_param.buff_size];
	// open the device to be non-blocking (read will return immediatly)
	fd = open(devicename, O_RDWR | O_NONBLOCK | O_NOCTTY); //
	if (fd ==-1) {     // error:
		err = RS232ERR_CANTOPEN;
	}
	else 	{ 
		err = 0;
		// set parameters: 
		tcgetattr(fd,&oldtio); // save current port settings
		tcgetattr(fd,&newtio); // fill with old port settings
		cfmakeraw(&newtio);
		switch(rs232_param.databits) {
			case 8:	newtio.c_cflag |= CS8 | CLOCAL | CREAD; break;
			case 7:	newtio.c_cflag |= CS7 | CLOCAL | CREAD; break;
			case 6:	newtio.c_cflag |= CS6 | CLOCAL | CREAD; break;
			case 5:	newtio.c_cflag |= CS5 | CLOCAL | CREAD; break;
			default:	err = RS232ERR_INVDATAB; break; 
		}
		cfsetspeed(&newtio,rs232_param.baudrate);
		switch(rs232_param.parity) {
			case PAR_NO:	newtio.c_iflag = IGNPAR; break;
			case PAR_ODD:	newtio.c_iflag = PARODD; break;
			case PAR_EVEN:	newtio.c_iflag = ~PARODD; break; //?
			case PAR_SPACE:	newtio.c_iflag = IGNPAR; break;  //?
			case PAR_MARK:	newtio.c_iflag = PARMRK; break;
			default:	err = RS232ERR_INVPAR; break;
		}		
		newtio.c_cc[VMIN] = 1;
		newtio.c_cc[VTIME] = 10;
		if (tcflush(fd, TCIFLUSH)!=0)
			err = RS232ERR_CANTFLUSH;
		if (tcsetattr(fd,TCSANOW,&newtio)!=0)
			err = RS232ERR_CANTSET;
	}
	return err; // relax, it's 0 if no error
}

void Rs232Parser::close_rs232() 
{
	if (buf!=NULL)
		delete[] buf;
	if (fd!=0) // close the serial port:
	{
		close(fd);
		tcsetattr(fd,TCSANOW,&oldtio);   // restore old port settings
	}
}


