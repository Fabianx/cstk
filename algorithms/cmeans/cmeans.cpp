#include "cmeans.h"

CMeansOptions::CMeansOptions()
{
		// Set sane defaults (matlab)
		exp = CMEANS_DEFAULT_EXP;
		maxIter = CMEANS_DEFAULT_MAXITER;
		epsilon = CMEANS_DEFAULT_EPSILON;
		displayInfo = CMEANS_DEFAULT_DISPLAY_INFO;
}

static CMeansOptions DefaultCMeansOptions;

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

	if (algorithm_options == NULL)
		options = &DefaultCMeansOptions;
}

CMeans::~CMeans()
{
	if (free_input)
		delete input;
}

void CMeans::readInputVector(DVector vec)
{
	input->push_back(vec);
}
