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

KSOM::KSOM(){
    vect = NULL;
}

KSOM::~KSOM(){
    if (vect!=NULL) delete []vect;
}

void KSOM::createGrid(vei_t x, vei_t y, vei_t l) {
    vect = new ve_t[x*y*l];
    max_x = x;
    max_y = y;
    max_l = l;
}

void KSOM::initRandom(ve_t base) {
    vei_t x,y,l;    
    for (x=0; x<max_x; x++) {
      for (y=0; y<max_y; y++) {
         for (l=0; l<max_l; l++) {
              vect[_to3(x,y,l)] = (ve_t) (25.0*rand()/(RAND_MAX+1.0));
         }           
      }        
    }
}

void KSOM::feed_diam(KVector *kvect, float lr) {
    ves_t dist, mindist = max_ves;
    vei_t x,y,i;
    float nb;    
    // find winner coords
    for (x=0; x<max_x; x++) {
      for (y=0; y<max_y; y++) {
         dist = 0;
         for (i=0; i<max_l; i++)
           dist += abs( kvect->pvect[i] - vect[_to3(x,y,i)] );
         if (dist<mindist) {
           mindist  = dist;
           winner_x = x;
           winner_y = y;            
         }
      }
    }
    // update
    for (x=0; x<max_x; x++) {
      for (y=0; y<max_y; y++) {
         for (i=0; i<max_l; i++) {  
           // linear decreasing diamond
            nb = 1.0/(1.0+(abs(x-winner_x) + abs(y-winner_y)));
           // mexican hat:
            //nb = sqrt( (x-winner_x)*(x-winner_x) + (y-winner_y)*(y-winner_y) );
            //nb = sin(nb)/(nb);
            //if (nb<vect[_to3(x,y,i)]) nb=0;
           // update:
            vect[_to3(x,y,i)] += ( (ve_t)(lr*nb*(float)(kvect->pvect[i] - vect[_to3(x,y,i)])) );
         }
      }
    }             
}

void KSOM::feed_circ(KVector *kvect, float lr) {

    ves_t dist, mindist = max_ves;
    vei_t x,y,i;
    float nb;    
    // find winner coords
    for (x=0; x<max_x; x++) {
      for (y=0; y<max_y; y++) {
         dist = 0;
         for (i=0; i<max_l; i++)
           dist += abs( kvect->pvect[i] - vect[_to3(x,y,i)] );
         if (dist<mindist) {
           mindist  = dist;
           winner_x = x;
           winner_y = y;            
         }
      }
    }
    // update
    for (x=0; x<max_x; x++) {
      for (y=0; y<max_y; y++) {
         for (i=0; i<max_l; i++) {  
           // linear decreasing circle
            nb = 1.0/(1.0+sqrt( (x-winner_x)*(x-winner_x) + (y-winner_y)*(y-winner_y) )); 
           // update:
            vect[_to3(x,y,i)] += ( (ve_t)(lr*nb*(float)(kvect->pvect[i] - vect[_to3(x,y,i)])) );
         }
      }
    }             
}

void KSOM::feed_bell(KVector *kvect, float lr, float nb_radius ) {
    ves_t dist, mindist = max_ves;
    vei_t x,y,i;
    float nb;    
    // find winner coords
    for (x=0; x<max_x; x++) {
      for (y=0; y<max_y; y++) {
         dist = 0;
         for (i=0; i<max_l; i++)
           dist += abs( kvect->pvect[i] - vect[_to3(x,y,i)] );
         if (dist<mindist) {
           mindist  = dist;
           winner_x = x;
           winner_y = y;            
         }
      }
    }
    // update
    for (x=0; x<max_x; x++) {
      for (y=0; y<max_y; y++) {
         for (i=0; i<max_l; i++) {  
           // gaussian (bell shaped):
            nb = sqrt( (x-winner_x)*(x-winner_x) + (y-winner_y)*(y-winner_y) );
            nb = exp((-0.5*nb)/(2*nb_radius*nb_radius));                 
            vect[_to3(x,y,i)] += ( (ve_t)(lr*nb*(float)(kvect->pvect[i] - vect[_to3(x,y,i)])) );
         }
      }
    }             
}

void KSOM::getCell(vei_t x, vei_t y, KVector *kvect) {
    for (vei_t i=0; i<max_l; i++)
       kvect->pvect[i] = vect[_to3(x,y,i)] ;
}


