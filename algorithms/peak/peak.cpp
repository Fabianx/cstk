/***************************************************************************
                              peak.cpp  -  v.1.1
                             --------------------
    begin                : Mon Aug 18 2003
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

#include "peak.h"

   
Peak::Peak(pei_t size){
  ppeak_size = size;
  ppeak_s = new ves_t[size];
  ppeak_a = new ve_t[size];
  ppeak_l = new vei_t[size];
  ppeak_p = new bool[size];
  
  // initialise all:
  for (pei_t i=0; i<size; i++) {
    ppeak_s[i] = 0;
    ppeak_a[i] = 0;
    ppeak_l[i] = 0;
    ppeak_p[i] = 0;
  }
  s_min = 0;   s_max = 0;
  s_rsum = 0;  s_rsqsum = 0;
  a_min = 0;   a_max = 0;
  a_rsum = 0;  a_rsqsum = 0;
  l_min = 0;   l_max = 0;
  l_rsum = 0;  l_rsqsum = 0;
}

Peak::~Peak(){
  if (ppeak_s!=NULL) delete[] ppeak_s;
  if (ppeak_a!=NULL) delete[] ppeak_a;
  if (ppeak_l!=NULL) delete[] ppeak_l;
  if (ppeak_p!=NULL) delete[] ppeak_p;
}

void Peak::createPeak(pei_t size){
  ppeak_size = size;
  ppeak_s = new ves_t[size];
  ppeak_a = new ve_t[size];
  ppeak_l = new vei_t[size];
  ppeak_p = new bool[size];
  
  // initialise all:
  for (pei_t i=0; i<size; i++) {
    ppeak_s[i] = 0;
    ppeak_a[i] = 0;
    ppeak_l[i] = 0;
    ppeak_p[i] = 0;
  }
  s_min = 0;   s_max = 0;
  s_rsum = 0;  s_rsqsum = 0;
  a_min = 0;   a_max = 0;
  a_rsum = 0;  a_rsqsum = 0;
  l_min = 0;   l_max = 0;
  l_rsum = 0;  l_rsqsum = 0;
}

/** Add the size, amplitude and length, shifting all others to the left if necessary */
void Peak::add_peak(ves_t psize, ve_t pamp, vei_t plen, bool psgn) {
  if ( ppeak_p[ppeak_size-1] != psgn ) {
    ////// if stats: //////////////////////////////////////////////////////////
       // update stats:
        s_rsum -= ppeak_s[0];
        s_rsqsum -= (ppeak_s[0]*ppeak_s[0]);
        a_rsum -= ppeak_a[0];
        a_rsqsum -= (ppeak_a[0]*ppeak_a[0]);
        l_rsum -= ppeak_l[0];
        l_rsqsum -= (ppeak_l[0]*ppeak_l[0]);
       // recalculate if the value that goes out is min or max:
        if ((ppeak_s[0] == s_min) || (ppeak_s[0] == s_max)) {
           s_min = max_ves;    s_max = min_ves;
           for (pei_t i=1; i<ppeak_size; i++) {
              if (ppeak_s[i] > s_max) s_max = ppeak_s[i];
              if (ppeak_s[i] < s_min) s_min = ppeak_s[i];
           }
        }
        if ((ppeak_a[0] == a_min) || (ppeak_a[0] == a_max)) {
           a_min = max_ve;    a_max = min_ve;
           for (pei_t i=1; i<ppeak_size; i++) {
              if (ppeak_a[i] > a_max) a_max = ppeak_a[i];
              if (ppeak_a[i] < a_min) a_min = ppeak_a[i];
           }
        }
        if ((ppeak_l[0] == l_min) || (ppeak_l[0] == l_max)) {
           l_min = max_vei;    l_max = min_vei;
           for (pei_t i=1; i<ppeak_size; i++) {
              if (ppeak_l[i] > l_max) l_max = ppeak_l[i];
              if (ppeak_l[i] < l_min) l_min = ppeak_l[i];
           }
        }
        if (ppeak_s[ppeak_size-1] > s_max) s_max = ppeak_s[ppeak_size-1];
        if (ppeak_s[ppeak_size-1] < s_min) s_min = ppeak_s[ppeak_size-1];
        s_rsum += ppeak_s[ppeak_size-1];
        s_rsqsum += (ppeak_s[ppeak_size-1]*ppeak_s[ppeak_size-1]);   
        if (ppeak_a[ppeak_size-1] > a_max) a_max = ppeak_a[ppeak_size-1];
        if (ppeak_a[ppeak_size-1] < a_min) a_min = ppeak_a[ppeak_size-1];
        a_rsum += ppeak_a[ppeak_size-1];
        a_rsqsum += (ppeak_a[ppeak_size-1]*ppeak_a[ppeak_size-1]);  
        if (ppeak_l[ppeak_size-1] > l_max) l_max = ppeak_l[ppeak_size-1];
        if (ppeak_l[ppeak_size-1] < l_min) l_min = ppeak_l[ppeak_size-1];
        l_rsum += ppeak_l[ppeak_size-1];
        l_rsqsum += (ppeak_l[ppeak_size-1]*ppeak_l[ppeak_size-1]);
    ////// if stats: /////////////////////////////////////////////////////////
       // shift left
        for (pei_t i=0; i<ppeak_size-1; i++) {
           ppeak_s[i] = ppeak_s[i+1];
           ppeak_a[i] = ppeak_a[i+1];
           ppeak_l[i] = ppeak_l[i+1];         
           ppeak_p[i] = ppeak_p[i+1];         
        } 
  }
  // update last
  ppeak_s[ppeak_size-1] = psize;
  ppeak_a[ppeak_size-1] = pamp;
  ppeak_l[ppeak_size-1] = plen;
  ppeak_p[ppeak_size-1] = psgn;
}

/** Add the size, amplitude and length, shifting all others to the left if necessary */
void Peak::add_peak(Pk* pk) {
  add_peak(pk->a, pk->l, pk->s, pk->p);
}

/** get the amplitude's stats */
void Peak::get_a_stats(ve_t* min, ve_t* max, pas_t* mean, pas_t* var) {
  *min = a_min;
  *max = a_max;
  *mean = (a_rsum/ppeak_size);
  *var = ((a_rsqsum - ((a_rsum*a_rsum)/ppeak_size))/ppeak_size);
}

/** get the length's stats */
void Peak::get_l_stats(vei_t* min, vei_t* max, pls_t* mean, pls_t* var) {
  *min = l_min;
  *max = l_max;
  *mean = (l_rsum/ppeak_size);
  *var = ((l_rsqsum - ((l_rsum*l_rsum)/ppeak_size))/ppeak_size);
}

/** get the size's stats */
void Peak::get_s_stats(ves_t* min, ves_t* max, pss_t* mean, pss_t* var) {
  *min = s_min;
  *max = s_max;
  *mean = (s_rsum/ppeak_size);
  *var = ((s_rsqsum - ((s_rsum*s_rsum)/ppeak_size))/ppeak_size);
}

/** get all the stats */
void Peak::get_stats(PeakStats* ps) {
  get_a_stats(&ps->a_min, &ps->a_max, &ps->a_mean, &ps->a_var);
  get_l_stats(&ps->l_min, &ps->l_max, &ps->l_mean, &ps->l_var);
  get_s_stats(&ps->s_min, &ps->s_max, &ps->s_mean, &ps->s_var);
}

