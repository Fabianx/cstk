/***************************************************************************
                            bvector.cpp  -  v.1.00
                            ----------------------
    begin                : Aug 9 2004
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

/**** BVector Declarations *************************************************/

template<class T> BVector<T>::BVector() 
{
   size = 0;
   comp = NULL; 
   sorted = NULL;
   strout = NULL;
}

template<class T> BVector<T>::BVector(vei_t nsize) 
{
   vei_t i;
   size = nsize; 
   comp = new T[size];
   sorted = new T[size];
   for (i=0; i<size; i++) comp[i] = 0;      
   comp_min = 0;
   comp_max = 0;
   rsum = 0;
   rsqsum = 0;
   strout = NULL;
}

template<class T> BVector<T>::~BVector() 
{
   if (comp!=NULL)   delete []comp;
   if (sorted!=NULL) delete []sorted;
   if (strout!=NULL) delete []strout;
}

template<class T> 
void BVector<T>::create(vei_t nsize) 
{
   vei_t i;
   size=nsize; 
   sorted = new T[size];
   comp = new T[size];
   for (i=0; i<size; i++) comp[i] = 0;      
   comp_min = 0;
   comp_max = 0;
   rsum = 0;
   rsqsum = 0;
}

template<class T> 
void BVector<T>::add_comp(T newvalue) 
{
  // recalculate running sum and sum of squares:
  rsum -= comp[0];
  rsqsum -= (comp[0]*comp[0]);
  // recalculate min/max if value that goes out is min or max:
  if ((comp[0] == comp_min) || (comp[0] == comp_max))
      comp_min = comp[1];    comp_max = 0;
      for (vei_t i=1; i<size; i++) {
         if (comp[i] > comp_max) comp_max = comp[i];
         if (comp[i] < comp_min) comp_min = comp[i];      
      }
  // update with last new value:
  for (vei_t i=0; i<size-1; i++) comp[i]= comp[i+1];
  if (newvalue > comp_max) comp_max = newvalue;
  if (newvalue < comp_min) comp_min = newvalue;
  rsum += newvalue;  
  rsqsum += (newvalue*newvalue);
  comp[size-1] = newvalue;
}

template<class T> 
T BVector<T>::get_comp(vei_t iter) 
{
   return comp[iter];
}

template<class T> 
void BVector<T>::set_comp(T value, vei_t iter) 
{
   comp[iter]=value;
}

template<class T> 
void BVector<T>::inc_comp(vei_t iter) 
{
   comp[iter]++;
}

template<class T> 
void BVector<T>::dec_comp(vei_t iter) 
{
   comp[iter]--;
}

template<class T> 
vei_t BVector<T>::get_dim() 
{
   return size;
}
   
template<class T> 
T BVector<T>::min() {
   return comp_min;
}

template<class T> 
T BVector<T>::max() {
   return comp_max;
}

template<class T> 
T BVector<T>::mean() {
   return (T) (rsum/size);
}

template<class T> 
T BVector<T>::median() {
   // do a quicksort:
    for (vei_t i=0; i<size; i++) sorted[i] = comp[i];
    BVector<T>::qsort(0,size-1); 
   return (sorted[(vei_t)(size/2)]);
}

template<class T> 
oas_t BVector<T>::rvar() {
   return (oas_t)((rsqsum - ((rsum*rsum)/size))/size);
}

template<class T> 
oas_t BVector<T>::std() {
   return (oas_t) sqrt(rvar()); 
}

template<class T> 
oas_t BVector<T>::var() {
   oas_t variance = 0;
   T avg = mean();
   for (vei_t i=0; i<size; i++) variance += (comp[i]-avg)*(comp[i]-avg);
   variance /= size;
   return variance;
}   

template<class T> 
T BVector<T>::val() {
  return comp[size-1];
}        

template<class T> 
oas_t BVector<T>::get_histo(T buckets, T current) {
  oas_t sum = 0;
  // stub
  return (sum);
} 

template<class T> 
char BVector<T>::get_peak(oas_t *nsize, T *amp, vei_t *lngth, bool *sgn) {
  vei_t i=size-1;
  T t_mean = mean(), t_amp;
  *amp = 0;
  *sgn = boolsign(comp[i]-t_mean);   // 1 = pos, 0 = neg
  *nsize = 0;
  do {
     t_amp = abs(comp[i]-t_mean);
     *nsize += t_amp;
     if (*amp<t_amp) *amp = t_amp;  
     i--;
  } while ((i>0) && (boolsign(comp[i]-t_mean)==*sgn));  
  *lngth = size-i-1;  
  return 0;  
}

template<class T> 
char* BVector<T>::to_string(const char* format, int cell_len) {
  vei_t iter = 0, iter2 = 0;
  if (strout!=NULL) delete []strout;
  strout = new char[(size*cell_len)+1];
  char strtmp[cell_len];
  for (vei_t i=0; i<size; i++) {
     iter = sprintf(strtmp, format, comp[i]);
     for (unsigned char j=0; j<cell_len; j++) strout[j+iter2] = strtmp[j];
     iter2 += iter;
  }
  strout[iter2] = '\0';
  return strout; 
}

/** Quicksort: */
template<class T> 
void BVector<T>::qsort(vei_t start, vei_t end){     
  vei_t i = 0,j = 0;
  T x, t;             
  if (start<end) {
    i = start;   j = end+1;
    x = sorted[start];
    for(;;) { 
      do i++; while ((comp[i]<x)&&(i<=end-1));
      do j--; while ((comp[j]>x)&&(j>=start+1));
      if (i<j) {t=sorted[i]; sorted[i]=sorted[j]; sorted[j]=t;}  
      else
         break;
    }
    t=sorted[start]; sorted[start]=sorted[j]; sorted[j]=t;
    if (j!=0) BVector<T>::qsort(start, j-1); 
    qsort(j+1, end);   
  }
}


template<class T>
double BVector<T>::dis_eucl(BVector<T>& invec)
{
	double sumeukl = 0.0;
	for (vei_t h=0; h<size; h++)
	{
		sumeukl += (pow((comp[h]- invec.get_comp(h)), 2.0));
	}
	sumeukl = sqrt(sumeukl);
	return sumeukl;
}

template<class T>
double BVector<T>::dis_manh(BVector<T>& invec)
{
	double summanh = 0.0;
	for (vei_t h=0; h<size; h++)
	{
		summanh += abs(comp[h]- invec.get_comp(h));
	}
	return summanh;
}

template<class T>
double BVector<T>::dis_cheb(BVector<T>& invec)
{
	double maxcheb = 0.0;
	for (vei_t h=0; h<size; h++)
	{
		if((abs(comp[h]- invec.get_comp(h)))>maxcheb)
		{
			maxcheb = abs(comp[h]- invec.get_comp(h));
		}
	}
	return maxcheb;
}

template<class T>
double BVector<T>::dis_mink(BVector<T>& invec, ve_t exp)
{
	double summink = 0.0;
	for (vei_t h=0; h<size; h++)
	{
		summink += pow(abs(comp[h]- invec.get_comp(h)), (double)(exp));
	}
	summink = pow(summink, (1/(double)(exp)));
	return summink;
}

