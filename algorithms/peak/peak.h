/***************************************************************************
                              peak.h  -  v.1.1
                             ------------------
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

#ifndef PEAK_H
#define PEAK_H

#include <stdio.h>
#include <stdlib.h>
#include "cstk_base/types.h"
#include "cstk_base/vector/kvector.h"

/**Peak Analysis 
  *@author Kristof Van Laerhoven

  Peak represents an array of 'peaks' that have been detected in a sensor
  signal by kvector's get_peak() member function. 
  
  This class reuses the types from the kvector class, by defining a peak as
  an element in an array containing 3 values per cell:
   - the height of the peak => this is the same type as a kvector component
   - the size, or area under a peak => same type as kvector sum of components
   - the length of the peak => same type as an iterator (or length) of kvector
  
  The length of a peak array has been given the same type as the vectors, but 
  is redefined as pei_t for 'peak element iterator type'. The type for a sum
  of peaklengths has been defined as pls_t, and for the sum of peaksizes has
  been defined as pss_t.
      
*/

// peak element iterator type
typedef s_16b pei_t;
#ifndef max_pei
#define max_pei s_16b_max // 32767
#endif
// peak amplitude sum type  
typedef s_32b pas_t;
#ifndef max_pas
#define max_pas (max_ve*max_pei) // 8355585 < 2147483647
#endif
// peak length sum type  
typedef s_32b pls_t;
#ifndef max_pls
#define max_pls (max_vei*max_pei) // 1073676289 < 
#endif                            // 2147483647
// peak size sum type  
typedef u_32b pss_t;
#ifndef max_pss
#define max_pss u_32b_max   // 273787453695 (max_ves*max_pei)> 
#endif                      //   4294967295    !

typedef struct Peakstruct {
  ve_t  a;
  vei_t l;
  ves_t s;
  bool  p;
} Pk;  

typedef struct Peakstats_struct {
  ve_t  a_min;  
  ve_t  a_max;
  pas_t  a_mean;  
  pas_t  a_var;  
  vei_t l_min;
  vei_t l_max;
  pls_t  l_mean;
  pls_t  l_var;  
  ves_t s_min;
  ves_t s_max;
  pss_t  s_mean;
  pss_t  s_var;  
} PeakStats;  

class Peak {
public: 
  Peak() {ppeak_s=NULL; ppeak_a=NULL; ppeak_l=NULL; ppeak_p=NULL;};
  Peak(pei_t size);
  ~Peak();
  void createPeak(pei_t size);
  /** Add the size, amp, length and sign, left-shifting others if necessary */
  void add_peak(ves_t psize, ve_t pamp, vei_t plen, bool psgn);
  /** Add a peak, left-shifting others if necessary */
  void add_peak(Pk* pk);
  /** get the amplitude's stats */
  void get_a_stats(ve_t* min, ve_t* max, pas_t* mean, pas_t* var);
  /** get the length's stats */
  void get_l_stats(vei_t* min, vei_t* max, pls_t* mean, pls_t* var);
  /** get the size's stats */
  void get_s_stats(ves_t* min, ves_t* max, pss_t* mean, pss_t* var);
  /** get the all the stats */
  void get_stats(PeakStats* ps);
  
public:    
  ves_t *ppeak_s;  // peak size (=area under the peak)
  ve_t  *ppeak_a;  // peak amplitude (=height of the peak vs baseline)
  vei_t *ppeak_l;  // peak length (in vector elements)
  bool  *ppeak_p;  // peak sign (boolean for it being positive)
  
  pei_t  ppeak_size;

private:
  ve_t  a_min;  
  ve_t  a_max;
  pas_t  a_rsum;  
  pas_t  a_rsqsum;
  
  vei_t l_min;
  vei_t l_max;
  pls_t  l_rsum;
  pls_t  l_rsqsum;
  
  ves_t s_min;
  ves_t s_max;
  pss_t  s_rsum;
  pss_t  s_rsqsum;
  
};

#endif
