/***************************************************************************
                            bpnn_t.cpp  -  v.1.00
                            ---------------------
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

 #include "algorithms/bpnn/bpnn.h"

 int main(void) {

  BPNN bp(5,3,2); // 5 inputs, 4 outputs, and 2 layers (plus 1 input)
 
  printf("testing backpropagation routines:\n\r");

  bp.add_layer(0, 4, 5);  
  bp.add_layer(1, 3, 4);  
  bp.random_init();
  bp.train_from_file("nntest.txt", 0.05, 10000, 0.05, 0.0);
  bp.test_from_file("nntest.txt", 1);

  printf("\n\rdone.\n\r");   
 }
 
