/***************************************************************************
                             bpnn.cpp  -  v.0.1
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

#include "bpnn.h"

BPNN::BPNN(uint newnum_inputs, uint newnum_outputs, uint newnum_layers)
{
  num_inputs = newnum_inputs;
  inputs = new float[num_inputs];
  num_outputs = newnum_outputs;
  outputs = new float[num_outputs];
  num_layers = newnum_layers;
  layers = new Layer[num_layers];
}

BPNN::~BPNN()
{
  delete[] inputs;
  delete[] outputs;
  delete[] layers;
}
  
int BPNN::add_layer(uint iter, uint num_neurons, uint num_weights)
{
  uint i,j;
  layers[iter].create(num_neurons, num_weights);
  for (i=0; i<num_neurons;i++) {
     layers[iter].neuron[i] = 0.0;
     for (j=0; j<num_weights;j++) {
       _w(i,j,iter) = 0.0;
     }
  }   
  return 0;  
}

int BPNN::print_layer(uint iter)
{
  uint i,j;
  printf("Layer %i:\n\r", iter);
  for (i=0; i<layers[iter].size;i++) {
     printf("  %f (", layers[iter].neuron[i]);
     for (j=0; j<layers[iter].num_weights;j++) {
       printf("%f,", _w(i,j,iter) );
     }
     printf(")\n\r");
  }
  return 0;
}

int BPNN::random_init()
{
  uint i,j,k;
  for (k=0; k<num_layers;k++) {
   for (i=0; i<layers[k].size;i++) {
     for (j=0; j<layers[k].num_weights;j++) {
       _w(i,j,k) = ((2.0*rand()/(RAND_MAX+1.0))-1);
     }
   }
  }
  return 0;
}

int BPNN::feed()
{
  uint i,j,k;
  float sum;
  // start with the first layer taking data from the input vector:        
   for (i=0;i<layers[0].size;i++) {
      sum =0;
      for (j=0;j<layers[0].num_weights;j++) {
        sum += _w(i,j,0)*inputs[j];
      }
      // symmetric sigmoid:
       sum = sigmoid(sum);
      layers[0].neuron[i] = sum;
      layers[0].err[i] = 0.0;
   }
  // and propagate through all the layers:
   for (k=1; k<num_layers; k++) {
     for (i=0;i<layers[k].size;i++) {
        sum = 0;
        for (j=0;j<layers[k].num_weights;j++) {
          sum += _w(i,j,k)*layers[k-1].neuron[j];
        }
        // symmetric sigmoid:
         sum = sigmoid(sum);
        layers[k].neuron[i] = sum;
        layers[k].err[i] = 0.0;
     }
   }   
  return 0;
}

/* calculate the sum squared error for the entire net for the current input and current desired output */
float BPNN::calc_sserror()
{
  uint i;
  float sum=0;
  feed(); // feed input to the net
  for (i=0; i<num_outputs;i++) {    // targets      real outputs:
     layers[num_layers-1].err[i] += outputs[i] - layers[num_layers-1].neuron[i];
     sum += layers[num_layers-1].err[i];      
  }
  return sum;
}

/* reset the sum squared error */
float BPNN::reset_sserror()
{
  uint i;
  for (i=0; i<num_outputs;i++) {             
     layers[num_layers-1].err[i] = 0.0;
  }
  return 0;
}

int BPNN::backprop(float lr, float mom)
{
  uint i,j,k;

  // clear all errors but the top ones.. 
  for (k=0; k<num_layers-1; k++) {                 // go through the layers (output -> input)
     for (j=0; j<layers[k].size; j++) {            // browse through neurons (0 -> n) on current layer
             layers[k].err[j] = 0.0;
     }
  }
  // trickle errors down..  
  for (k=num_layers-1; k>0; k--) {                 // go through the layers (output -> input)    
     for (j=0; j<layers[k].size; j++) {            // browse through neurons (0 -> n) on current layer
         for (i=0; i<layers[k-1].size; i++) {      // browse through neurons (0 -> m) on previous layer
             layers[k-1].err[i] += layers[k].err[j]*sigmoid_diff(layers[k].neuron[j])*_w(j,i,k);
         }
     }
  }
  // update weight, backpropagating down..
  for (k=num_layers-1; k>0; k--) {                 // go through the layers (output -> input) 
     for (j=0; j<layers[k].size; j++) {            // browse through neurons (0 -> n) on current layer
         for (i=0; i<layers[k-1].size; i++) {      // browse through neurons (0 -> m) on previous layer
              layers[k].delta[j] = lr*layers[k].err[j]*sigmoid_diff(layers[k].neuron[j])*layers[k-1].neuron[i]
                                   + mom*layers[k].delta[j]; // momentum term
              layers[k].weight[j*layers[k].num_weights+i] += layers[k].delta[j];
         }
     }
  }
  // and for the weights to the input layer:
  for (j=0; j<layers[0].size; j++) {            // browse through neurons (0 -> n) on first hidden layer
     for (i=0; i<num_inputs; i++) {             // browse through neurons (0 -> m) on input layer
         layers[0].delta[j] = lr*layers[0].err[j]*sigmoid_diff(layers[0].neuron[j])*inputs[i]
                              + mom*layers[0].delta[j];
         layers[k].weight[j*layers[0].num_weights+i] += layers[0].delta[j];
      }
  }  
   
  return 0;
}

float BPNN::sigmoid(float val)
{ 
  return (tanh(val));
  //return 1/(1+exp(-val));
}

float BPNN::sigmoid_diff(float val)
{ 
  return (1-tanh(val)*tanh(val));
  //return (val*(1-val));
}

uint BPNN::train_from_file(char* filename, float threshold, uint maxiter, float lr, float mom)
{
  uint i=0,j;
  float tot_error=threshold+1.0;
  FILE *fp;

  if ( (fp = fopen(filename,"r"))!=NULL)
  {
    while (tot_error > threshold) { 
      tot_error = 0;
      while (!feof(fp) && (i<maxiter)) {
         // fill in the inputs:
         for (j=0; j<num_inputs; j++)  fscanf(fp,"%f", &inputs[j]);
         // fill in the outputs:
         for (j=0; j<num_outputs; j++) fscanf(fp,"%f", &outputs[j]);
         // calulate:
         tot_error += calc_sserror();
         backprop(lr);
         i++;
      }
      if (feof(fp)) {
         fp = freopen(filename,"r",fp);
         tot_error=threshold+1.0;
      } 
    }
    fclose(fp);
  }
  return i;
}
 
float BPNN::test_from_file(char* filename, short int print)
{
  uint i,j;
  float tot_error=0;
  FILE *fp;

  if ( (fp = fopen(filename,"r"))!=NULL)
  {
      while (!feof(fp)) {
         // fill in the inputs:
         for (j=0; j<num_inputs; j++)  fscanf(fp,"%f", &inputs[j]);
         // fill in the outputs:
         for (j=0; j<num_outputs; j++) fscanf(fp,"%f", &outputs[j]);
         // calulate:
         if (!feof(fp)) {
           feed();
           tot_error += calc_sserror();
           if (print) {
             printf(" res = ");
             for (i=0; i<layers[num_layers-1].size;i++) {
                printf("  %1.3f ", layers[num_layers-1].neuron[i]);
             }
             printf("(trg = ");
             for (i=0; i<num_outputs;i++) {
                printf("  %1.3f ", outputs[i]);
             }
             printf(")\n\r");            
           }
         }
      }
      fclose(fp);
  }

  return tot_error;
}

