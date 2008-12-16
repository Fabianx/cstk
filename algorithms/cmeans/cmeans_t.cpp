#include "cmeans_cstk.h"
#include "cmeans_matlab.h"

void test_algorithm(const char* name, CMeans* algorithm)
{
	int i,j;

	// Run it ...
	algorithm->run();
	
	// And print results ...
	printf("Results for %s algorithm:\n", name);
	printf("=========================\n");
	printf("\nCenters:\n\n");
	
	for (i = 0; i < algorithm->clusterCenters.size(); i++)
		printf("%d: %s\n", i, algorithm->clusterCenters[i].to_string());

	printf("\nobj. Fcn:\n\n");
	
	for (i = 0; i < algorithm->objFcn.size(); i++)
		printf("Iteration count = %d, obj. fcn = %f\n", i, algorithm->objFcn[i]);
	
	printf("\nMembershiptable U:\n\n");

	for(vei_t h=0; h < algorithm->membershiptable_U.size(); h++)
	{
		vector<oas_t>* row = &(algorithm->membershiptable_U[h]);
		printf("%d: ", h);
 	        
		for(vei_t c=0; c < (*row).size(); c++)
		{
			printf("%f ", (*row)[c]);
		}
		printf("\n");
	}
	printf("\n");
}

int main(void)
{
	vector<DVector>* l = new vector<DVector>;

	DVector x(2);

	srand(time(0));
	rand();
	
#if 1
	for ( int i = 0; i < 100; i++)
	{
		for ( int y = 0; y < 2; y++)
		{
			x.set_comp((oas_t)(rand() % RAND_MAX) / RAND_MAX, F64B_TYPE, y);
		}
		l->push_back(x);
	}

#if 0
	x.set_comp(1, F64B_TYPE, 0);
	l->push_back(x);

	x.set_comp(10, F64B_TYPE, 0);
	l->push_back(x);

	x.set_comp(5, F64B_TYPE, 0);
	l->push_back(x);
#endif

#else
	x.set_comp(1, F64B_TYPE, 0);
	l->push_back(x);

	x.set_comp(10, F64B_TYPE, 0);
	l->push_back(x);

	CSTK_CMeansOptions options;

	options.displayInfo = true;
	options.mexp = 2;

	x.set_comp(5, F64B_TYPE, 0);

	CSTK_CMeans algorithm(l, 2, &options);
	algorithm.readInputVector(x);
	algorithm.run();
	exit(0);
#endif

	// Now lets compare it:

	Matlab_CMeans matlab_algorithm(l, 2);
	CSTK_CMeans cstk_algorithm(l, 2);

	test_algorithm("CSTK", &cstk_algorithm);
	test_algorithm("MATLAB", &matlab_algorithm);
}
