//Basics
#ifndef ABSTRACT_CMEANS_H
#define ABSTRACT_CMEANS_H

#include <cstdlib>
#include <ctime>
#include <vector>
#include <list>

#include "cstk_base/types.h"
#include "cstk_base/vector/dvector.h" 
#include "cstk_base/matrix/dvectormatrix.h"

class CMeansOptions
{
	public:
		vei_t exp; /**< exponent for the partition matrix U */
		vei_t maxIter; /**< maximum number of iterations */
		vei_t epsilon; /**< minimum amount of improvement */
		bool displayInfo; /**< info display during iteration */
}

#if 0
class CSTK_CMeansOptions : CMeansOptions
{
	public:
		f_64b fuzzyfyer; /**< fuzzyfyer */
		vei_t seldist; /**< selected Distance function */
}
#endif

class AbstractCMeans 
{
	public:
		AbstractCMeans(list<DVector>* data, vei_t cluster_n)
		{
			input=data;
			if (input == NULL)
				input=new list<DVector>;
			anzcluster=cluster_n;

			clusterCenters.resize(cluster_n);

			// Set sane defaults (matlab)
			options.exp = 2.0;
			options.maxIter = 100;
			options.epsilon = 1e-5;
			options.displayInfo = true;
		}

		AbstractCMeans(list<DVector>* data, vei_t cluster_n, CMeansOptions algorithm_options); 
		{
			AbstractCMeans(data, cluster_n);
			options = algorithm_options;
		}
		
		virtual void run()=0; /**< process the c-means algorithm */

		// Output parameters
		
		vector<vei_t> objFcn; /**< The objective function */
		vector<DVector> clusterCenters; /**< The found cluster centers */
		double* membershiptable_U; /**< The membershiptable U */

		// Online version

		void readInputVector(DVector& vec)
		{
			input->push_back(vec);
		}

	private:
		list<DVector>* input;
		vei_t anzcluster;
		CMeansOptions options;
};

#endif
