/***************************************************************************
                              bpnn.h  -  v.0.1
                             ------------------
    begin                : Tue Sep 30 2003
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

#ifndef KBPNN_H
#define KBPNN_H

#ifndef uint
#define uint unsigned int
#endif

#ifndef sint
#define sint signed int
#endif

#define _w(i,j,k) layers[(k)].weight[((i)*layers[(k)].num_weights+(j))]

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

class Layer {
public:
  Layer() {;}; 
  ~Layer() {
    if (neuron!=NULL) delete[] neuron;
    if (err!=NULL) delete[] err;
    if (delta!=NULL) delete[] delta;
    if (weight!=NULL) delete[] weight;
  };
  uint create(uint newsize, uint newweights) {
     size = newsize; num_weights = newweights;
     neuron = new float[size];
     err = new float[size];
     delta = new float[size];
     weight = new float[num_weights*size];
     return 0;
  };                       
  uint size;     // number of neurons
  uint num_weights;  // number of weights per neuron
  float *neuron;     // neuron i
  float *weight;     // weights arriving at neuron i
  float *delta;      // previous delta's (for momentum)
  float *err;        // error at neuron i 
};

/**Backprop Neural Net without too many floats
  *@author Kristof Van Laerhoven
  */

class BPNN {
public: 
  BPNN(uint newnum_inputs, uint newnum_outputs, uint newnum_layers);
  ~BPNN();

  int add_layer(uint iter, uint num_neurons, uint num_weigths);
  int print_layer(uint iter);
  int random_init();
  int feed();
  float calc_sserror();
  float reset_sserror();
  int backprop(float lr, float mom=0.0);
  float sigmoid(float val);
  float sigmoid_diff(float val);
  uint train_from_file(char* filename, float threshold, 
                       uint maxiter, float lr, float mom=0.0);
  float test_from_file(char* filename, short int print=1);

  float *inputs;
  uint num_inputs; 
  float *outputs;
  uint num_outputs;

private:
  Layer *layers;
  uint num_layers;
  
};

#endif
