/***************************************************************************
                          mvg.h  -  v.0.1
                             -------------------
    begin                : Mo Oct 08 2004
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

#ifndef MVG_H
#define MVG_H

#include "cstk_base/stochastics/stochastics.h"

/****************************************************************************************
*	Simple class that performs a scan with a multi variate Gaussian model		*
*	on a covariance matrix list according to a input vector. The result of the	*
*	scan is a class information belonging to the 'winning' covariance matrix 	*
*	and mean vector tuple.								*
****************************************************************************************/

class MVG : virtual public Stochastics 
{
	public:
		MVG();
		~MVG();
		
		//empty return method since there is no matrix as result possible	
		DVectorMatrix matrix(DVectorList& veclist) {DVectorMatrix mat; return mat;}
		//empty return method since there is no vector as result possible
		DVector vector(DVector& vec) {return vec;}
		
		/****************************************************************************************
		*	The method that performs the scan returns a value containing the 		*
		*	class information of the hypothesis with the highest probability.		*
		****************************************************************************************/
		vei_t value(DMatrixList& matlist, DVector& I);
};

#endif