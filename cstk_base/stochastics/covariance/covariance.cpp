/***************************************************************************
                          covariance.cpp  -  v.0.1
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
 
 #include "covariance.h"
 
 Covariance::Covariance()
 {
 	matrix_cov = NULL;
	mean = NULL;
 }
 
 Covariance::~Covariance()
 {
 	if (matrix_cov!=NULL) delete matrix_cov;
	if (mean!=NULL) delete mean;
 }

 DVectorMatrix Covariance::matrix(DVectorList& veclist)
 {
 	vei_t dim = veclist.vector->get_dim();
	
	if (matrix_cov!=NULL) delete matrix_cov;
 	matrix_cov = new DVectorMatrix(dim, dim);
	
	if (mean!=NULL) delete mean;
	mean = new DVector(dim);
	
	for (vei_t i=0; i<(veclist.vector->get_dim()); i++)
     	{
		for (vei_t j=0; j<(veclist.vector->get_dim()); j++)
		{
			matrix_cov->vector[i].set_comp(cov(i,j,veclist),F64B_TYPE,j);
		}  
     	}
	return *matrix_cov;
 }
 
 oas_t Covariance::cov(vei_t i, vei_t j, DVectorList& veclis)
 {
 	oas_t cov = 0;
	vei_t count = 0;
	oas_t muej = 0, muei = 0;
	DVectorList* current;
	current = &veclis;
	do 
	{
		muei += (*current).vector->get_comp(i);
		muej += (*current).vector->get_comp(j);
		count++;
		current = (*current).next;
	} 
	while (current != NULL);
	muei /= count;
	muej /= count;
	mean->set_comp(muei, F64B_TYPE, i);
	mean->set_comp(muej, F64B_TYPE, j);
	current = &veclis;
	do 
	{
		cov += ((*current).vector->get_comp(i)-muei)*((*current).vector->get_comp(j)-muej);
		current = (*current).next;
	} 
	while (current != NULL);
	return (cov/count);
 }
