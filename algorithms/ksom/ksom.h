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

#define LINNB 0
#define MANHNB 1
#define MEXNB 2
#define GAUSSNB 3

class KSOM {
 public: 
  	KSOM();
  	/* create the map with the right dimensions and all parameters*/
  	KSOM(vei_t x, vei_t y, vei_t n, ve_t distance=DIS_EUCL, ve_t neighbourfct=LINNB, bool autolearn=false, ve_t learnfct=LIN);
  	~KSOM();
  	/* create the map with the right dimensions and all parameters*/
  	void create(vei_t x, vei_t y, vei_t n, ve_t distance=DIS_EUCL, ve_t neighbourfct=LINNB, bool autolearn=false, ve_t learnfct=LIN);
  	/* randomize the contents of the map's cells, prototyp vector spezializes the range of each dimension*/
  	void initRandom(DVector& prototyp);
  	/* feed a vector into the map, lr specifies the learning rate when autolearn=false*/
  	void feed(DVector& vec, float lr);
  	/* get the value of the cell x/y*/
  	oas_t getCell(vei_t x, vei_t y, vei_t i);
	
  	/* winner coords on the map*/
  	KVector *win_xy;  
	vei_t winner_x, winner_y; 
	vei_t max_x, max_y; 
	
	oas_t c;
	/*number of epochs trained jet*/ 
	vei_t epoch;
	/*neighbourhood radius*/
	oas_t nb_radius;
	
 private:
  	oas_t det_dis(DVector& vec1, DVector& vec2);
	oas_t det_dis(KVector& vec1, KVector& vec2);
	oas_t det_nb(KVector& vec1, KVector& vec2);
	oas_t det_lr(oas_t lr);
  	DVector *vect;
  	KVector  *max_xy;  
  	ve_t nfct;
  	ve_t dist;
	ve_t exp;
  	bool autol;
  	ve_t lfct;
};

#endif
