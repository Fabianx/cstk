/***************************************************************************
                           stochastics.h  -  v.0.1
                           ----------------------
    begin                : Mon Oct 04 2004
    copyright            : (C) 2004 by Martin Berchtold, 
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

#ifndef STOCHASTICS_H
#define STOCHASTICS_H

#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h> 

#include "cstk_base/types.h"
#include "cstk_base/matrix/dvectormatrix.h"

/****************************************************************************************
*	Abstract base class for all stochastical analyses classes. The methods		*
*	are matrix, vector and value corresponding to the return values.		*
****************************************************************************************/
struct Stochastics{
	virtual ~Stochastics() {};
	
	virtual DVectorMatrix matrix(DVectorList& veclist)=0;
	virtual DVector vector(DVector& vec)=0;
	virtual vei_t value(DMatrixList& matlist, DVector& I)=0;
};

#endif

