/***************************************************************************
                             ksom.cpp  -  v.1.00
                             -------------------
    begin                : Tue Nov 25 2003
    copyright            : (C) 2003 by Kristof Van Laerhoven
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

#include "ksom.h"

#define _to3(x,y,l) (x)+((y)*par->max_x)+((l)*par->max_x*par->max_y)
#define _to2(x,y) (((x)*par->max_y)+y)

KSOM::KSOM()
{
    	vect = NULL;
    	win_xy = NULL;
    	max_xy = NULL;
}


KSOM::KSOM(KSOMSettings *parameters)
{
	par = parameters;
	win_xy = new KVector(2);
	max_xy = new KVector(2);
	max_xy->add_comp(par->max_x,0);
	max_xy->add_comp(par->max_y,1);
	vect = new DVector[_to2(par->max_x-1, par->max_y)];
	for (vei_t i=0; i<(_to2(par->max_x-1, par->max_y)); i++)
		vect[i].create(par->vecdim);
}

KSOM::~KSOM()
{
    	if (vect!=NULL) delete []vect;
    	delete win_xy;
    	delete max_xy;
}

void KSOM::create(KSOMSettings *parameters)
{
	par = parameters;
	win_xy = new KVector(2);
    	max_xy = new KVector(2);
    	max_xy->add_comp(par->max_x,0);
	max_xy->add_comp(par->max_y,1);
	vect = new DVector[_to2(par->max_x-1,par->max_y)];
	for (vei_t i=0; i<(_to2(par->max_x-1,par->max_y)); i++)
		vect[i].create(par->vecdim);
}

void KSOM::savetoFile()
{
	FILE *f = fopen("grid.sav","w");
	for (vei_t i=0; i<(_to2(par->max_x-1,par->max_y)); i++)
		for (vei_t j=0; j<vect[i].get_dim(); j++)
			fprintf(f,"(%i,%i,%i)%f ",i,j,vect[i].get_type(j),vect[i].get_comp(j));
	fclose(f);
}

int KSOM::restorefromFile()
{
	FILE *f = fopen("grid.sav","r");
	if (!ferror(f))
	{
		double out;	
		int i,j,t;
		while (!feof(f))
		{
			fscanf(f,"(%i,%i,%i)%lf ",&i,&j,&t,&out);
			vect[i].set_comp(out,t,j);
		}
		fclose(f);
	}
	return ferror(f);
}

void KSOM::initRandom(DVector& prototyp)
{
for (vei_t x=0; x<max_xy->pvect[0]; x++) 
{
      	for (vei_t y=0; y<max_xy->pvect[1]; y++) 
      	{
         	for (vei_t n=0; n<prototyp.get_dim(); n++) 
		{
			switch (prototyp.get_type(n))
			{
				case U8B_TYPE:  vect[_to2(x,y)].set_comp( ((u_8b)rand()), U8B_TYPE, n);   // random numbers
						break;
				case U16B_TYPE: vect[_to2(x,y)].set_comp( ((u_8b)rand()), U16B_TYPE, n ); // small random numbers!
						break;
				case U32B_TYPE: vect[_to2(x,y)].set_comp(((u_32b_max)*((double)(rand())/(double)(RAND_MAX+1))),U32B_TYPE,n); 
						break;
				/*case U64B_TYPE: vect[_to2(x,y)].set_comp(((u_64b_max)*((double)(rand())/(double)(RAND_MAX+1))),U64B_TYPE,n); 
						break;*/
				case S8B_TYPE:  if (floor(2.0*rand()/(RAND_MAX+1.0))==1)
							vect[_to2(x,y)].set_comp(((s_8b_max)*((double)(rand())/(double)(RAND_MAX+1))),  S8B_TYPE,n); 
						else
							vect[_to2(x,y)].set_comp(((s_8b_min)*((double)(rand())/(double)(RAND_MAX+1))),  S8B_TYPE,n); 
						break;
				case S16B_TYPE: if (floor(2.0*rand()/(RAND_MAX+1.0))==1)
							vect[_to2(x,y)].set_comp(((s_16b_max)*((double)(rand())/(double)(RAND_MAX+1))),  S16B_TYPE,n); 
						else
							vect[_to2(x,y)].set_comp(((s_16b_min)*((double)(rand())/(double)(RAND_MAX+1))),  S16B_TYPE,n); 
						break;
				case S32B_TYPE: if (floor(2.0*rand()/(RAND_MAX+1.0))==1)
							vect[_to2(x,y)].set_comp(((s_32b_max)*((double)(rand())/(double)(RAND_MAX+1))),  S32B_TYPE,n); 
						else
							vect[_to2(x,y)].set_comp(((s_32b_min)*((double)(rand())/(double)(RAND_MAX+1))),  S32B_TYPE,n); 
						break;
				/*case S64B_TYPE: if (floor(2.0*rand()/(RAND_MAX+1.0))==1)
							vect[_to2(x,y)].set_comp(((s_64b_max)*((double)(rand())/(double)(RAND_MAX+1))),  S64B_TYPE,n); 
						else
							vect[_to2(x,y)].set_comp(((s_64b_min)*((double)(rand())/(double)(RAND_MAX+1))),  S64B_TYPE,n); 
						break;*/
				case F32B_TYPE: if (floor(2.0*rand()/(RAND_MAX+1.0))==1)
							vect[_to2(x,y)].set_comp(((f_32b_max)*((double)(rand())/(double)(RAND_MAX+1))),  F32B_TYPE,n); 
						else
							vect[_to2(x,y)].set_comp(((f_32b_min)*((double)(rand())/(double)(RAND_MAX+1))),  F32B_TYPE,n);  
						break;
				case F64B_TYPE: if (floor(2.0*rand()/(RAND_MAX+1.0))==1)
							vect[_to2(x,y)].set_comp(((f_64b_max)*((double)(rand())/(double)(RAND_MAX+1))),  F64B_TYPE,n); 
						else
							vect[_to2(x,y)].set_comp(((f_64b_min)*((double)(rand())/(double)(RAND_MAX+1))),  F64B_TYPE,n);  
						break;
			}
        	}           
	}
}
}

