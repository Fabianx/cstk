/***************************************************************************
                          mgv.cpp  -  v.0.1
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
 
 #include "mvg.h"
 
MVG::MVG()
{
}
 
MVG::~MVG()
{
}

vei_t MVG::value(DMatrixList& matlist, DVector& I)
{
	DMatrixList* current;
	DMatrixList* selected;
	
	f_64b tmp1, tmp2=(f_64b_max*2);
	DVector result;

	current = &matlist;

	do 
	{
		result = I - (*((*current).mue));
		tmp1 = ((result * (*current).matrix->T()) * result) + log((*current).matrix->det());
		printf("   dist=%7.3e, class=%i\n",tmp1,(*current).classinf);
		if (tmp1 < tmp2)
		{
			tmp2 = tmp1;
			selected = current;
		}	
		current = (*current).next;
	} 
	while (current != NULL);

	return (*selected).classinf;
}