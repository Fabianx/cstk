/***************************************************************************
                          rs232parser.cpp  -  v.0.1
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

Rs232Parser::Rs232Parser() {
   buf = NULL;
   fd = 0;
   bufcounter = 0;
   err = 0;
}

Rs232Parser::Rs232Parser(int baud, int buff, char* poll, char* dev, short mod) 
{
   buf = NULL;
   fd = 0;
   bufcounter = 0;
   err = 0;
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
   bufcounter = 0;
   err = 0;
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
	uint i, j, k;
	// if rs232=closed, open it:
	if (fd==0) {
		err = open_rs232(rs232_param.device);
		if (err!=0) {
			return -err;
		}
	}
    
	if (rs232_param.poll_char[0]!=0) 
	{   
		res = write(fd,rs232_param.poll_char,1);
		if (res==-1) {
			err = RS232ERR_CANTWRITE;
			return -err;
		}
	}
	
	usleep(100);
	res = ::read(fd,buf,rs232_param.buff_size);  // read buffer
	
	buf[res]=0;   
	if (res==-1) {
		err = RS232ERR_CANTREAD;
		return -err;
	}
	else { 
		i=0;  // counter for the buffer array
		// if the data is ascii, and no poll is given, wait for a return '\r':
		while ((rs232_param.mode==ASC_MODE)&&(rs232_param.poll_char[0]=='\0')
			&&(buf[i++]!='\r')&&(i<(unsigned int)res)) {
		}
		// if the data is binary, and no poll is given, wait for a return 0xFF:
		while ((rs232_param.mode==BIN_MODE)&&(rs232_param.poll_char[0]=='\0')
			&&(buf[i++]!=0xFF)&&(i<(unsigned int)res)) {
		}
		if (i==(unsigned int)res) return 0;
     
		j=0;  // counter for the datacells
		k=0;  // temp counter
		while ((i<(unsigned int)res)&&(j<numchannels)) {  
			switch(channel_types[j]) {
				case U8B_TYPE: 
					for (k=0; k<numcolumns; k++) 
						if (filter[k]==j)  
							columns[k].set_u8val(buf[i]);
					i++;    
					break;
				case S8B_TYPE: 
					for (k=0; k<numcolumns; k++) 
						if (filter[k]==j)  
							columns[k].set_s8val(buf[i]);
					i++;    
					break;
				case U16B_TYPE:    
					for (k=0; k<numcolumns; k++) 
						if (filter[k]==j) 
							columns[k].set_u16val(buf[i],buf[i+1]);
					i+=2;
					break;
				case S16B_TYPE:
					for (k=0; k<numcolumns; k++) 
						if (filter[k]==j) 
							columns[k].set_s16val(buf[i],buf[i+1]);
					i+=2;
					break;
				case U32B_TYPE:    
					for (k=0; k<numcolumns; k++) 
						if (filter[k]==j) 
							columns[k].set_u32val(buf[i],buf[i+1],
												buf[i+2],buf[i+3]);
					i+=4;
					break;
				case S32B_TYPE:    
					for (k=0; k<numcolumns; k++) 
						if (filter[k]==j)
							columns[k].set_s32val(buf[i],buf[i+1],
												buf[i+2],buf[i+3]);
					i+=4;
					break;
#ifdef U64
				case U64B_TYPE:
					for (k=0; k<numcolumns; k++)
						if (filter[k]==j)
							columns[k].set_u64val(buf[i],buf[i+1],
									buf[i+2],buf[i+3],buf[i+4],
									buf[i+5],buf[i+6],buf[i+7]);
					i+=8;
					break;
#endif
#ifdef S64
				case S64B_TYPE:
					for (k=0; k<numcolumns; k++)
						if (filter[k]==j)
							columns[k].set_s64val(buf[i],buf[i+1],
									buf[i+2],buf[i+3],buf[i+4],
									buf[i+5],buf[i+6],buf[i+7]);
					i+=8;
					break;
#endif
				default: ; // errormsg? 
					break;    
			}//switch
     
			j++;
		}//while
  	} //if..else 
	return res+1;
}

int Rs232Parser::read(DataCell* channels, uint numchannels) 
{
   return 0;
}

int Rs232Parser::read(DataCell* channels, uint* numchannels) {
   // stub
   return 0;
}

int Rs232Parser::read(char *line) {
   int res=-2;
   
   // if rs232=closed, open it:
   if (fd==0) {
        err = open_rs232(rs232_param.device);
		if (err!=0)
         return err;
   }
   
	if (rs232_param.poll_char[0]!='\0') 
	{
		res = write(fd,rs232_param.poll_char,1);
		if (res==-1) {
			return RS232ERR_CANTWRITE;
		}
	}
	usleep(100); // wait a bit for the buffer to fill up
 
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

int Rs232Parser::open_rs232(char* devicename) {
  
  if (fd!=0) {  // close previous connection if there is one
      close_rs232();
  }

  // reset read buffer:
  if (buf!=NULL)
      delete[] buf;
  buf = new unsigned char [rs232_param.buff_size];

  // open the device to be non-blocking (read will return immediatly)
   fd = open(devicename, O_RDWR | O_NONBLOCK | O_NOCTTY); //
	if (fd ==-1) {     // error:
		return RS232ERR_CANTOPEN;
	}
	else 
	{ // ???
	  // if( fcntl(fd, F_SETFL, 0) ==-1 ) return -1;
	  // set parameters: 
       tcgetattr(fd,&oldtio); // save current port settings
       tcgetattr(fd,&newtio); // fill with old port settings
	   cfmakeraw(&newtio);
	   //newtio.c_cflag = rs232_param.baudrate | CS8 | CLOCAL | CREAD;
       newtio.c_cflag |= CS8 | CLOCAL | CREAD;
       cfsetspeed(&newtio,rs232_param.baudrate);
	   newtio.c_iflag = IGNPAR ;
       //newtio.c_oflag = 0;
       //newtio.c_lflag = 0;
	   newtio.c_cc[VMIN] = 1;
	   newtio.c_cc[VTIME] = 10;
       if (tcflush(fd, TCIFLUSH)!=0) return RS232ERR_CANTFLUSH;
       if (tcsetattr(fd,TCSANOW,&newtio)!=0) return RS232ERR_CANTSET;
	}

	return 0; // no error
}

void Rs232Parser::close_rs232() {

   if (buf!=NULL)
       delete[] buf;
   if (fd!=0) // close the serial port:
   {
       close(fd);
       tcsetattr(fd,TCSANOW,&oldtio);   // restore old port settings
   }
}


