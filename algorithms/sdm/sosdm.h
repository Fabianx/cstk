/***************************************************************************
                              sosdm.h  -  v.1.00
                             --------------------
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

#ifndef SOSDM_H
#define SOSDM_H

#include "sdma.h" 
#define e 2.7182818284    
#define pi 3.1415926535   

class SOSDM : public SDMA
{
 public:
 	SOSDM();
	~SOSDM();
	
	void create_SO(vei_t thrs);
	vei_t store_SO(BinVector& v1, BinVector& b1);
	vei_t retrieve_SO(BinVector& v1, BinVector& tsum, 
	                  BVector<oas_t>& tempsum);
	void address_update();
	void del_datavec();
	void del_errovec();
	void destroy_errvec();
	
 private:
 	vei_t max_invdist(BinVector& v1);
	oas_t signal_thres(BinVector& v1, BinVector& v2, 
	                   vei_t maxdis);
	void error_calc(BinVector& v1, BinVector& v2, oas_t sigthr, 
	                vei_t cunter);  
	
 	BVector<oas_t> *errv; // all error vectors are vectors of counters
 	vei_t threshold;
	oas_t total_sig;
};

#endif // SOSDM_H

