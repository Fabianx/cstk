/***************************************************************************
                         annotate.cpp  -  v.1.00
                         ------------------------
    begin                : Dec 14 2004
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

#include "cstk_base/vector/kvector.h"
#include "kprof/kprof.h"
#include "misc/conio.h"
#include <sys/time.h>     // for timings

int main(int ac, char *argv[]) {
	
	if (ac<2) {
		printf("\n  This tool reads a CSTK XML file and\n");
		printf("  parses its input section. The user\n");
		printf("  then gets the option of annotating\n");
		printf("  the incoming data with a key as it\n");
		printf("  streams in.\n");
		printf("\n   syntax:");
		printf("\n     %s <xsd> [options]", argv[0]);
		printf("\n");
		printf("\n    <xsd> is a CSTK XML file.\n");
		printf("\n    valid options:");
		printf("\n     -i: add iterator in front");
		printf("\n     -t: add timestamp in front");
		printf("\n     -b: just print the buffer");
		printf("\n\n");
		exit(0);
	}
	
	bool iterator = false;
	bool timestamp = false;
	char mode = 1;
	
	for (int i=2; i<ac; i++) {
		if (ac>i) {
			if (strcasecmp(argv[i],"-i")==0) 
				iterator=true;
			else if (strcasecmp(argv[i],"-t")==0) 
				timestamp=true;
			else if (strcasecmp(argv[i],"-b")==0) 
				mode=0;
		}
	}
	
	ConIO con;
	
	char buff[0x10000];
	KProf kp;
	
	FILE* fp = fopen(argv[1],"r");	
	if (fp==NULL) {
		printf("Error opening file %s.\n\r", argv[1]);
		return 0;
	}
	
	kp.parse(fp);	
	if (kp.err!=0) {
		printf("Error parsing file %s on line %i (code=%i).\n\r", 
			argv[1], kp.errline, kp.err);
		return kp.err;
	}
	
	kp.export_dtd(buff);
	printf("DTD:\n%s\n",buff);
	
	kp.export_xsd(buff);
	printf("XSD:\n%s\n",buff);
	
	kp.setup_sensordata_parser();	
	if (kp.err!=0) {
		printf("Error opening sensor data in %s (code=%i).\n\r", 
			argv[1], kp.err);
		return kp.err;
	}
	
	{ // read and print
		KVector vect[kp.num_icols];
		for (unsigned int i=0; i<kp.num_icols; i++) 
			vect[i].createVector(100);
		long unsigned int t=0;
		char ch=0, cl='0';
		struct timeval tv;
		struct timezone tzv;
		while (ch!='q'&&ch!='Q') {
			int ret;
			if(mode==1) 
				ret = kp.sd->read(kp.chs, kp.num_chs, 
				                  kp.icols, kp.filter, kp.num_icols);
			else 
				ret = kp.sd->read(buff); 	   
			if ( ret < 0 ) {
		 		printf("Error(%i)\n",ret);
				return ret;
			} 
			else
			{
				if (iterator) printf("%6lu\t",t);
				if (timestamp) {
					gettimeofday(&tv, &tzv);
					printf("%li\t%li\t", (long int)tv.tv_sec, (long int)tv.tv_usec);
				}
				if(mode==1) {
					for (unsigned int i=0; i<kp.num_icols; i++) {
						vect[i].add_comp(kp.icols[i].get_u8b());
						printf("%6i\t",vect[i].val());
					}
				}
				else printf("%6s\t", buff);
				printf("%c\n",cl);
			}
			ch = con.kb_getc();
			if (ch!=0) cl = ch;
			t++;
		}
	}
	
	return 0; // no error
}

