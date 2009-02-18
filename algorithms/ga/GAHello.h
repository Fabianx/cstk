#ifndef GAHELLO_H
#define GAHELLO_H

#include "GeneticAlgorithm.h"
#include <iostream>
#include <string>

class GAHelloIndividual : public GAIndividual
{
	public:
		string str;
		static string target;
		static vei_t tsize;
		static vei_t max_mutations;

		virtual GAIndividual* mate_with(GAIndividual* B);
		virtual void mutate();
		virtual GAIndividual* clone();
		virtual string toString();
};

class GAHelloFitness : public GAFitness
{
	public:
		virtual void start_fitness();
		virtual float calc_fitness(GAIndividual* g);
		virtual GAIndividual* create();
};

#endif
