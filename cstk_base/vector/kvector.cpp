/***************************************************************************
                            kvector.cpp  -  v.1.10
                            ----------------------
    begin                : Aug 17 2002
    copyright            : (C) 2002-2003 by Kristof Van Laerhoven
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

#include "kvector.h"
 
// standard constructor
KVector::KVector()
{
  pvect_size = 0;
  pvect = NULL;
  strout = NULL;
  pvect_sorted = NULL;
}
// constructor specifying size and activating statistics
KVector::KVector(vei_t size, bool stats)
{
  pvect_size = size;
  pvect = new ve_t[pvect_size];
  strout = NULL;
  if (stats) {
     vei_t i;
     pvect_sorted = new ve_t[pvect_size];
     for (i=0; i<size; i++) pvect[i] = 0;      
     for (i=0; i<max_ve+1; i++) histo[i] = 0;
     pvect_min = 0;
     pvect_max = 0;
     pvect_rsum = 0;
     pvect_rsqsum = 0;
  }
}
// destructor:
KVector::~KVector()
{
  if (pvect!=NULL) delete[] pvect;
  if (pvect_sorted!=NULL) delete[] pvect_sorted;
  if (strout!=NULL) delete []strout;
}
// Create explicit vector
void KVector::createVector(vei_t size)
{
   vei_t i;
   pvect_size = size;
   pvect = new ve_t[pvect_size];
   pvect_sorted = new ve_t[pvect_size];
   for (i=0; i<size; i++) pvect[i] = 0;
   for (i=0; i<max_ve+1; i++) histo[i] = 0;
   pvect_min = 0;
   pvect_max = 0;
   pvect_rsum = 0;
   pvect_rsqsum = 0;
}
// Fill the vector with random values
void KVector::set_rand()
{
   vei_t i;
   // reset histogram, min, max, rsum and rsqsum:
   for (i=0; i<max_ve+1; i++) histo[i] = 0; 
   pvect_min = max_ve;    pvect_max = 0;
   pvect_rsum = 0;     pvect_rsqsum = 0;
   // update all with random values:
   for (i=0; i<pvect_size; i++) {
     pvect[i]= (ve_t) ((float)max_ve*rand()/(RAND_MAX+1.0));
     if (pvect[i] > pvect_max) pvect_max = pvect[i];
     if (pvect[i] < pvect_min) pvect_min = pvect[i];
     histo[pvect[i]]++;
     pvect_rsum += pvect[i];
     pvect_rsqsum += (pvect[i]*pvect[i]);      
   }
}
// Add a value after shifting all values to the left
void KVector::add_comp(ve_t newvalue)
{
   // recalculate histogram, running sum and sum of squares:
   if (histo[pvect[0]]>0)  histo[pvect[0]] --;
   pvect_rsum -= pvect[0];
   pvect_rsqsum -= (pvect[0]*pvect[0]);
    // recalculate if the value that goes out is min or max:
   if ((pvect[0] == pvect_min) || (pvect[0] == pvect_max))
       pvect_min = max_ve;    pvect_max = 0;
       for (vei_t i=1; i<pvect_size; i++) {
          if (pvect[i] > pvect_max) pvect_max = pvect[i];
          if (pvect[i] < pvect_min) pvect_min = pvect[i];      
       }
   // update with last new value:
   for (vei_t i=0; i<pvect_size-1; i++) pvect[i]= pvect[i+1];
   if (newvalue > pvect_max) pvect_max = newvalue;
   if (newvalue < pvect_min) pvect_min = newvalue;
   pvect_rsum += newvalue;  
   pvect_rsqsum += (newvalue*newvalue);
   pvect[pvect_size-1] = newvalue;
   histo[newvalue]++;
}
// Add/replace a value at the specified place in the array
void KVector::add_comp(ve_t newvalue, vei_t place)
{
   pvect[place] = newvalue;
}
// Return the size of the vector 
vei_t KVector::get_dim()
{
  return pvect_size;
}
// Get the minimum 
ve_t KVector::min()
{
  return pvect_min;  
}
// Get the maximum 
ve_t KVector::max()
{
  return pvect_max;
}
// Get the mean 
ve_t KVector::mean()
{
  return (ve_t)((pvect_rsum)/pvect_size);
}
// Get the median 
ve_t KVector::median()
{
  // do a quicksort:
  for (vei_t i=0; i<pvect_size; i++) pvect_sorted[i] = pvect[i];
  qsort(0,pvect_size-1); 
  return (pvect_sorted[(vei_t)(pvect_size/2)]);
}
// Get the last value
ve_t KVector::val()
{
  return pvect[pvect_size-1];
}
// Get the running variance
ves_t KVector::rvar()
{
  return (ves_t)((pvect_rsqsum - ((pvect_rsum*pvect_rsum)/pvect_size))/pvect_size);
}
// Get the variance 
ves_t KVector::var()
{
  ves_t variance = 0;
  ve_t tmp_mean = (pvect_rsum/pvect_size);
  for (vei_t i=0; i<pvect_size; i++) 
     variance += (pvect[i]-tmp_mean)*(pvect[i]-tmp_mean);
  variance /= pvect_size;
  return variance;
}
// Get the standard deviation 
ves_t KVector::std()
{
  return (ves_t) sqrt(rvar()); 
}
// Get histogram bucket 'current' if there are 'buckets' buckets
ves_t KVector::get_histo(ve_t buckets, ve_t current)
{
  ves_t sum = 0;
  for (vei_t i=((max_ve+1)*current)/buckets; 
       i<(vei_t)(((max_ve+1)*(current+1))/buckets); 
       i++) 
       sum += histo[i];
  return (sum);
}          
// Get the size, amplitude, length, and sign of the last peak 
char KVector::get_peak(ves_t *size, ve_t *amp, vei_t *lngth, bool *sgn)
{
  vei_t i=pvect_size-1;
  ve_t t_mean = mean(), t_amp;
  *amp = 0;
  *sgn = boolsign(pvect[i]-t_mean);   // 1 = pos, 0 = neg
  *size = 0;
  do {
     t_amp = abs(pvect[i]-t_mean);
     *size += t_amp;
     if (*amp<t_amp) *amp = t_amp;  
     i--;
  } while ((i>0) && (boolsign(pvect[i]-t_mean)==*sgn));  
  *lngth = pvect_size-i-1;  
  return 0;
}
// output to string: 
char* KVector::to_string(const char* format, int cell_len)
{
  vei_t iter = 0, iter2 = 0;
  if (strout!=NULL) delete []strout;
  strout = new char[(pvect_size*cell_len)+1];
  char strtmp[cell_len];
  for (vei_t i=0; i<pvect_size; i++) {
     iter = sprintf(strtmp, format, pvect[i]);
     for (unsigned char j=0; j<cell_len; j++) strout[j+iter2] = strtmp[j];
     iter2 += iter;
  }
  strout[iter2] = '\0';
  return strout; 
}
// Quicksort: (for median) 
void KVector::qsort(vei_t start, vei_t end)
{     
  vei_t i = 0,j = 0;
  ve_t x, t;
  if (start<end) {
    i = start;   j = end+1;
    x = pvect_sorted[start];
    for(;;) { 
      do i++; while ((pvect_sorted[i]<x)&&(i<=end-1));
      do j--; while ((pvect_sorted[j]>x)&&(j>=start+1));
      if ( i < j ) {
                t = pvect_sorted[i]; 
                pvect_sorted[i] = pvect_sorted[j]; 
                pvect_sorted[j] = t;
      }  
      else
         break;
    }
    t=pvect_sorted[start]; 
    pvect_sorted[start]=pvect_sorted[j]; 
    pvect_sorted[j]=t;
    if (j!=0) qsort(start, j-1);    // pick out the negative values
    qsort(j+1, end);   
  }
}
// Euclidean distance
double KVector::dis_eucl(KVector& invec)
{
    double sumeucl = 0;
    for (vei_t h=0; h<pvect_size; h++) {
       sumeucl += (int)(pow((double)(pvect[h]- invec.pvect[h]), 2.0));
    }
    sumeucl = sqrt(sumeucl);
    return sumeucl;
}
// Manhattan distance
double KVector::dis_manh(KVector& invec)
{
    double summanh = 0;
    for (vei_t h=0; h<pvect_size; h++) {
       summanh += abs(pvect[h]- invec.pvect[h]);
    }
    return summanh;
}
// Chebychev distance
double KVector::dis_cheb(KVector& invec)
{
    double maxcheb = 0;
    for (vei_t h=0; h<pvect_size; h++) {
       if((abs(pvect[h]- invec.pvect[h]))>maxcheb) {
           maxcheb = abs(pvect[h]- invec.pvect[h]);
       }
    }
    return maxcheb;
}
// Minkowsky distance
double KVector::dis_mink(KVector& invec, ve_t exp)
{
    double summink = 0;
    for (vei_t h=0; h<pvect_size; h++) {
       summink += pow((double)(abs(pvect[h]- invec.pvect[h])), (double)(exp));
    }
    summink = pow((double)(summink), (1/(double)(exp)));
    return summink;
}
