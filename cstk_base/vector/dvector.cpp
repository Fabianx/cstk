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

DVector::DVector(const DVector& vec) 
{
	strout = NULL;
	create(vec.vctsize);
	for (vei_t i=0; i<vec.vctsize; i++)
		set(vec, i);
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
     vct = new void*[nsize];
     for (vei_t i=0; i<nsize; i++)
     	vct[i]=NULL;
     types = new char[nsize];
}

// allocate a comonent and fill it
void DVector::set_comp(f_64b value, char type, vei_t comp) 
{
     void *v;
     v = vct[comp];
     if (v!=NULL)
        switch (types[comp]){
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
     vct[comp]=NULL;
     switch (type) {
        case U8B_TYPE:  v = new u_8b;  
                        *((u_8b*)v)  = (u_8b)value; 
                        break;
        case U16B_TYPE: v = new u_16b; 
                        *((u_16b*)v) = (u_16b)value; 
                        break;
        case U32B_TYPE: v = new u_32b; 
                        *((u_32b*)v) = (u_32b)value; 
                        break;
        case U64B_TYPE: v = new u_64b; 
                        *((u_64b*)v) = (u_64b)value; 
                        break;
        case S8B_TYPE:  v = new s_8b;  
                        *((s_8b*)v)  = (s_8b)value;  
                        break;
        case S16B_TYPE: v = new s_16b; 
                        *((s_16b*)v) = (s_16b)value; 
                        break;
        case S32B_TYPE: v = new s_32b; 
                        *((s_32b*)v) = (s_32b)value; 
                        break;   
        case S64B_TYPE: v = new s_64b; 
                        *((s_64b*)v) = (s_64b)value; 
                        break;
        case F32B_TYPE: v = new f_32b; 
                        *((f_32b*)v) = (f_32b)value; 
                        break;
        case F64B_TYPE: v = new f_64b;
                        *((f_64b*)v) = (f_64b)value; 
                        break; 
     }
     vct[comp] = v;
     types[comp] = type;
}
// allocate a comonent and fill it
void DVector::set_comp(DataCell value, vei_t comp) 
{
     void *v=NULL;
     v = vct[comp];
     if (v!=NULL)
        switch (types[comp]){
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
     vct[comp]=NULL;
     switch (value.get_type()) {
        case U8B_TYPE:  v = new u_8b;  
                        *((u_8b*)v)  = value.get_u8b(); 
                        break;
        case U16B_TYPE: v = new u_16b; 
                        *((u_16b*)v) = value.get_u16b(); 
                        break;
        case U32B_TYPE: v = new u_32b; 
                        *((u_32b*)v) = value.get_u32b(); 
                        break;
        case U64B_TYPE: v = new u_64b; 
                        *((u_64b*)v) = value.get_u64b(); 
                        break;
        case S8B_TYPE:  v = new s_8b;  
                        *((s_8b*)v)  = value.get_s8b();  
                        break;
        case S16B_TYPE: v = new s_16b; 
                        *((s_16b*)v) = value.get_s16b(); 
                        break;
        case S32B_TYPE: v = new s_32b; 
                        *((s_32b*)v) = value.get_s32b(); 
                        break;   
        case S64B_TYPE: v = new s_64b; 
                        *((s_64b*)v) = value.get_s64b(); 
                        break;
        case F32B_TYPE: v = new f_32b; 
                        *((f_32b*)v) = value.get_f32b(); 
                        break;
        case F64B_TYPE: v = new f_64b;
                        *((f_64b*)v) = value.get_f64b(); 
                        break; 
     }
     vct[comp]=v;
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
     strout = new char[vctsize*20];
     char strtmp[20];
     for (vei_t i=0; i<vctsize; i++) {
        iter = sprintf(strtmp, "%g ", get_comp(i));
        for (vei_t j=0; j<20; j++) strout[j+iter2] = strtmp[j];
        iter2 += iter;
     }
     strout[iter2] = '\0';
     return strout;
}

DVector& DVector::set(const DVector& vec, vei_t i)
{
	switch (vec.types[i]) 
	{
        	case U8B_TYPE:  set_comp(*((u_8b*)vec.vct[i]),U8B_TYPE,i); break;
        	case U16B_TYPE: set_comp(*((u_16b*)vec.vct[i]),U16B_TYPE,i); break;
        	case U32B_TYPE: set_comp(*((u_32b*)vec.vct[i]),U32B_TYPE,i); break;
        	case U64B_TYPE: set_comp(*((u_64b*)vec.vct[i]),U64B_TYPE,i); break;
        	case S8B_TYPE:  set_comp(*((s_8b*)vec.vct[i]),S8B_TYPE,i); break;
        	case S16B_TYPE: set_comp(*((s_16b*)vec.vct[i]),S16B_TYPE,i); break;
        	case S32B_TYPE: set_comp(*((s_32b*)vec.vct[i]),S32B_TYPE,i); break;
        	case S64B_TYPE: set_comp(*((s_64b*)vec.vct[i]),S64B_TYPE,i); break;
        	case F32B_TYPE: set_comp(*((f_32b*)vec.vct[i]),F32B_TYPE,i); break;
        	case F64B_TYPE: set_comp(*((f_64b*)vec.vct[i]),F64B_TYPE,i); break;
     	}
	return *this;
}

oas_t get(const DVector& vec, vei_t i)
{
	switch (vec.types[i]) 
	{
        	case U8B_TYPE:  return (f_64b)(*((u_8b*)vec.vct[i])); 
        	case U16B_TYPE: return (f_64b)(*((u_16b*)vec.vct[i])); 
        	case U32B_TYPE: return (f_64b)(*((u_32b*)vec.vct[i])); 
        	case U64B_TYPE: return (f_64b)(*((u_64b*)vec.vct[i])); 
        	case S8B_TYPE:  return (f_64b)(*((s_8b*)vec.vct[i])); 
        	case S16B_TYPE: return (f_64b)(*((s_16b*)vec.vct[i])); 
        	case S32B_TYPE: return (f_64b)(*((s_32b*)vec.vct[i])); 
        	case S64B_TYPE: return (f_64b)(*((s_64b*)vec.vct[i]));
        	case F32B_TYPE: return (f_64b)(*((f_32b*)vec.vct[i]));
        	case F64B_TYPE: return (f_64b)(*((f_64b*)vec.vct[i])); 
     	}
	return 0.0;
}

DVector& DVector::operator=(const DVector& vec) 
{
	strout = NULL;
	create(vec.vctsize);
	for (vei_t i=0; i<vec.vctsize; i++)
	{
		set(vec, i);
	}
	return *this;
}

bool DVector::operator==(const DVector& vec) 
{
	for (vei_t i=0; i<vec.vctsize; i++)
	{
		if (get(vec, i)!=get(*this, i))
			return false;
	}
	return true;
}

DVector& DVector::operator+=(const DVector& vec)
{
	if (vec.vctsize == vctsize)
	{
		for (vei_t i=0; i<vec.vctsize; i++)
		{
			if (vec.types[i] == types[i])
			{
		 		switch (vec.types[i]) 
				{
					case U8B_TYPE:  set_comp((*((u_8b*)vec.vct[i]))+(*((u_8b*)vct[i])),U8B_TYPE,i); break;
					case U16B_TYPE: set_comp((*((u_16b*)vec.vct[i]))+(*((u_16b*)vct[i])),U16B_TYPE,i); break;
					case U32B_TYPE: set_comp((*((u_32b*)vec.vct[i]))+(*((u_32b*)vct[i])),U32B_TYPE,i); break;
					case U64B_TYPE: set_comp((*((u_64b*)vec.vct[i]))+(*((u_64b*)vct[i])),U64B_TYPE,i); break;
					case S8B_TYPE:  set_comp((*((s_8b*)vec.vct[i]))+(*((s_8b*)vct[i])),S8B_TYPE,i); break;
					case S16B_TYPE: set_comp((*((s_16b*)vec.vct[i]))+(*((s_16b*)vct[i])),S16B_TYPE,i); break;
					case S32B_TYPE: set_comp((*((s_32b*)vec.vct[i]))+(*((s_32b*)vct[i])),S32B_TYPE,i); break;
					case S64B_TYPE: set_comp((*((s_64b*)vec.vct[i]))+(*((s_64b*)vct[i])),S64B_TYPE,i); break;
					case F32B_TYPE: set_comp((*((f_32b*)vec.vct[i]))+(*((f_32b*)vct[i])),F32B_TYPE,i); break;
					case F64B_TYPE: set_comp((*((f_64b*)vec.vct[i]))+(*((f_64b*)vct[i])),F64B_TYPE,i); break;
				}
			}
			else
				set_comp((get(vec,i)+get(*this,i)), F64B_TYPE, i);
		}
	}
	return *this;
}

DVector& DVector::operator-=(const DVector& vec)
{
	if (vec.vctsize == vctsize)
	{
		for (vei_t i=0; i<vec.vctsize; i++)
		{
			if (vec.types[i] == types[i])
			{
		 		switch (vec.types[i]) 
				{
					case U8B_TYPE:  set_comp((*((u_8b*)vec.vct[i]))-(*((u_8b*)vct[i])),U8B_TYPE,i); break;
					case U16B_TYPE: set_comp((*((u_16b*)vec.vct[i]))-(*((u_16b*)vct[i])),U16B_TYPE,i); break;
					case U32B_TYPE: set_comp((*((u_32b*)vec.vct[i]))-(*((u_32b*)vct[i])),U32B_TYPE,i); break;
					case U64B_TYPE: set_comp((*((u_64b*)vec.vct[i]))-(*((u_64b*)vct[i])),U64B_TYPE,i); break;
					case S8B_TYPE:  set_comp((*((s_8b*)vec.vct[i]))-(*((s_8b*)vct[i])),S8B_TYPE,i); break;
					case S16B_TYPE: set_comp((*((s_16b*)vec.vct[i]))-(*((s_16b*)vct[i])),S16B_TYPE,i); break;
					case S32B_TYPE: set_comp((*((s_32b*)vec.vct[i]))-(*((s_32b*)vct[i])),S32B_TYPE,i); break;
					case S64B_TYPE: set_comp((*((s_64b*)vec.vct[i]))-(*((s_64b*)vct[i])),S64B_TYPE,i); break;
					case F32B_TYPE: set_comp((*((f_32b*)vec.vct[i]))-(*((f_32b*)vct[i])),F32B_TYPE,i); break;
					case F64B_TYPE: set_comp((*((f_64b*)vec.vct[i]))-(*((f_64b*)vct[i])),F64B_TYPE,i); break;
				}
			}
			else
				set_comp((get(vec,i)-get(*this,i)), F64B_TYPE, i);
		}
	}
	return *this;
}

DVector operator+(const DVector& vec1, const DVector& vec2)
{
   	DVector vec;
	if (vec1.vctsize == vec1.vctsize)
	{
		vec.create(vec1.vctsize);
		for (vei_t i=0; i<vec1.vctsize; i++)
		{
			if (vec1.types[i] == vec2.types[i])
			{
		 		switch (vec1.types[i]) 
				{
					case U8B_TYPE:  vec.set_comp((*((u_8b*)vec1.vct[i]))+(*((u_8b*)vec2.vct[i])),U8B_TYPE,i); break;
					case U16B_TYPE: vec.set_comp((*((u_16b*)vec1.vct[i]))+(*((u_16b*)vec2.vct[i])),U16B_TYPE,i); break;
					case U32B_TYPE: vec.set_comp((*((u_32b*)vec1.vct[i]))+(*((u_32b*)vec2.vct[i])),U32B_TYPE,i); break;
					case U64B_TYPE: vec.set_comp((*((u_64b*)vec1.vct[i]))+(*((u_64b*)vec2.vct[i])),U64B_TYPE,i); break;
					case S8B_TYPE:  vec.set_comp((*((s_8b*)vec1.vct[i]))+(*((s_8b*)vec2.vct[i])),S8B_TYPE,i); break;
					case S16B_TYPE: vec.set_comp((*((s_16b*)vec1.vct[i]))+(*((s_16b*)vec2.vct[i])),S16B_TYPE,i); break;
					case S32B_TYPE: vec.set_comp((*((s_32b*)vec1.vct[i]))+(*((s_32b*)vec2.vct[i])),S32B_TYPE,i); break;
					case S64B_TYPE: vec.set_comp((*((s_64b*)vec1.vct[i]))+(*((s_64b*)vec2.vct[i])),S64B_TYPE,i); break;
					case F32B_TYPE: vec.set_comp((*((f_32b*)vec1.vct[i]))+(*((f_32b*)vec2.vct[i])),F32B_TYPE,i); break;
					case F64B_TYPE: vec.set_comp((*((f_64b*)vec1.vct[i]))+(*((f_64b*)vec2.vct[i])),F64B_TYPE,i); break;
				}
			}
			else
				vec.set_comp(get(vec1,i)+get(vec2,i), F64B_TYPE, i);
		}
	}
	return vec;
}
   
DVector operator-(const DVector& vec1, const DVector& vec2)
{
   	DVector vec;
	if (vec1.vctsize == vec1.vctsize)
	{
		vec.create(vec1.vctsize);
		for (vei_t i=0; i<vec1.vctsize; i++)
		{
			if (vec1.types[i] == vec2.types[i])
			{
		 		switch (vec1.types[i]) 
				{
					case U8B_TYPE:  vec.set_comp((*((u_8b*)vec1.vct[i]))-(*((u_8b*)vec2.vct[i])),U8B_TYPE,i); break;
					case U16B_TYPE: vec.set_comp((*((u_16b*)vec1.vct[i]))-(*((u_16b*)vec2.vct[i])),U16B_TYPE,i); break;
					case U32B_TYPE: vec.set_comp((*((u_32b*)vec1.vct[i]))-(*((u_32b*)vec2.vct[i])),U32B_TYPE,i); break;
					case U64B_TYPE: vec.set_comp((*((u_64b*)vec1.vct[i]))-(*((u_64b*)vec2.vct[i])),U64B_TYPE,i); break;
					case S8B_TYPE:  vec.set_comp((*((s_8b*)vec1.vct[i]))-(*((s_8b*)vec2.vct[i])),S8B_TYPE,i); break;
					case S16B_TYPE: vec.set_comp((*((s_16b*)vec1.vct[i]))-(*((s_16b*)vec2.vct[i])),S16B_TYPE,i); break;
					case S32B_TYPE: vec.set_comp((*((s_32b*)vec1.vct[i]))-(*((s_32b*)vec2.vct[i])),S32B_TYPE,i); break;
					case S64B_TYPE: vec.set_comp((*((s_64b*)vec1.vct[i]))-(*((s_64b*)vec2.vct[i])),S64B_TYPE,i); break;
					case F32B_TYPE: vec.set_comp((*((f_32b*)vec1.vct[i]))-(*((f_32b*)vec2.vct[i])),F32B_TYPE,i); break;
					case F64B_TYPE: vec.set_comp((*((f_64b*)vec1.vct[i]))-(*((f_64b*)vec2.vct[i])),F64B_TYPE,i); break;
				}
			}
			else
				vec.set_comp(get(vec1,i)-get(vec2,i), F64B_TYPE, i);
		}
	}
	return vec;
}

oas_t operator*(const DVector& vec1, const DVector& vec2)
{
	oas_t sum = 0.0;
	
	if (vec1.vctsize == vec2.vctsize)
		for (vei_t i=0; i<vec1.vctsize; i++)
			sum += get(vec1,i) * get(vec2,i);
			
	return sum;
}
   
DVector operator*(const oas_t val, const DVector& vec1)
{
	DVector vec;
	vec.create(vec1.vctsize);
	for (vei_t i=0; i<vec1.vctsize; i++)
		vec.set_comp(val * get(vec1,i), F64B_TYPE, i);

	return vec;
}   
   
   
   
   
   
   
   
   
   
   
   
   
