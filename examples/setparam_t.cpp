/***************************************************************************
                         setparam_t.cpp  -  v.1.00
                         -------------------------
    begin                : Oct 21 2004
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
#include "cstk_base/vector/kvector.h"

int main(int ac, char *argv[]) {
	
	if (ac<2) {
		printf("\n");
		printf("  This demo shows how sensordata settings can be\n");
		printf("  read and generated with the setparse classes.\n");
		printf("\n   syntax:");
		printf("\n     %s <xsd>", argv[0]);
		printf("\n");
		printf("\n    <xsd> is an XML Settings Document.\n");
		printf("\n\n");
		exit(0);
	}

	char buff[0x10000];
	KProf kp;
	
	FILE* fp = fopen(argv[1],"r");	
	if (fp==NULL) {
		printf("Error opening file %s.\n\r", argv[1]);
		return 0;
	}
	
	kp.parse(fp);
	
	kp.export_dtd(buff);
	printf("--- DTD: -------------------------------------\n%s\n",buff);
	
	kp.export_xsd(buff);
	printf("--- XSD: -------------------------------------\n%s\n",buff);
	
	kp.setup_sensordata_parser();
	kp.setup_inputchannels();
	kp.setup_inputcolumns();

	if (kp.error()) { kp.export_err(buff); printf("%s",buff); exit(-1);}

	printf("--- raw output strings: ----------------------\n");

		// wait a sec or 2 for sensordata to get ready
		kp.read_buffer(buff);
		usleep(2000000);
		// and get a few output strings
		for (int t=0; t<20; t++) {
			int ret = kp.read_buffer(buff);
			if (ret>=0) 
				printf("%5i:\t'%s'\n\r",t,buff);
			else 
				printf("error(%i)\n\r",ret);
		}

	printf("\n--- interpreted output: ----------------------\n");

		KVector vect[kp.num_icols];
		for (unsigned int i=0; i<kp.num_icols; i++) 
			vect[i].createVector(100);		
		for (int t=0; t<20; t++) {
			int ret = kp.read_icols();
		 	if ( ret <= 0 ) {
		 		printf("Error: %i\n",ret);
			} 
			else
			{
				printf("%5i: K[",t);
				for (int i=0; i<ret; i++) {
					vect[i].add_comp(kp.icols[i].get_u8b());
					printf(" %6i",(unsigned char)vect[i].val());
				}
				printf("] (%i)\n",ret);
			}
		}

	return 0; // no error
}
