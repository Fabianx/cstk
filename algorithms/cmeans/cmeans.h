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
		CMeans(list<DVector>* data, vei_t cluster_n, CMeansOptions* algorithm_options = NULL) 
		{
			input=data;

			free_input=false;
			if (input == NULL)
			{
				input=new list<DVector>;
				free_input=true;
			}

			anzcluster=cluster_n;

			clusterCenters.resize(cluster_n);

			options = algorithm_options;

			free_options=false;

			if (!options)
			{
				options = new CMeansOptions();
				free_options=true;

				// Set sane defaults (matlab)
				options->exp = (vei_t)2.0;
				options->maxIter = 100;
				options->epsilon = (vei_t)1e-5;
				options->displayInfo = true;
			}
		}
		
		~CMeans()
		{
			if (free_input)
				delete input;
			if (free_options)
				delete options;
		}	
		
		virtual void run()=0; /**< process the c-means algorithm */

		// Output parameters
		
		vector<vei_t> objFcn; /**< The objective function */
		vector<DVector> clusterCenters; /**< The found cluster centers */
		double* membershiptable_U; /**< The membershiptable U */

		// Online version

		void readInputVector(DVector vec)
		{
			input->push_back(vec);
		}

	protected:
		list<DVector>* input;
		vei_t anzcluster;
		CMeansOptions* options;
	private:
		bool free_input;
		bool free_options;
};



#endif
