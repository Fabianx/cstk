#include "GAHello.h"

#if 0
void test_algorithm(const char* name, GeneticAlgorithm* algorithm)
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
#endif

int main(void)
{
	srand(time(0));
	rand();
	
	GeneticAlgorithmOptions options;
	options.mutation = 0.25f; // 25%
	options.maxiter = 16384; 
	GAHelloFitness f;

	GeneticAlgorithm algorithm(&f, &options);
	algorithm.run();
	exit(0);
#if 0
	CSTK_CMeansOptions options;

	// Test option values
	//options.exp=1.5;

	// Now lets compare it:

	Matlab_CMeans matlab_algorithm(l, 2, &options);
	CSTK_CMeans cstk_algorithm(l, 2, &options);

	test_algorithm("CSTK", &cstk_algorithm);
	test_algorithm("MATLAB", &matlab_algorithm);
#endif
}
