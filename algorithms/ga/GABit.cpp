#include "GABit.h"
#include <sstream>

GABitIndividual::GABitIndividual(vei_t size)
{
	b.create(size);
	tsize = size;
}

GAIndividual* GABitIndividual::mate_with(GAIndividual* B)
{
	vei_t spos = rand() % tsize;
	GABitIndividual* P = (GABitIndividual*)B;

	GABitIndividual* g = new GABitIndividual(tsize);
	for (int j=0; j<spos; j++)
		g->b.set_comp(b.get_comp(j), j);
	for (int j=spos; j<tsize; j++)
		g->b.set_comp(P->b.get_comp(j), j);
	return g;
}

void GABitIndividual::mutate()
{
	vei_t ipos = rand() % tsize;

	b.tgl_comp(ipos);
}

GAIndividual* GABitIndividual::clone()
{
	GABitIndividual* g = new GABitIndividual(tsize);
	g->b = b;
	g->fitness = fitness;
	return g;
}

string GABitIndividual::toString()
{
	ostringstream out;
	//out << "Individual " << str << " has fitness " << fitness;
	out << b.to_string() << ":" << fitness;
	return out.str();
}

// Fitness

GABitFitness::GABitFitness(vei_t size)
{
	tsize=size;
}

void GABitFitness::start_fitness()
{
}

float GABitFitness::calc_fitness(GAIndividual* g)
{
	oas_t fitness = 0;
	GABitIndividual* o = (GABitIndividual*)g;

	for (int j=0; j<tsize; j++)
		fitness+=o->b.get_comp(j);

	g->fitness=fitness;

	return fitness;
}

GAIndividual* GABitFitness::create()
{
	GABitIndividual* g = new GABitIndividual(tsize);

	for (int j=0; j<tsize; j++)
		g->b.set_comp((rand() % 10 != 0), j);

	return g;
}
