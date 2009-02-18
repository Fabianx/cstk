#include "GAHello.h"
#include <sstream>

string GAHelloIndividual::target = std::string("Hello world!");
vei_t GAHelloIndividual::tsize = GAHelloIndividual::target.size();
vei_t GAHelloIndividual::max_mutations = GAHelloIndividual::tsize;

GAIndividual* GAHelloIndividual::mate_with(GAIndividual* B)
{
	vei_t spos = rand() % tsize;
	GAHelloIndividual* P = (GAHelloIndividual*)B;

	GAHelloIndividual* g = new GAHelloIndividual();
	g->str = str.substr(0, spos) + P->str.substr(spos, tsize - spos);
	return g;
}

void GAHelloIndividual::mutate()
{
	vei_t num_mutations = rand() % max_mutations;

	for (vei_t i = 0; i < num_mutations; i++)
	{
		vei_t ipos = rand() % tsize;
		vei_t delta = (rand() % 90) + 32;

		str[ipos] = ((str[ipos] + delta) % 122);
	}
}

GAIndividual* GAHelloIndividual::clone()
{
	GAHelloIndividual* g = new GAHelloIndividual();
	g->str = str;
	g->fitness = fitness;
	return g;
}

string GAHelloIndividual::toString()
{
	ostringstream out;
	//out << "Individual " << str << " has fitness " << fitness;
	out << str << ":" << fitness;
	return out.str();
}

// Fitness

float GAHelloFitness::calc_fitness(GAIndividual* g)
{
	oas_t fitness = 0;
	GAHelloIndividual* o = (GAHelloIndividual*)g;

	for (int j=0; j<GAHelloIndividual::tsize; j++)
		fitness += abs(int(o->str[j] - GAHelloIndividual::target[j]));

	g->fitness=fitness;

	return fitness;
}

GAIndividual* GAHelloFitness::create()
{
	GAHelloIndividual* g = new GAHelloIndividual();
	g->str.erase();

	for (int j=0; j<GAHelloIndividual::tsize; j++)
		g->str += (rand() % 90) + 32;

	return g;
}
