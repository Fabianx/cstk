#include "GeneticAlgorithm.h"
#include <algorithm>

GeneticAlgorithmOptions::GeneticAlgorithmOptions()
{
		// Set sane defaults
		epsilon = GA_DEFAULT_EPSILON;
		popsize = GA_DEFAULT_POPULATION_SIZE;
		maxiter = GA_DEFAULT_MAX_ITERATIONS;
		elite = GA_DEFAULT_ELITE;
		mutation = GA_DEFAULT_MUTATION;
		recombination = GA_DEFAULT_RECOMBINATION;
}

static GeneticAlgorithmOptions DefaultGeneticAlgorithmOptions;

GeneticAlgorithm::GeneticAlgorithm(GAFitness* f, GeneticAlgorithmOptions* algorithm_options) 
{
	fitness = f;
	//if (fitness == NULL)
	//	fitness = new GAFitness();

	options = algorithm_options;

	if (algorithm_options == NULL)
		options = &DefaultGeneticAlgorithmOptions;

	if (population == NULL)
		population = new vector<GAIndividual*>(options->popsize);
	if (new_population == NULL)
		new_population = new vector<GAIndividual*>(options->popsize);
}

void GeneticAlgorithm::run()
{
	init();

	for (int i=0; i<options->maxiter; i++)
	{
		calc_fitness();		// calculate fitness
		sort_by_fitness();	// sort them
		print_best();		// print the best one

		if ((*population)[0]->fitness == 0)
			break;

		mate();			// mate the population together
	}
}
void GeneticAlgorithm::init(vector<GAIndividual*>* new_pop)
{
	for (int i = 0; i < options->popsize; i++)
		(*population)[i]=fitness->create();
}

void GeneticAlgorithm::calc_fitness()
{
	for (int i = 0; i < options->popsize; i++)
		fitness->calc_fitness((*population)[i]);
}

static bool fitness_sort(GAIndividual* x, GAIndividual* y)
{
	return (x->fitness < y->fitness);
}

void GeneticAlgorithm::sort_by_fitness()
{
	sort(population->begin(), population->end(), fitness_sort);
}

void GeneticAlgorithm::print_best()
{
	(*population)[0]->print();
}

void GeneticAlgorithm::mate()
{
	vei_t i1, i2;
	vei_t mating_selected = options->recombination * options->popsize;

	for (vei_t i=elite_selected; i<options->popsize; i++) 
	{
		i1 = rand() % mating_selected;
		i2 = rand() % mating_selected;

		if ((*new_population)[i])
			delete (*new_population)[i];	
		(*new_population)[i] = (*population)[i1]->mate_with((*population)[i2]);

		(*new_population)[i]->mutate();
	}
	// Swap it
	vector<GAIndividual*>* temp = population;
	population=new_population;
	new_population=temp;
}

void GeneticAlgorithm::select_elite()
{
	elite_selected = options->elite * options->popsize;

	for (vei_t i=0; i<elite_selected; i++)
	{ 
		if ((*new_population)[i])
			delete (*new_population)[i];	
		(*new_population)[i] = (*population)[i]->clone();
	}
}
