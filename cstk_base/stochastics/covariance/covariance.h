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

/********************************************************************************
*	This class produces out of a DVectorList a covariance matrix.		*
********************************************************************************/

class Covariance : virtual public Stochastics 
{
	public:
		Covariance();
		~Covariance();
		
		/************************************************************************
		*	This is the main method that outputs as return value the 	*
		*	calculated covariace matrix. The input of the method is a	*
		* 	list of DVectors that represent a senor-arrays output over 	*
		*	the time.							*
		************************************************************************/
		DVectorMatrix matrix(DVectorList& veclist);
		
		//empty return method since there is no vector as result possible
		DVector vector(DVector& vec) {return vec;}
		//empty return method since there is no value as result possible
		vei_t value(DMatrixList& matlist, DVector& I) {return 0;}
		
		//over the time mean vector produced in calculations, needed for MVG
		DVector *mean;
	
	protected:	
		//pointer to the resulting covariance matrix
		DVectorMatrix *matrix_cov;
		
	private: 
		//calculation of the i'th row j'th column covariance matrix element
		oas_t cov(vei_t i, vei_t j, DVectorList& veclis);
};

#endif
