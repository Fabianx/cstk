/***************************************************************************
                             kprof_t.cpp  -  v1.00
                             ---------------------
    begin                : Tue Mar 09 2004
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

#include "kprof/kprof.h"
#include <stdlib.h> // exit

int main(int ac, char **args) {

  KProf profile;
  if (ac<2) {
    printf("\n supply an XML Settings Document please\n\n");
    exit(1);
  } else {
    printf("\nParsing:\n\r");
  }
  int res = profile.parse(args[1],SETTP_RTP);
  if (res!=0) {
       printf("Error: Parse error on line %i.\n\r",res);
       exit(1);
  };      
    
  printf("\n\rINPUT: ");
  switch (profile.input_mode) {
    case IMODE_RS232: printf("rs232 (poll='%s')", profile.is.rs232->poll_char); 
                      printf("[port='%s', br=%i]", profile.is.rs232->device,
                      profile.is.rs232->baudrate);      
                      break;
    case IMODE_FILE:  printf("datafile (file='%s')", profile.is.filename); 
                      break; 
    case IMODE_PIPE:  printf("pipe"); break;
    case IMODE_UDP:   printf("udp (%i)", profile.is.port); break;
    case IMODE_SIM:   printf("sim"); break;
    case IMODE_INT:   printf("int"); break; 
    default: printf("?");
  }
  printf("\n\r");
  { // print out the input channels:
	if (!profile.is.firstch()) {
	printf("  IChannel List is empty!\n\r");
	} else {
		do {
			printf("  - channel id(%i), ", profile.is.get_id()); 
			printf("bits(%i), ", profile.is.get_bits());
			printf("%ssigned", (profile.is.get_sign())?" ":"un");
			printf("\n\r");
		} while (profile.is.nextch());
	}
  }
  printf("\n\r");  
  { // print out the input columns:
	if (!profile.is.firstcol()) {
	printf("  IColumn List is empty!\n\r ");
	} else {
		do {
			printf("  - column id(%i) ", profile.is.get_col_id()); 
			printf("channel(%i), ", profile.is.get_col_channel()); 
			printf("bits(%i), ", profile.is.get_col_bits());
			printf("%s", (profile.is.get_col_sign())?" ":"un");
			printf("signed\n\r");
		} while (profile.is.nextcol());
	}
  }  
  printf("\n\rOUTPUT: ");
  switch (profile.output_mode) {
    case OMODE_RS232: printf("rs232"); break;
    case OMODE_FILE:  printf("datafile (file='%s')", profile.os.filename); 
                      break;
    case OMODE_PIPE:  printf("pipe"); break;
    case OMODE_UDP:   printf("udp (%s,%i)", profile.os.host, profile.os.port); 
                      break;
    default: printf("?");
  }
  printf("\n\r");  
  { // print out the output channels:
	if (!profile.os.firstch()) {
	printf("  OChannel List is empty!\n\r ");
	} else {
		do {
			printf("  - channel id(%i), ", profile.os.get_id()); 
			printf("bits(%i), ", profile.os.get_bits());
			printf("%ssigned", (profile.os.get_sign())?" ":"un");
			printf(", src(%i), ", profile.os.get_src());
			printf("type(%s)\n\r", otypes[profile.os.get_type()] );
		} while (profile.os.nextch());
	}  
  }  
  { // print out the plots:
	printf("\n\rWINDOW: ");
	printf("(%i,%i,%i,%i), border=%i", profile.win.xpos, profile.win.ypos,
	       profile.win.width, profile.win.height, profile.win.border);
	printf(", framespeed:%i, skip:%i\n\r", profile.win.framespersec,
	       profile.win.skip);
	if (!profile.win.firstplot()) {
	printf("  Plot list is empty!\n\r ");
	} else {
		do {
			printf("  - plot slot:%i, ", profile.win.get_id());
			printf("%s, ", sptypes[profile.win.get_type()] ); 
			printf("src:%i, res.:%i", profile.win.get_src(), 
			       profile.win.get_res()); 
			printf(", %sscaled,", 
			       (profile.win.get_scale())?" ":"un" );
			printf(" color:%i\n\r", profile.win.get_colour() );
		} while (profile.win.nextplot());
	}
  }  
  printf("\n\rTOOL PARAMETERS: \n\r");
  
  printf("\n\rx:%i, y:%i, lr: %f, nbr: %f\n\r", profile.par.gridsize[0], 
            profile.par.gridsize[1], profile.par.lr, profile.par.nbr);  
    
}

