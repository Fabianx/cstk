#include "cmeans_matlab.h"
#include "engine.h"

//#define DEBUG 1
//
static void engPutDoubleValue(Engine* ep, const char* name, double val)
{
	mxArray* mx_val = mxCreateDoubleMatrix(1, 1, mxREAL);
	memcpy((char *) mxGetPr(mx_val), (char *) &val, sizeof(double));
	engPutVariable(ep, name, mx_val);
	mxDestroyArray(mx_val);
}

Matlab_CMeans::Matlab_CMeans(vector<DVector>* data, vei_t cluster_n, CMeansOptions* algorithm_options) : CMeans(data, cluster_n, algorithm_options)
{
}

void Matlab_CMeans::run()
{
#ifdef DEBUG2
	printf("It works! Input Vectors:\n");

	for (int i = 0; i < input.size(); i++)
		printf("%d: %s, ", i, input[i].to_string());
	printf("\n\n");
#endif

	// ---------------------
	
	Engine *ep;
	mxArray *mx_data = NULL;
	mxArray *mx_anzcluster = NULL;

	/*
	 * Start the MATLAB engine 
	 */
	if (!(ep = engOpen(NULL))) {
		printf("Matlab_CMeans: Error: Can't start MATLAB engine\n");
		return;
	}

	/* 
	 * Now copy our data to matlab data structures
	 *
	 * Note: Matlab uses column first then row to store data
	 *       structures. (FORTRAN)
	 */

	// Copy input vectors

	mx_data = mxCreateDoubleMatrix(input.size(), input[0].get_dim(), mxREAL);
	double* dataptr = (double*)mxGetPr(mx_data);

	int i = 0;
	
	for(vei_t d=0; d < input[0].get_dim(); d++)
	{
		for(vei_t h=0; h < input.size(); h++)
		{
			//printf("%d = %f\n",i,input[h].get_comp(d));
			dataptr[i++]=input[h].get_comp(d);
		}
	}
	
	/*
	 * And pass the data structure to Matlab
	 */ 
	engPutVariable(ep, "data", mx_data);
	
	// Free memory afterwards
	mxDestroyArray(mx_data);

	// Copy anzcluster
	engPutDoubleValue(ep, "anzcluster", anzcluster);

	// Copy options
	engPutDoubleValue(ep, "exp", options->exp);
	engPutDoubleValue(ep, "maxIter", options->maxIter);
	engPutDoubleValue(ep, "epsilon", options->epsilon);

	double temp=options->displayInfo?1:0;

	engPutDoubleValue(ep, "displayInfo", temp);

	/*
	 * Evaluate the fuzzy cmeans
	 */

	engEvalString(ep, "[center, U, obj_fcn] = fcm(data, anzcluster, [exp, maxIter, epsilon, displayInfo]);");

#ifdef DEBUG
	printf("done calc\n");
#endif

	/*
	 * Now get the results
	 */

	mxArray *mx_centers = NULL;
	mx_centers = engGetVariable(ep, "center");

	if (mx_centers == NULL) {
		printf("Matlab_CMeans: Error: Get Array 'center' failed");
		return;
	}
	
	dataptr = (double*)mxGetPr(mx_centers);
	
	clusterCenters.resize(mxGetM(mx_centers));

	// Re-Init Cluster Centers
	for(vei_t h=0; h < mxGetM(mx_centers); h++)
		clusterCenters[h].create(mxGetN(mx_centers));

	i=0;
	for(vei_t d=0; d < mxGetN(mx_centers); d++)
		for(vei_t h=0; h < mxGetM(mx_centers); h++)
			clusterCenters[h].set_comp(dataptr[i++], F64B_TYPE, d);

	mxDestroyArray(mx_centers);

#ifdef DEBUG
	printf("done getting centers\n");
#endif

	// ... and now obj_fcn

	mxArray *mx_obj_fcn = NULL;
	mx_obj_fcn = engGetVariable(ep, "obj_fcn");

	if (mx_obj_fcn == NULL) {
		printf("Matlab_CMeans: Error: Get Array 'obj_fcn' failed");
		return;
	}

	dataptr = (double*)mxGetPr(mx_obj_fcn);

	for(vei_t h=0; h < mxGetM(mx_obj_fcn); h++)
		objFcn.push_back(dataptr[h]);

	mxDestroyArray(mx_obj_fcn);

#ifdef DEBUG
	printf("done getting obj_fcn\n");
#endif

	// ... and now the membership table U
	
	mxArray *mx_U = NULL;
	mx_U = engGetVariable(ep, "U");

	if (mx_U == NULL) {
		printf("Matlab_CMeans: Error: Get Array 'U' failed");
		return;
	}

	dataptr = (double*)mxGetPr(mx_U);

	membershiptable_U.resize(mxGetN(mx_U));

	for(vei_t h=0; h < mxGetN(mx_U); h++)
		membershiptable_U[h].resize(mxGetM(mx_U));
	
	i=0;
	for(vei_t d=0; d < mxGetN(mx_U); d++)
		for(vei_t h=0; h < mxGetM(mx_U); h++)
		{
			//printf("%d,%d = %f\n", h, d, dataptr[i]);
			(membershiptable_U[d])[h]=dataptr[i++];
		}

	mxDestroyArray(mx_U);

#ifdef DEBUG
	printf("done getting mx_U\n");
#endif

	/*
	 * We're done! Close MATLAB engine and exit.
	 */
	
	//engClose(ep);
}