oas_t KSOM::det_dis(DVector& vec1, DVector& vec2)
{
	switch (par->dist)
	{
		case DIS_MANH:	return vec1.dis_manh(vec2);
		case DIS_CHEB:	return vec1.dis_cheb(vec2);
		case DIS_EUCL:	return vec1.dis_eucl(vec2);
		case DIS_MINK:	return vec1.dis_mink(vec2, par->minkexp);
	}
	return vec1.dis_manh(vec1);
}

oas_t KSOM::det_dis(KVector& vec1, KVector& vec2)
{
	switch (par->dist)
	{
		case DIS_MANH:	return vec1.dis_manh(vec2);
		case DIS_CHEB:	return vec1.dis_cheb(vec2);
		case DIS_EUCL:	return vec1.dis_eucl(vec2);
		case DIS_MINK:	return vec1.dis_mink(vec2, par->minkexp);
	}
	return vec1.dis_manh(vec1);
}

void KSOM::feed(DVector& vec, float lr) 
{
	KVector xy(2);
    	oas_t dist, mindist = (2.0 * DBL_MAX);
    	float nb;    
	// find winner coords
    	for (vei_t x=0; x<max_xy->pvect[0]; x++) 
	{
      		for (vei_t y=0; y<(max_xy->pvect[1]-1); y++) 
      		{
         		dist = det_dis(vect[_to2(x,y)],vec);
         		if (dist<mindist) 
			{
           			mindist  = dist;
           			win_xy->add_comp(x, 0);
           			win_xy->add_comp(y, 1);       
         		}
      		}
    	}
	winner_x = win_xy->pvect[0];
	winner_y = win_xy->pvect[1]; 
	if (par->nfct==MEXNB)
		par->d=(mindist);
		
    	// update the grid
	for (vei_t x=0; x<max_xy->pvect[0]; x++) 
	{
      		for (vei_t y=0; y<max_xy->pvect[1]; y++) 
      		{ 
           		xy.add_comp(x, 0);
			xy.add_comp(y, 1);
			
	   		nb = det_nb(*win_xy, xy, par->nfct);
			if (par->autol)
				vect[_to2(x,y)] +=(det_lr(lr) * nb * (vec - vect[_to2(x,y)]));
			else
            			vect[_to2(x,y)] +=(lr * nb * (vec - vect[_to2(x,y)]));
         	}
      	}         
}

