//Basics
#ifndef CMEANS_H
#define CMEANS_H

#include <cstdlib>
#include <ctime>
#include <vector>
#include <list>

using namespace std;

#include "cstk_base/types.h"
#include "cstk_base/vector/dvector.h"
#include "cstk_base/matrix/dvectormatrix.h"

#define CMEANS_DEFAULT_EXP		2.0
#define CMEANS_DEFAULT_MAXITER		100
#define CMEANS_DEFAULT_EPSILON		1e-5
#define CMEANS_DEFAULT_DISPLAY_INFO	false

class CMeansOptions
{
	public:
		f_64b exp; /**< exponent for the partition matrix U */
		vei_t maxIter; /**< maximum number of iterations */
		f_64b epsilon; /**< minimum amount of improvement */
		bool displayInfo; /**< info display during iteration */
};

class CMeans 
{
	public:
		CMeans(list<DVector>* data, vei_t cluster_n, CMeansOptions* algorithm_options = NULL);
		
		~CMeans();
		
		virtual void run()=0; /**< process the c-means algorithm */

		// Output parameters
		
		vector<vei_t> objFcn; /**< The objective function */
		vector<DVector> clusterCenters; /**< The found cluster centers */
		double* membershiptable_U; /**< The membershiptable U */

		// Online version

		void readInputVector(DVector vec);

	protected:
		list<DVector>* input;
		vei_t anzcluster;
		CMeansOptions* options;
	private:
		bool free_input;
		bool free_options;
};



#endif
