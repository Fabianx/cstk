/***************************************************************************
                            dvector.h  -  v.1.10
                            --------------------
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

#ifndef DVECTOR_H
#define DVECTOR_H

#include <stdio.h> // NULL, sprintf, ...
#include <math.h>
#include "vect_types.h"  // datacell
#include "datacell.h" 

#define abs(a) ((a)>0)?(a):-(a)

/**** DVector Definition ***************************************************
 * Use this class for input or data vectors for your algorithms. It        *
 * supports mixed types, and stores a pointer per component.               *
 ***************************************************************************/
class DVector {
 public:
   DVector();
   DVector(vei_t nsize);
   DVector(const DVector& vec);
   
   ~DVector(); 
   void create(vei_t nsize);
   void set_comp(f_64b value, char type, vei_t comp); 
   void set_comp(DataCell value, vei_t comp); // allocate a component
   f_64b get_comp(vei_t comp);       // return a component as a 64 bit float
   char  get_type(vei_t comp);                // return the type
   vei_t get_dim();                           // return the vector dimension
   oas_t dis_eucl(DVector& invec);            // Euclidean distance
   oas_t dis_manh(DVector& invec);            // Manhattan Distance
   oas_t dis_cheb(DVector& invec);            // Chebychev Distance
   oas_t dis_mink(DVector& invec, vei_t exp); // Minkowski Distance
   char* to_string(void);
   
   bool operator==(const DVector& vec);
   
   /*************************************************************************************
   * 	The equal operator is just a loop that reads the elements of the source vector	*
   * 	(vec) and writes the values in the destination vector (this).			*
   * 	The types are the same than the types from the input vector,			*
   * 	which made it possible to use the set function that has only one		*
   * 	vector element as input.							*
   **************************************************************************************/
   DVector& operator=(const DVector& vec); 
   
   /*************************************************************************************
   *	This operator was more difficult to implement, because there has an actual	*
   * 	calculation to be done. The get function returns only an element		*
   * 	as a double, which makes it not possible to use this method. If			*
   * 	the types of the two input vectors are the same, the type is taken		*
   * 	over for the new edition of the first input vector. Are the types		*
   * 	different, the get function is used and all the elements of the			*
   * 	saving vector are becoming the double type. There is not jet a			*
   * 	safety treatment if the vector types are the same, but the calculation		*
   * 	causes an overflow of the type.							*
   *************************************************************************************/
   DVector& operator+=(const DVector& vec);
   
   /*************************************************************************************	
   * 	This operator is the same as the previous operator, except that there is done	* 
   *	a minus instead of the plus.							*	
   **************************************************************************************/
   DVector& operator-=(const DVector& vec);
   
   /*************************************************************************************
   *	The friend specifier specifies that the operator is not inside the vector class,*
   * 	but it belongs to the class, it is a friend of it. Friend Methods and operators	*
   * 	can use private declarations of the class. The friend specification is needed,	*
   * 	because with it the definition of operators with two input variables is first	*
   * 	possible. The implementation is nearly the same as the '+=' operator despite	*
   * 	of the destination vector where the results are saved. This vector has to be	*
   * 	constructed in the operator itself instead of returning the first input vector	*
   * 	with 'this'. It works the same way as the according mathematic operator, each	*
   * 	element of the first vector is added to the element of the second vector that	*
   * 	stands on the same position.							*
   **************************************************************************************/
   friend DVector operator+(const DVector& vec1, const DVector& vec2);
   
   /*************************************************************************************
   *	This operator is the same as the previous operator, except that there is done	*
   *	a minus instead of the plus.							*
   **************************************************************************************/
   friend DVector operator-(const DVector& vec1, const DVector& vec2);
   
   /*************************************************************************************
   * 	The value is multiplied with each element of the input vector and the result is	*
   * 	saved in a newly constructed result vector. Since the type of the value has to	*
   * 	be known in advance, the biggest type (double) had to be chosen. With this fact	*
   * 	the get function could be used within a loop. The operator returns again a	*
   * 	vector constructed inside the operator.						*
   **************************************************************************************/
   friend oas_t operator*(const DVector& vec1, const DVector& vec2);
   
   /*************************************************************************************
   *	This is the Vector multiplication operator according to the math standard, that	*
   *	multiplies each element and sums up the result of the multiplications. Since the*
   *	type of the multiplications result is not known in advance , the biggest type 	*
   *	(double) is chosen. With this abstraction the get function can be 		*
   *	used within a simple loop.							*	
   **************************************************************************************/
   friend DVector operator*(const oas_t val, const DVector& vec);
 
 private:
   DVector& set(const DVector& vec, vei_t i);
   friend oas_t get(const DVector& vec, vei_t i);
 
   void **vct;                                // the pointer array
   char *types;                               // the types for each pointer
   vei_t vctsize;
   char *strout;                              // pointer to outpout buffer
};

#endif
