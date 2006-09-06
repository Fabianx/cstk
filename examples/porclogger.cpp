/***************************************************************************
                           PorcLogger.cpp  -  v.1.00
                         ----------------------------
    begin                : Aug 09 2006
    copyright            : (C) 2006 by Kristof Van Laerhoven
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

#include "sensordata/rs232parser/rs232parser.h"
#include <stdio.h>  // strings

#define SBUFSIZE 30

int main(void) {
  
  // plot raw data 'R' from Kube:
  int i=0, j=0, FF_c=0, DA_c=-1;
  Rs232Parser pr(B2400,SBUFSIZE,"","/dev/ttyS0");
  char buffer[SBUFSIZE], c;
  bool quit = false;
  
  FILE* fp = NULL;
  
  {
	printf(" Available Commands:\n");
	printf("*********************\n");
	printf(" (u)pload binary\n");
	printf(" (w)ipe memory\n");
	printf("\n");
	printf(" Choose wisely: \n");
	scanf("%c", &c);
	
	pr.read(buffer); // send poll
	
	switch (c)
	{
		case 'u': 
			pr.set_poll("U");
			break;
		case 'w':
			pr.set_poll("w");
			break;
	}
	
	//pr.read(buffer); // send poll
	pr.read(buffer); // send poll
	pr.set_poll("");
	
	fp = fopen("log.txt","w");
	
	while (!quit)
	{
		// for (i=0; i<SBUFSIZE; i++)
		//	buffer[i] = 0;
		
		j = pr.read(buffer); 
		
		// flush all incoming data to screen and file:
		if (j>0)
		{
			for (i=0; i<j-1; i++)
			{
				if ((unsigned char) buffer[i] == 0xFF) {
					FF_c++; 
					printf("\n");
					fprintf(fp,"\n");
				} else {
					if (DA_c>=0) 
						DA_c++;
					FF_c = 0;
				}
				printf(" %02X", (unsigned char) buffer[i]);
				fprintf(fp," %02X", (unsigned char) buffer[i]);
			}
		}
		
		if (FF_c>10)
			quit = true;
  	}
	fclose(fp);
  } 
  
  return 0;
  
}
