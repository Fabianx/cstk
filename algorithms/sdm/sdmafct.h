/***************************************************************************
                              sdmafct.h  -  v.1.00
                             ----------------------
    begin                : Jun 01 2004
    copyright            : (C) 2002-2004 by Martin Berchtold
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

#ifndef SDMAFCT_H
#define SDMAFCT_H

#include "sdma.h" 
#define e 2.7182818284    
#define pi 3.1415926535   

/****************************************************************************************
*	Child class of SDMA that realizes several different store/retrieve methods	*
*	for different distribution functions. The initialization method and the		*
*	constructors are used form the parent class.					*
****************************************************************************************/
class SDMAfct : public SDMA
{
 public:
 	SDMAfct();
	SDMAfct(vei_t nsize, vei_t nasize, vei_t ndsize);
	~SDMAfct();
	
	// gauss distribution
	vei_t store_gauss(BinVector& v1, BinVector& b1, bool det_radius, 
	                  oas_t density);
	vei_t retrieve_gauss(BinVector& v1, BinVector& tsum, 
	                     BVector<oas_t>& tempsum, bool det_radius, 
	                     oas_t density);
			     
	// NAND distance measurement
	vei_t store_nand(BinVector& v1, BinVector& b1, bool det_radius);
	vei_t retrieve_nand(BinVector& v1, BinVector& tsum, 
	                    BVector<oas_t>& tempsum, bool det_radius);

 private:
	vei_t radius_nand(BinVector& v1);
};

#endif