oas_t KSOM::det_nb(KVector& vec1, KVector& vec2, ve_t fct)
{
	switch (fct)
	{
		case EUCLNB:	return (1.0/(1.0 + vec1.dis_eucl(vec2)));
		case MANHNB:	return (1.0/(1.0 + vec1.dis_manh(vec2)));
		case CHEBNB:	return (1.0/(1.0 + vec1.dis_cheb(vec2)));
		case MINKNB:	return (1.0/(1.0 + vec1.dis_mink(vec2, par->minkexp)));
		case MEXNB:	if ((vec1.dis_eucl(vec2)) <= par->d)
					return det_nb(vec1,vec2,GAUSSNB);
				else if ((par->d < vec1.dis_eucl(vec2)) 
					  && 
					 (vec1.dis_eucl(vec2) <= (3*par->d+1)))
					return -(det_nb(vec1,vec2,GAUSSNB)/par->roh); 
				else if (vec1.dis_eucl(vec2) > (3*par->d+1))
					return 0.0;
		case GAUSSNB:   return exp(((-0.5)*vec1.dis_eucl(vec2))/(2*par->nb_radius*par->nb_radius));
	}
	return 1.0;
}

oas_t KSOM::det_lr(oas_t lr)
{
	oas_t temp=0.0;
	switch (par->lfct)
	{
		case LIN:	temp = (-((1/par->c) * par->epoch) + lr);
				if (temp<0)
					return 0;
				else
					return (-((1/par->c) * par->epoch) + lr);
		case LOG:	return ((1/(par->c * log(par->epoch))) + lr);
		case EXP:	return ((1/(par->c * exp(par->epoch))) + lr);
	}
	return 1.0;
}

oas_t KSOM::getCell(vei_t x, vei_t y, vei_t i) 
{
    return vect[_to2(x,y)].get_comp(i);
}

DVector& KSOM::getCellVector(vei_t x, vei_t y) 
{
    return vect[_to2(x,y)];
}

/****************************************************************************************
*	This method realizes a different way of training the neuron grid using		*
*	the distance of the input vector to the neurons in the grid to distribute 	*
*	the data over the map. No determination of a winner neuron is needed in 	*
*	this case. The new feed method works only on a pre trained map.			*
****************************************************************************************/
void KSOMfct::feed_NoWinner(DVector& vec, float lr) 
{
	KVector xy(2);
    	oas_t mindist = (2.0 * DBL_MAX);
    	float nb;   
	 
	DVector distances(_to2(par->max_x-1,par->max_y));
	
	for (vei_t x=0; x<max_xy->pvect[0]; x++) 
	{
      		for (vei_t y=0; y<max_xy->pvect[1]; y++) 
      		{ 
			distances.set_comp(det_dis(vect[_to2(x,y)],vec), F64B_TYPE, _to2(x,y));
         		if (distances.get_comp(_to2(x,y))<mindist) 
			{
           			mindist  = distances.get_comp(_to2(x,y));
				win_xy->add_comp(x, 0);
           			win_xy->add_comp(y, 1); 
			}
		}
	}
	winner_x = win_xy->pvect[0];
	winner_y = win_xy->pvect[1]; 
	if (par->nfct==MEXNB)
		par->d=(mindist);
	
    	// update the grid
	for (vei_t x=0; x<max_xy->pvect[0]; x++) 
	{
      		for (vei_t y=0; y<max_xy->pvect[1]; y++) 
      		{ 
			nb = det_nb((distances.get_comp(_to2(x,y))/mindist), par->nfct);
		
			if (par->autol)
				vect[_to2(x,y)] +=(det_lr(lr) * nb * (vec - vect[_to2(x,y)]));
			else
            			vect[_to2(x,y)] +=(lr * nb * (vec - vect[_to2(x,y)]));
         	}
      	}         
}

oas_t KSOMfct::det_nb(oas_t dist, ve_t fct)
{
	switch (fct)
	{
		case EUCLNB:	return (1.0/(1.0 + dist));
		case MANHNB:	return (1.0/(1.0 + dist));
		case CHEBNB:	return (1.0/(1.0 + dist));
		case MINKNB:	return (1.0/(1.0 + dist));
		case MEXNB:	if (dist <= par->d)
					return det_nb(dist,GAUSSNB);
				else if ((par->d < dist) && (dist <= (3*par->d+1)))
					return -(det_nb(dist,GAUSSNB)/par->roh); 
				else if (dist > (3*par->d+1))
					return 0.0;
		case GAUSSNB:   return exp(((-2.5)*dist)/(par->nb_radius*par->nb_radius));
	}
	return 1.0;
}
