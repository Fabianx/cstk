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
  unsigned int create(unsigned int newsize, unsigned int newweights) {
     size = newsize; num_weights = newweights;
     neuron = new float[size];
     err = new float[size];
     delta = new float[size];
     weight = new float[num_weights*size];
     return 0;
  };                       
  unsigned int size;     // number of neurons
  unsigned int num_weights;  // number of weights per neuron
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
  BPNN(unsigned int newnum_inputs, unsigned int newnum_outputs, unsigned int newnum_layers);
  ~BPNN();

  int add_layer(unsigned int iter, unsigned int num_neurons, unsigned int num_weigths);
  int print_layer(unsigned int iter);
  int random_init();
  int feed();
  float calc_sserror();
  float reset_sserror();
  int backprop(float lr, float mom=0.0);
  float sigmoid(float val);
  float sigmoid_diff(float val);
  unsigned int train_from_file(char* filename, float threshold, 
                       unsigned int maxiter, float lr, float mom=0.0);
  float test_from_file(char* filename, short int print=1);

  float *inputs;
  unsigned int num_inputs; 
  float *outputs;
  unsigned int num_outputs;

private:
  Layer *layers;
  unsigned int num_layers;
  
};

#endif
