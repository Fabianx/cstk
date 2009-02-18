#ifndef GABIT_H
#define GABIT_H

#include "GeneticAlgorithm.h"
#include <iostream>
#include <string>
#include "cstk_base/vector/binvector.h"

class GABitIndividual : public GAIndividual
{
	public:
		GABitIndividual(vei_t size);
		BinVector b;

		virtual GAIndividual* mate_with(GAIndividual* B);
		virtual void mutate();
		virtual GAIndividual* clone();
		virtual string toString();
	protected:
		vei_t tsize;
};

class GABitFitness : public GAFitness
{
	public:
		GABitFitness(vei_t size);
		virtual void start_fitness();
		virtual float calc_fitness(GAIndividual* g);
		virtual GAIndividual* create();
	protected:
		vei_t tsize;
};

#endif
