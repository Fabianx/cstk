#include "cmeans.h"

CMeans::CMeans(list<DVector>* data, vei_t cluster_n, CMeansOptions* algorithm_options) 
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
		options->exp = CMEANS_DEFAULT_EXP;
		options->maxIter = CMEANS_DEFAULT_MAXITER;
		options->epsilon = CMEANS_DEFAULT_EPSILON;
		options->displayInfo = CMEANS_DEFAULT_DISPLAY_INFO;
	}
}

CMeans::~CMeans()
{
	if (free_input)
		delete input;
	if (free_options)
		delete options;
}

void CMeans::readInputVector(DVector vec)
{
	input->push_back(vec);
}
