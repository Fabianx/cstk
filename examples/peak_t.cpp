/***************************************************************************
                            Peak_t.cpp  -  v.1.00
                            ---------------------
    begin                : Tue Aug 19 2003
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

#include "algorithms/peak/peak.h"
#include "cstk_base/vector/kvector.h"
#include <stdio.h>

/**Peak Analysis Demo
  *@author Kristof Van Laerhoven

  This small demo extracts peaks from a datafile and prints their details
  step by step (i.e., a value is read from the file, the peak array gets 
  updated, the statistics get updated, and this ad infinitum till the file 
  is read).
  
  the output steps all look like this:
  
   17  18  17  34  23  ...  19  17  16  17  17  -> <...stats...>
   23   1  32  27   3  ...   1  46  37   1   5  -> <...stats...>
    8   1  10   9   2  ...   1  14  11   1   2  -> <...stats...>
    6   1   5   4   5  ...   1   5   6   2   4  -> <...stats...>
    +   -   +   -   +  ...   -   +   -   +   -

  where the first line gives the last read values from the file (plus
  stats**), the second line gives the sizes of the peaks, the third line 
  gives the amplitudes of the peaks, and the last line gives the peaks'     
  lengths. 
     
  **: see the vector documentation on how to interpret those.
*/

int main(int ac, char **args) {

 Pk pk;
 PeakStats ps;
 Peak peak(20);
 KVector vect(20);
 FILE* fp;
 uint val;
 int ret;
 
 if (ac>1) fp = fopen(args[1],"r");
 else fp = fopen("test.txt","r");
 if (fp!=NULL) {
   while (!feof(fp)) {
    ret = fscanf(fp,"%i ",&val);
    if ( ret!=0 ) {
      vect.add_comp(val);
      vect.get_peak(&pk.s, &pk.a, &pk.l, &pk.p);
      for (vei_t i=0; i<vect.pvect_size; i++)  printf("%3i ", vect.pvect[i]);
      printf(" -> %c[%3li %3i %3i] (%3i-%3i,%li,%3i-%3i)\n\r", (pk.p==1)?'+':'-',
       pk.s, pk.a, pk.l, vect.min(), vect.mean(), vect.var(), vect.median(), vect.max());
      peak.add_peak(&pk);
      peak.get_stats(&ps);  
      for (pei_t i=0; i<peak.ppeak_size; i++)  
         printf("%3li ", peak.ppeak_s[i]);
      printf(" -> [%3li %3li %3li %3li]\n\r", ps.s_min, ps.s_max, ps.s_mean, ps.s_var);
      for (pei_t i=0; i<peak.ppeak_size; i++)  
         printf("%3i ", peak.ppeak_a[i]);  
      printf(" -> [%3i %3i %3li %3li]\n\r", ps.a_min, ps.a_max, ps.a_mean, ps.a_var);
      for (pei_t i=0; i<peak.ppeak_size; i++)  
         printf("%3i ", peak.ppeak_l[i]);  
      printf(" -> [%3i %3i %3li %3li]\n\r", ps.l_min, ps.l_max, ps.l_mean, ps.l_var);
      for (pei_t i=0; i<peak.ppeak_size; i++)  
         printf("  %c ", (peak.ppeak_p[i]==1)?'+':'-');  
      printf("\n\r");
    }  
   }
   fclose(fp);
 }
 else
    printf("datafile not found!\n\r");
   
}
