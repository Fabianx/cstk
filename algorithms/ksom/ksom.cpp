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

#define _to3(x,y,l) (x)+((y)*max_x)+((l)*max_x*max_y)
#define _to2(x,y) (((x)*max_y)+y)

KSOM::KSOM()
{
	//printf("KSOM()1\n");
    	vect = NULL;
	c=2; 
	epoch=0;
	nb_radius=5;
	exp=2;
    	win_xy = NULL;
    	max_xy = NULL;
	//printf("KSOM()2\n");
}

KSOM::KSOM(vei_t x, vei_t y, vei_t n,ve_t distance, ve_t neighbourfct, bool autolearn, ve_t learnfct)
{
	c=2; 
	epoch=0;
	nb_radius=5;
	exp=2;
	win_xy = new KVector(2);
	max_xy = new KVector(2);
	max_xy->add_comp(x,0);
	max_xy->add_comp(y,1);
	max_y = y; max_x = x;
	dist = distance;
	nfct = neighbourfct;
	autol = autolearn;
	lfct = learnfct;
	vect = new DVector[_to2(x-1,y)];
	for (vei_t i=0; i<(_to2(x-1,y)); i++)
		vect[i].create(n);
}

KSOM::~KSOM()
{
    	if (vect!=NULL) delete []vect;
    	delete win_xy;
    	delete max_xy;
}

void KSOM::create(vei_t x, vei_t y, vei_t n,ve_t distance, ve_t neighbourfct, bool autolearn, ve_t learnfct)
{
	//printf("create(..)1\n");
	win_xy = new KVector(2);
    	max_xy = new KVector(2);
    	max_xy->add_comp(x,0);
	max_xy->add_comp(y,1);
	max_y = y; max_x = x;
	dist = distance;
	nfct = neighbourfct;
	autol = autolearn;
	lfct = learnfct;
	vect = new DVector[_to2(x-1,y)];
	for (vei_t i=0; i<(_to2(x-1,y)); i++)
		vect[i].create(n);
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
				case U8B_TYPE:  vect[_to2(x,y)].set_comp(((u_8b_max)*((double)(rand())/(double)(RAND_MAX+1))),U8B_TYPE,n); 
						break;
				case U16B_TYPE: vect[_to2(x,y)].set_comp(((u_16b_max)*((double)(rand())/(double)(RAND_MAX+1))),U16B_TYPE,n); 
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
/*printf("\n");
for (vei_t x=0; x<max_xy->pvect[0]; x++) 
{
      	for (vei_t y=0; y<max_xy->pvect[1]; y++) 
	{
		printf("vec_%i_%i = vec[%i]",x,y,_to2(x,y));
		for (vei_t n=0; n<prototyp.get_dim(); n++) 
			printf("%7.3e ",vect[_to2(x,y)].get_comp(n));
		printf("\n");
	}
}
printf("\n");*/	
}

oas_t KSOM::det_dis(DVector& vec1, DVector& vec2)
{
	switch (dist)
	{
		case DIS_MANH:	return vec1.dis_manh(vec2);
		case DIS_CHEB:	return vec1.dis_cheb(vec2);
		case DIS_EUCL:	return vec1.dis_eucl(vec2);
		case DIS_MINK:	return vec1.dis_mink(vec2, exp);
	}
	return vec1.dis_manh(vec1);
}

oas_t KSOM::det_dis(KVector& vec1, KVector& vec2)
{
	switch (dist)
	{
		case DIS_MANH:	return vec1.dis_manh(vec2);
		case DIS_CHEB:	return vec1.dis_cheb(vec2);
		case DIS_EUCL:	return vec1.dis_eucl(vec2);
		case DIS_MINK:	return vec1.dis_mink(vec2, exp);
	}
	return vec1.dis_manh(vec1);
}

void KSOM::feed(DVector& vec, float lr) 
{
	KVector xy(2);
    	oas_t dist, mindist = (2.0 * DBL_MAX);;
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
    	// update the grid
	for (vei_t x=0; x<max_xy->pvect[0]; x++) 
	{
      		for (vei_t y=0; y<max_xy->pvect[1]; y++) 
      		{ 
           		xy.add_comp(x, 0);
			xy.add_comp(y, 1);
	   		nb = det_nb(*win_xy, xy);
			//printf("nb=%7.3e\n",nb);
	    		if (autol)
				vect[_to2(x,y)] +=(det_lr(lr) * nb * (vec - vect[_to2(x,y)]));
			else
            			vect[_to2(x,y)] +=(lr * nb * (vec - vect[_to2(x,y)]));
         	}
      	}         
}

oas_t KSOM::det_nb(KVector& vec1, KVector& vec2)
{
	switch (nfct)
	{
		case LINNB:	return (1.0/(1.0 + vec1.dis_eucl(vec2)));
		case MANHNB:	return (1.0/(1.0 + vec1.dis_manh(vec2)));
		case MEXNB:	return ((-0.5*vec1.dis_manh(vec2))/(2*nb_radius*nb_radius));
		case GAUSSNB:   return 0.0;
	}
}

oas_t KSOM::det_lr(oas_t lr)
{
	switch (lfct)
	{
		case LIN:	return (-((1/c) * epoch) + lr);
		case LOG:	return (1/log(lr * epoch));
		case EXP:	return (1/(lr*epoch));
	}
}

oas_t KSOM::getCell(vei_t x, vei_t y, vei_t i) 
{
    return vect[_to2(x,y)].get_comp(i);
}


