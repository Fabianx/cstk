//Basics
#ifndef MATLAB_CMEANS_H 
#define MATLAB_CMEANS_H

#include "cmeans.h"

class Matlab_CMeans : public CMeans
{
	public:
		Matlab_CMeans(vector<DVector>* data, vei_t cluster_n, CMeansOptions* algorithm_options = NULL);

		virtual void run();
};

#endif
	
