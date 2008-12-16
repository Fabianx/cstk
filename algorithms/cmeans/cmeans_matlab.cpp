#include "cmeans_matlab.h"
#include "engine.h"

#define BUFSIZE 256

Matlab_CMeans::Matlab_CMeans(vector<DVector>* data, vei_t cluster_n, CMeansOptions* algorithm_options) : CMeans(data, cluster_n, algorithm_options)
{
}

void Matlab_CMeans::run()
{
#ifdef DEBUG
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

	mx_data = mxCreateDoubleMatrix(100, 2, mxREAL);
	
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
	
	// Copy anzcluster

	mx_anzcluster = mxCreateDoubleMatrix(1, 1, mxREAL);
	double temp = anzcluster;
	memcpy((char *) mxGetPr(mx_anzcluster), (char *) &temp, sizeof(double));

	/*
	 * And pass the data structures to Matlab
	 */ 
	engPutVariable(ep, "anzcluster", mx_anzcluster);
	engPutVariable(ep, "data", mx_data);
	
	// Free memory afterwards
	mxDestroyArray(mx_data);
	mxDestroyArray(mx_anzcluster);

	/*
	 * Evaluate the fuzzy cmeans
	 */

	// FIXME: Add options

	engEvalString(ep, "[center, U, obj_fcn] = fcm(data, anzcluster);");

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

	// Re-Init Cluster Centers
	for(vei_t h=0; h < mxGetM(mx_centers); h++)
	{
		DVector vec;
		vec.create(mxGetN(mx_centers));
		clusterCenters[h]=vec;
	}
	
	i=0;

	for(vei_t d=0; d < mxGetN(mx_centers); d++)
		for(vei_t h=0; h < mxGetM(mx_centers); h++)
			clusterCenters[h].set_comp(dataptr[i++], F64B_TYPE, d);

	mxDestroyArray(mx_centers);

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

	// ... and now the membership table U
	
	mxArray *mx_U = NULL;
	mx_U = engGetVariable(ep, "U");

	if (mx_U == NULL) {
		printf("Matlab_CMeans: Error: Get Array 'center' failed");
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

	/*
	 * We're done! Close MATLAB engine and exit.
	 */
	
	//engClose(ep);
}
