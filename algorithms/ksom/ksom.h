/***************************************************************************
                              ksom.h  -  v.1.00
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

#ifndef KSOM_H
#define KSOM_H

/**2D Kohonen Self-Organizing Map
  *@author Kristof Van Laerhoven
  
  */

#include "cstk_base/vector/kvector.h"   // for the types & basic vector
#include <stdlib.h>  // rand
#include <math.h>

class KSOM {
 public: 
  KSOM();
  ~KSOM();
  /* create the map with the right dimensions */
  void createGrid(vei_t x, vei_t y, vei_t l);
  /* randomize the contents of the map's cells */
  void initRandom(ve_t base);
  /* feed a vector into the map, bnf = 1/(1+manhattan) */
  void feed_diam(KVector *vect, float lr);
  /* feed a vector into the map, bnf = 1/(1+euclidean) */
  void feed_circ(KVector *vect, float lr);
  /* feed a vector into the map, nbf = gaussian */
  void feed_bell(KVector *vect, float lr, float nb_radius);
  /* get the contents from one of the map's cells */
  void getCell(vei_t x, vei_t y, KVector *kvect);
  /* winner coords on the map*/
  vei_t   winner_x, winner_y;    
 private:
  ve_t *vect;
  vei_t  max_x, max_y, max_l;  
};

#endif
