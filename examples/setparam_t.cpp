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

	printf("raw output strings:\n");

		// wait a sec or 2 for sensordata to get ready
		usleep(200000);
		// and get a few output strings
		for (int t=0; t<20; t++) {
			int ret = kp.sd->read(buff);
			if (ret>0) {
				buff[ret]='\0';
				printf("%5i:\t'%s'\n\r",t,buff);
			}
		}

	printf("\ninterpreted output:\n");

		KVector vect[kp.num_icols];
		for (unsigned int i=0; i<kp.num_icols; i++) vect[i].createVector(100);
		for (int t=0; t<20; t++) {
			int ret = kp.sd->read(kp.chs, kp.num_chs, 
			                      kp.icols, kp.filter, kp.num_icols);
		 	if ( ret <= 0 ) {
		 		printf("Error: %i\n",ret);
			} 
			else
			{
				printf("%5i: K[",t);
				for (unsigned int i=0; i<kp.num_icols; i++) {
					vect[i].add_comp(kp.icols[i].get_u8b());
					printf(" %6i",vect[i].val());
				}
				printf("]\n");
			}
		}

	return 0; // no error
}
