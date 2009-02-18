//Basics
#ifndef GENETIC_ALGORITHM_H 
#define GENETIC_ALGORITHM_H

#include <cstdlib>
#include <ctime>
#include <vector>
#include <list>

using namespace std;

#include "cstk_base/types.h"
#include "cstk_base/vector/binvector.h"

#define GA_DEFAULT_EPSILON		1e-5
#define GA_DEFAULT_POPULATION_SIZE	2048
#define GA_DEFAULT_MAX_ITERATIONS	200
#define GA_DEFAULT_ELITE		0.1 // 10% elite
#define GA_DEFAULT_MUTATION		0.01 // 1% mutation probability
#define GA_DEFAULT_RECOMBINATION	0.5 // 50% recombination probability

class GeneticAlgorithmOptions
{
	public:
		oas_t epsilon; /**< minimum amount of improvement */
		vei_t popsize; /**< population size */
		vei_t maxiter; /**< population size */
		oas_t elite; /**< percentage of elite members */
		oas_t mutation; /**< probability of mutation */
		oas_t recombination; /**< probability of recombination */

		GeneticAlgorithmOptions();
};

class GAIndividual
{
	public:
		oas_t fitness;
		virtual GAIndividual* mate_with(GAIndividual* B)=0;
		virtual void mutate()=0;
		virtual GAIndividual* clone()=0;
		virtual void print()=0;
};

// Perhaps add a cache
// Factory pattern
class GAFitness
{
	public:
		virtual float calc_fitness(GAIndividual* g)=0;
		virtual GAIndividual* create()=0;
};

class GeneticAlgorithm
{
	public:
		GeneticAlgorithm(GAFitness* f, GeneticAlgorithmOptions* algorithm_options = NULL);
		~GeneticAlgorithm();
		
		virtual void run(); /**< process the ga algorithm */

		// Algorithm pattern: You can replace each step
		virtual void init(vector<GAIndividual*>* new_pop = NULL); /**< choose initial population */
		virtual void calc_fitness();
		virtual void sort_by_fitness();
		virtual void print_best();
		
		virtual void mate();
		
		virtual void select_elite();

		// Output parameters
		
		vector<GAIndividual*>* population; /**< The population P' */

	protected:
		GeneticAlgorithmOptions* options; /**< The algorithm options */
		GAFitness* fitness; /**< The fitness class */
		vector<GAIndividual*>* new_population; /**< The population P */
		vei_t elite_selected;
};

#endif
