/***************************************************************************
                            dvector.cpp  -  v.1.10
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

#include "dvector.h"

DVector::DVector() 
{
     strout = NULL;
     vctsize = 0;
}

DVector::DVector(vei_t nsize) 
{
     strout = NULL;
     create(nsize);
}

DVector::~DVector() 
{   
     // destroy the allocated elements:
     void *v = NULL;
     for (vei_t i=0; i<vctsize; i++) {
        v = vct[i];
        if (v!=NULL)
        switch (types[i]){
           case U8B_TYPE:  delete (u_8b*)v;  break;
           case U16B_TYPE: delete (u_16b*)v; break; 
           case U32B_TYPE: delete (u_32b*)v; break;
           case U64B_TYPE: delete (u_64b*)v; break;
           case S8B_TYPE:  delete (s_8b*)v;  break;
           case S16B_TYPE: delete (s_16b*)v; break;
           case S32B_TYPE: delete (s_32b*)v; break;
           case S64B_TYPE: delete (s_64b*)v; break;
           case F32B_TYPE: delete (f_32b*)v; break;
           case F64B_TYPE: delete (f_64b*)v; break;
        }
     }
     // destroy the array:
     if (vctsize>0) {
        delete []vct;
        delete []types;
     }    
     if (strout!=NULL) delete []strout; 
}
// 
void DVector::create(vei_t nsize) 
{
     vctsize = nsize;
     vct = new void *[nsize];
     types = new char[nsize];
}
// allocate a comonent and fill it
void DVector::set_comp(f_64b value, char type, vei_t comp) 
{
     void *v=NULL;
     switch (type) {
        case U8B_TYPE:  v = new u_8b;  
                        *((u_8b*)v)  = (u_8b)value; 
                        ((u_8b*)vct[comp]) = ((u_8b*)v); 
                        break;
        case U16B_TYPE: v = new u_16b; 
                        *((u_16b*)v) = (u_16b)value; 
                        ((u_16b*)vct[comp]) = ((u_16b*)v);  
                        break;
        case U32B_TYPE: v = new u_32b; 
                        *((u_32b*)v) = (u_32b)value; 
                        ((u_32b*)vct[comp]) = ((u_32b*)v);  
                        break;
        case U64B_TYPE: v = new u_64b; 
                        *((u_64b*)v) = (u_64b)value; 
                        ((u_64b*)vct[comp]) = ((u_64b*)v); 
                        break;
        case S8B_TYPE:  v = new s_8b;  
                        *((s_8b*)v)  = (s_8b)value;  
                        ((s_8b*)vct[comp]) = ((s_8b*)v);   
                        break;
        case S16B_TYPE: v = new s_16b; 
                        *((s_16b*)v) = (s_16b)value; 
                        ((s_16b*)vct[comp]) = ((s_16b*)v);  
                        break;
        case S32B_TYPE: v = new s_32b; 
                        *((s_32b*)v) = (s_32b)value; 
                        ((s_32b*)vct[comp]) = ((s_32b*)v);  
                        break;   
        case S64B_TYPE: v = new s_64b; 
                        *((s_64b*)v) = (s_64b)value; 
                        ((s_64b*)vct[comp]) = ((s_64b*)v);  
                        break;
        case F32B_TYPE: v = new f_32b; 
                        *((f_32b*)v) = (f_32b)value; 
                        ((f_32b*)vct[comp]) = ((f_32b*)v);  
                        break;
        case F64B_TYPE: v = new f_64b;
                        *((f_64b*)v) = (f_64b)value; 
                        ((f_64b*)vct[comp]) = ((f_64b*)v);  
                        break; 
     }
     types[comp] = type;
}
// allocate a comonent and fill it
void DVector::set_comp(DataCell value, vei_t comp) 
{
     void *v=NULL;
     switch (value.get_type()) {
        case U8B_TYPE:  v = new u_8b;  
                        *((u_8b*)v)  = value.get_u8b(); 
                        ((u_8b*)vct[comp]) = ((u_8b*)v); 
                        break;
        case U16B_TYPE: v = new u_16b; 
                        *((u_16b*)v) = value.get_u16b(); 
                        ((u_16b*)vct[comp]) = ((u_16b*)v);  
                        break;
        case U32B_TYPE: v = new u_32b; 
                        *((u_32b*)v) = value.get_u32b(); 
                        ((u_32b*)vct[comp]) = ((u_32b*)v);  
                        break;
        case U64B_TYPE: v = new u_64b; 
                        *((u_64b*)v) = value.get_u64b(); 
                        ((u_64b*)vct[comp]) = ((u_64b*)v); 
                        break;
        case S8B_TYPE:  v = new s_8b;  
                        *((s_8b*)v)  = value.get_s8b();  
                        ((s_8b*)vct[comp]) = ((s_8b*)v);   
                        break;
        case S16B_TYPE: v = new s_16b; 
                        *((s_16b*)v) = value.get_s16b(); 
                        ((s_16b*)vct[comp]) = ((s_16b*)v);  
                        break;
        case S32B_TYPE: v = new s_32b; 
                        *((s_32b*)v) = value.get_s32b(); 
                        ((s_32b*)vct[comp]) = ((s_32b*)v);  
                        break;   
        case S64B_TYPE: v = new s_64b; 
                        *((s_64b*)v) = value.get_s64b(); 
                        ((s_64b*)vct[comp]) = ((s_64b*)v);  
                        break;
        case F32B_TYPE: v = new f_32b; 
                        *((f_32b*)v) = value.get_f32b(); 
                        ((f_32b*)vct[comp]) = ((f_32b*)v);  
                        break;
        case F64B_TYPE: v = new f_64b;
                        *((f_64b*)v) = value.get_f64b(); 
                        ((f_64b*)vct[comp]) = ((f_64b*)v);  
                        break; 
     }
     types[comp] = value.get_type();
}
// return a component as a 64 bit float
f_64b DVector::get_comp(vei_t comp) 
{
     switch (types[comp]) {
        case U8B_TYPE:  return *((u_8b*)vct[comp]); 
        case U16B_TYPE: return *((u_16b*)vct[comp]); 
        case U32B_TYPE: return *((u_32b*)vct[comp]); 
        case U64B_TYPE: return *((u_64b*)vct[comp]); 
        case S8B_TYPE:  return *((s_8b*)vct[comp]); 
        case S16B_TYPE: return *((s_16b*)vct[comp]); 
        case S32B_TYPE: return *((s_32b*)vct[comp]); 
        case S64B_TYPE: return *((s_64b*)vct[comp]); 
        case F32B_TYPE: return *((f_32b*)vct[comp]); 
        case F64B_TYPE: return *((f_64b*)vct[comp]); 
     }
     return 0;
}

char DVector::get_type(vei_t comp)
{
    return types[comp];
}
   
vei_t DVector::get_dim()
{
    return vctsize;
}

// Euclidean distance
oas_t DVector::dis_eucl(DVector& invec)
{
    oas_t sumeucl = 0;
    for (vei_t h=0; h<vctsize; h++) {
       sumeucl += pow((double)(get_comp(h)- invec.get_comp(h)), 2.0);
    }
    sumeucl = sqrt(sumeucl);
    return sumeucl;
}

// Manhattan distance
oas_t DVector::dis_manh(DVector& invec)
{
    oas_t summanh = 0;
    for (vei_t h=0; h<vctsize; h++) {
       summanh += abs(get_comp(h)- invec.get_comp(h));
    }
    return summanh;
}

// Chebychev distance
oas_t DVector::dis_cheb(DVector& invec)
{
    oas_t maxcheb = 0;
    for (vei_t h=0; h<vctsize; h++) {
       if(abs(get_comp(h)- invec.get_comp(h))>maxcheb) {
           maxcheb = abs(get_comp(h)- invec.get_comp(h));
       }
    }
    return maxcheb;
}

// Minkowsky distance
oas_t DVector::dis_mink(DVector& invec, vei_t exp)
{
    oas_t summink = 0;
    for (vei_t h=0; h<vctsize; h++) {
       summink += pow((double)(abs(get_comp(h)- invec.get_comp(h))),
                      (double)(exp));
    }
    summink = pow((double)(summink), (1/(double)(exp)));
    return summink;
}

char* DVector::to_string(void)
{
     vei_t iter = 0, iter2 = 0;
     if (strout!=NULL) delete []strout;
     strout = new char[vctsize*16];
     char strtmp[16];
     for (vei_t i=0; i<vctsize; i++) {
        iter = sprintf(strtmp, "%g\t", get_comp(i));
        for (vei_t j=0; j<16; j++) strout[j+iter2] = strtmp[j];
        iter2 += iter;
     }
     strout[iter2] = '\0';
     return strout;
}
