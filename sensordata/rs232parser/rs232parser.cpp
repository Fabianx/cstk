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
   poll_char = NULL;
   buff_size = 1024;
   buf = NULL;
   fd = 0;
   baudrate = B115200;
   device = NULL;
}

Rs232Parser::Rs232Parser(int baud, int buff, char* poll, char* dev) 
{
   fd = 0;
   buf = NULL;
   poll_char = NULL;
   device    = NULL;
   set_baudrate(baud);
   set_buff_size(buff);
   set_poll(poll);
   set_device(dev);
}

Rs232Parser::~Rs232Parser() { 
   close_rs232();
   if (device!=NULL) {
      delete []device;
   } 
   if (poll_char!=NULL) {
      delete []poll_char;
   }    
}
  
/* set/get poll character */
void Rs232Parser::set_poll(char* new_poll_char) {
   if (poll_char!=NULL) {
      delete []poll_char;
   }
   poll_char = new char[strlen(new_poll_char)];
   for (uint i=0; i<strlen(new_poll_char); i++)
       poll_char[i] = new_poll_char[i];
}

void Rs232Parser::get_poll(char* ret_poll_char) {
    for (uint i=0; i<strlen(poll_char); i++)
       ret_poll_char[i] = poll_char[i];
}

/* set/get device */
void Rs232Parser::set_device(char* new_device) {
    if (device!=NULL) {
      delete []device;
    }
    device = new char[strlen(new_device)+1];
    for (uint i=0; i<strlen(new_device)+1; i++)
       device[i] = new_device[i];
}

void Rs232Parser::get_device(char* ret_device) {
    for (uint i=0; i<strlen(device); i++)
       ret_device[i] = device[i];
}
  
