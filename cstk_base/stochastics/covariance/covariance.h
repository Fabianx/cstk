/***************************************************************************
                          covariance.h  -  v.0.1
                             -------------------
    begin                : Mo Oct 04 2004
    copyright            : (C) 2004 by Martin Berchtold
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

#ifndef COVARIANCE_H
#define COVARIANCE_H

#include "cstk_base/stochastics/stochastics.h"

class Covariance// : virtual public Stochastics 
{
	public:
		Covariance();
		~Covariance();
			
		DVectorMatrix matrix(DVectorList& veclist);
		DVector vector(DVector& vec) {return vec;}
		vei_t value(DMatrixList& matlist, DVector& I) {return 0;}
		
		DVector *mean;
	
	protected:	
		DVectorMatrix *matrix_cov;
		
	private: 
		oas_t cov(vei_t i, vei_t j, DVectorList& veclis);
};

#endif
