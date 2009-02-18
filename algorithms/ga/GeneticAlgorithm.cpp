#include "GeneticAlgorithm.h"
#include <algorithm>
#include <iostream>

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

	population = new vector<GAIndividual*>(options->popsize);
	for (int i = 0; i < options->popsize; i++)
		(*population)[i]=NULL;

	new_population = new vector<GAIndividual*>(options->popsize);
	for (int i = 0; i < options->popsize; i++)
		(*new_population)[i]=NULL;

	elite_selected = 0;
}

GeneticAlgorithm::~GeneticAlgorithm()
{
	if (population)
	{
		for (int i = 0; i < options->popsize; i++)
			if ((*population)[i])
				delete (*population)[i];

		delete population;
	}

	if (new_population)
	{
		for (int i = 0; i < options->popsize; i++)
			if ((*new_population)[i])
				delete (*new_population)[i];

		delete new_population;
	}
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
	fitness->start_fitness();
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
	cout << "Best: " << (*population)[0]->toString() << endl;
#if 0
	string old = "";
	string s = "";
	for (int i = 0; i < options->popsize; i++)
	{
		s = (*population)[i]->toString();
		if (s.compare(old) == 0)
			continue;
		cout << s << ",";
		old = s;
	}
	cout << endl;
#endif
}

void GeneticAlgorithm::mate()
{
	vei_t i1, i2;
	vei_t mating_selected = options->recombination * options->popsize;
	uint32_t mutation_prop = (RAND_MAX * options->mutation);

	select_elite();

	for (vei_t i=elite_selected; i<options->popsize; i++) 
	{
		i1 = rand() % mating_selected;
		i2 = rand() % mating_selected;

		if ((*new_population)[i])
			delete (*new_population)[i];
		(*new_population)[i] = (*population)[i1]->mate_with((*population)[i2]);
		if (rand() < mutation_prop)
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
