/***************************************************************************
                              ksom.h  -  v.1.00
                             -------------------
    begin                : Mon Oct 25 2004
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

#ifndef KSOM_H
#define KSOM_H

#include <stdlib.h>  // rand
#include <math.h>

#include "cstk_base/vector/dvector.h"   // for the types & basic vector
#include "cstk_base/vector/kvector.h"   // x/y winner and max

#define DIS_MANH 0
#define DIS_CHEB 1
#define DIS_EUCL 2
#define DIS_MINK 3

#define LIN 0
#define LOG 1
#define EXP 2

#define EUCLNB 0
#define MANHNB 1
#define CHEBNB 2
#define MINKNB 3
#define MEXNB 4
#define GAUSSNB 5

typedef struct KSOM_PARAM
{	
	/*---parameters used for automatig determination of the learning rate---*/
	/*----------------------------------------------------------------------*/
	/*automatic decreasing learning rate*/
  	bool autol;
	/*specification of the function that decreases the learning rate*/
  	ve_t lfct;
	/*constant that is used by learning rate calculations*/
	oas_t c;
	/*number of epochs trained jet*/ 
	vei_t epoch;
	/*----------------------------------------------------------------------*/
	
	/*---parameters to determine the neighbourhood---*/
	/*-----------------------------------------------*/
	/*specification of neighbourhood function*/
	ve_t nfct;
	/*neighbourhood radius*/
	oas_t nb_radius;
	/*-----------------------------------------------*/
	
	/*---specification of the distance masurement used to determine the winner neuron---*/
	/*----------------------------------------------------------------------------------*/
	/*selection of the distance masurement*/
  	ve_t dist;
	/*exponent for Minkowski distance*/
	vei_t minkexp;
	/*----------------------------------------------------------------------------------*/
	
	/*---parameters for the maxican hat neighbour function---*/
	/*-------------------------------------------------------*/
	/*parameter that specifies the entrance of the lower zero part of the maxican hat function*/
	oas_t d;
	/*constant for mexican hat function for f(x)<0*/  
	oas_t roh;
	/*-------------------------------------------------------*/
};

/****************************************************************************************
*	Kohonen Self Organizing Map (2-dimensional) implementation with several 	*
*	different neighborhood functions, automatically determination of the 		*
*	learning rate and different measurements for the distances of neurons in 	*
*	the grid. 									*
****************************************************************************************/
class KSOM {
 public: 
  	KSOM();
  	/* create the map with the right dimensions and all parameters*/
  	KSOM(vei_t x, vei_t y, vei_t n, ve_t distance=DIS_EUCL, ve_t neighbourfct=EUCLNB, bool autolearn=false, ve_t learnfct=LIN);
  	~KSOM();
	
	void savetoFile();
	int restorefromFile();
	
  	/* create the map with the right dimensions and all parameters*/
  	void create(vei_t x, vei_t y, vei_t n, ve_t distance=DIS_EUCL, ve_t neighbourfct=EUCLNB, bool autolearn=false, ve_t learnfct=LIN);
  	/* randomize the contents of the map's cells, prototyp vector spezializes the range of each dimension*/
  	void initRandom(DVector& prototyp);
  	/* feed a vector into the map, lr specifies the learning rate when autolearn=false*/
  	void feed(DVector& vec, float lr);
  	/* get the value of the cell x/y*/
  	oas_t getCell(vei_t x, vei_t y, vei_t i);
	DVector& getCellVector(vei_t x, vei_t y);
	
  	/* winner coords on the map*/
  	KVector *win_xy;  
	vei_t winner_x, winner_y; 
	vei_t max_x, max_y; 
	
	KSOM_PARAM par;
	
 protected:
  	oas_t det_dis(DVector& vec1, DVector& vec2);
	oas_t det_dis(KVector& vec1, KVector& vec2);
	oas_t det_nb(KVector& vec1, KVector& vec2, ve_t fct);
	
	oas_t det_lr(oas_t lr);
	KVector  *max_xy; 
  	DVector *vect;
};

/****************************************************************************************
*	This is a child class of the KSOM class that implements different		*
*	functions to improve the KSOM algorithm. 					*
****************************************************************************************/
class KSOMfct : public KSOM 
{
 public:
 	/****************************************************************************************
	*	This method realizes a different way of training the neuron grid using		*
	*	the distance of the input vector to the neurons in the grid to distribute 	*
	*	the data over the map. No determination of a winner neuron is needed in 	*
	*	this case. The new feed method works only on a pre trained map.			*
	****************************************************************************************/
 	void feed_NoWinner(DVector& vec, float lr);
 private: 
 	oas_t det_nb(oas_t dist, ve_t fct);
};

#endif
