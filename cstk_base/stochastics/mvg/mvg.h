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

class MVG// : virtual public Stochastics 
{
	public:
		MVG();
		~MVG();
			
		DVectorMatrix matrix(DVectorList& veclist) {DVectorMatrix mat; return mat;}
		DVector vector(DVector& vec) {return vec;}
		vei_t value(DMatrixList& matlist, DVector& I);
	
	protected:	
		
	private: 
};

#endif