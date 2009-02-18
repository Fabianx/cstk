#ifndef GAHELLO_H
#define GAHELLO_H

#include "GeneticAlgorithm.h"
#include <iostream>
#include <string>

class GAHelloIndividual : public GAIndividual
{
	public:
		string str;

		virtual GAIndividual* mate_with(GAIndividual* B);
		virtual void mutate();
		virtual GAIndividual* clone();
		virtual string toString();
};

class GAHelloFitness : public GAFitness
{
	public:
		virtual float calc_fitness(GAIndividual* g);
		virtual GAIndividual* create();
};

#endif