/* read specific values from rs232 port, see SensorData class for more */
int Rs232Parser::read(char* channel_types, uint numchannels, 
                      DataCell* columns,  uint* filter, uint numcolumns) {
   int res=-2;
   uint i, j, k;
   // if rs232=closed, open it:
   if (fd==0) {
        res = open_rs232(device);
        if (res) {
           return 0;
        }
   }
    
   if (poll_char[0]!='\0') res = write(fd,poll_char,1);
   if (res==-1) { 
         return 0;
   }
   
   usleep(100);
   res = ::read(fd,buf,buff_size);  // read buffer
   
   if (res==-1) { 
         return 0;
   }
   else {     
     j=0;  // counter for the datacells
     i=0;  // counter for the buffer array
     k=0;  // temp counter
     while (j<(uint)res) { 
        switch(channel_types[j]) {
         case U8B_TYPE: 
	     for (k=0; k<numcolumns; k++) {
	         if (filter[k]==j)  
	              columns[k].set_u8val(buf[i]);
	     }    
             i++;    
	     break;
        case S8B_TYPE: 
	     for (k=0; k<numcolumns; k++) {
	         if (filter[k]==j)  
	              columns[k].set_s8val(buf[i]);
	     }    
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
	              columns[k].set_u32val(buf[i],buf[i+1],buf[i+2],buf[i+3]);
             i+=4;
	     break;
        case S32B_TYPE:    
	     for (k=0; k<numcolumns; k++) 
	         if (filter[k]==j)
	              columns[k].set_s32val(buf[i],buf[i+1],buf[i+2],buf[i+3]);
             i+=4;
	     break;
#ifdef U64
        case U64B_TYPE:
	     for (k=0; k<numcolumns; k++)
	         if (filter[k]==j)
	              columns[k].set_u64val(buf[i],buf[i+1],buf[i+2],buf[i+3],
		                            buf[i+4],buf[i+5],buf[i+6],buf[i+7]);
             i+=8;
	     break;
#endif
#ifdef S64
        case S64B_TYPE:
	     for (k=0; k<numcolumns; k++)
	         if (filter[k]==j)
	              columns[k].set_s64val(buf[i],buf[i+1],buf[i+2],buf[i+3],
		                            buf[i+4],buf[i+5],buf[i+6],buf[i+7]);
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

/* read 1 value from rs232 port, see SensorData class for more */
int Rs232Parser::read(DataCell* channels, uint numchannels) {

   int res=-2;
   uint i, j;
   
   // if rs232=closed, open it:
   if (fd==0) {
        if (!open_rs232(device))
	    return 0;
   }
 
   if (poll_char[0]!='\0') res = write(fd,poll_char,1);
   if (res==-1) {
         return 0;
   }
   usleep(100);
   
   res = ::read(fd,buf,buff_size);  // read buffer
   if (res==-1) {
         return 0;
   }
   else {
     
     j=0;  // counter for the datacells
     i=0;  // counter for the buffer array
     while (j<(uint)res) {
        
        if (channels[j].test_type(U8B_TYPE)) {
	     channels[j].set_u8val(buf[i]);
             i++;
        } else 
	if (channels[j].test_type(S8B_TYPE)) {
	     channels[j].set_s8val(buf[i]);
             i++;
        } else 
	if (channels[j].test_type(U16B_TYPE)) {
	     channels[j].set_u16val(buf[i], buf[i+1]);
	     i+=2;
        } else
	if (channels[j].test_type(S16B_TYPE)) {
	     channels[j].set_s16val(buf[i],buf[i+1]);
	     i+=2;
        } else
	if (channels[j].test_type(U32B_TYPE)) {
	     channels[j].set_u32val(buf[i],buf[i+1],buf[i+2],buf[i+3]);
             i+=4;
        } else
	if (channels[j].test_type(S32B_TYPE)) {
	     channels[j].set_s32val(buf[i],buf[i+1],buf[i+2],buf[i+3]);
             i+=4;
        }
#ifdef U64_TYPE
	  else
	if (channels[j].test_type(U64B_TYPE)) {    
	     channels[j].set_u64val(buf[i],buf[i+1],buf[i+2],buf[i+3],
		                    buf[i+4],buf[i+5],buf[i+6],buf[i+7]);
             i+=8;
        } 
#endif 
#ifdef S64_TYPE
	  else
	if (channels[j].test_type(S64B_TYPE)) {    
	     channels[j].set_s64val(buf[i],buf[i+1],buf[i+2],buf[i+3],
		                    buf[i+4],buf[i+5],buf[i+6],buf[i+7]);
             i+=8;
        }
#endif 
	else; //  errormsg?
	
	j++;
     }
   }
   
  return res+1;
}

/* read all from rs232 port, see SensorData class for more */
int Rs232Parser::read(DataCell* channels, uint* numchannels) {
   // stub
   return 0;
}

int Rs232Parser::read(char *line) {
   int res=-2;
   
   // if rs232=closed, open it:
   if (fd==0) {
        if (!open_rs232(device))
         return 0;
   }
 
   if (poll_char[0]!='\0') 
         res = write(fd,poll_char,1);
   if (res==-1) {
         return 0;
   }
   usleep(100);
   
   res = ::read(fd,buf,buff_size);  
   if (res==-1) {
         return 0;
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
  buf = new unsigned char [buff_size];

  // open the device to be non-blocking (read will return immediatly)
   fd = open(devicename, O_RDWR | O_NDELAY); //  | | O_NOCTTY
   if (fd <0) {     // error:
        return 1;
   }

  // set parameters: 
   if (fd >-1) {
       tcgetattr(fd,&oldtio); // save current port settings
       newtio.c_cflag = baudrate | CS8 | CLOCAL | CREAD;
       newtio.c_iflag = IGNPAR ;
       newtio.c_oflag = 0;
       newtio.c_lflag = 0;
       newtio.c_cc[VMIN]=0;
       newtio.c_cc[VTIME]=0;
       tcflush(fd, TCIFLUSH);
       tcsetattr(fd,TCSANOW,&newtio); 
   }
   else return 2;

  return 0; 
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


