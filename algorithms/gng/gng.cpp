/***************************************************************************
                              gng.cpp  -  v.1.00
                             -------------------
    begin                : Fri Nov 05 2004
    copyright            : (C) 2004 by Martin Berchtold
    email                : 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 
 #include "gng.h"
 
 GNG::GNG()
 {
 	par.age_max = 5;
	par.d = 0.8;
	par.alpha = 0.8;
	par.epsilon_b = 0.8;
	par.epsilon_n = 0.5 * par.epsilon_b;
 }
 
 GNG::GNG(DVector NodeA, DVector NodeB, vei_t MaximumAge, oas_t DecreaseError, oas_t AlphaVal, oas_t EpsilonVals)
 {
 	create(NodeA, NodeB, MaximumAge, DecreaseError, AlphaVal, EpsilonVals);
 }
 
 void GNG::create(DVector NodeA, DVector NodeB, vei_t MaximumAge, oas_t DecreaseError, oas_t AlphaVal, oas_t EpsilonVals)
 {
 	par.age_max = MaximumAge;
	par.d = DecreaseError;
	par.alpha = AlphaVal;
	par.epsilon_b = EpsilonVals;
	par.epsilon_n = 0.5 * par.epsilon_b;
	
	//create first node in neural gas network
	NodeListElement *nodeA;
	nodeA = new NodeListElement;
	(*nodeA).next = NULL;
	(*((*nodeA).vector)) = NodeA;
	(*nodeA).DeltaError = 0;
	//create second node in neural gas network
	NodeListElement *nodeB;
	nodeB = new NodeListElement;
	(*nodeB).next = nodeA;
	(*((*nodeB).vector)) = NodeB;
	(*nodeB).DeltaError = 0;
	(*nodeA).last = nodeB;
	(*nodeB).last = NULL;
	first = nodeB;
 }
 
 GNG::~GNG()
 {
 	NodeListElement *node;
	EdgeListElement *edge;
	while (first!=NULL) 
	{
		node = first;
		first = first->next;
		while (node->firstEdge!=NULL)
		{
			edge = node->firstEdge;
			node->firstEdge = edge->next;
			delete edge;
		}	   
		delete node;
	}
 }
 
 void GNG::feed(DVector& input)
 {
 	NodeListElement *currNode, *mindisel0, *mindisel1;
	mindisel0 = NULL; mindisel1 = NULL;
	oas_t MinDis0=(2.0 * DBL_MAX), MinDis1=(2.0 * DBL_MAX), tmp;
	vei_t NodeNum0, NodeNum1;
	currNode = first;
	bool edgeex=false;
	do
	{
		tmp = input.dis_eucl(*((*currNode).vector));
		if (tmp < MinDis0)
		{
			MinDis1 = MinDis0;
			NodeNum1 = NodeNum0;
			mindisel1 = mindisel0;	
		
			MinDis0 = tmp;
			NodeNum0 = (*currNode).NodeNumber;
			mindisel0 = currNode;
		}	
		else if ((tmp < MinDis1) and (tmp > MinDis0))
		{
			MinDis1 = tmp;
			NodeNum1 = (*currNode).NodeNumber;
			mindisel1 = currNode;
		}
		currNode = (*currNode).next;
	}
	while (currNode!=NULL);
	(*mindisel0).DeltaError += (MinDis0 * MinDis0);
	(*((*mindisel0).vector)) += par.epsilon_b * (input - (*((*mindisel0).vector)));
		
	EdgeListElement *currEdge;
	currEdge = (*mindisel0).firstEdge;
	do
	{
		(*currEdge).age++;
		(*((*currEdge).connectB->vector)) +=  par.epsilon_n * (input - (*((*currEdge).connectB->vector)));
		if ((*currEdge).connectB == mindisel0)
			edgeex = true;
		currEdge = (*currEdge).next;
	}
	while (currEdge!=NULL);
	if (not edgeex)
		newEdge(mindisel0, mindisel1);
 }
 
 void GNG::newNode()
 {
 	NodeListElement *current;
	current = first;
	NodeListElement *maxelem;
	maxelem = NULL;
	oas_t MaxError = 0;
	vei_t NodeNum;
 	do
	{
		if ((*current).DeltaError>MaxError)
		{
			MaxError = (*current).DeltaError;
			NodeNum = (*current).NodeNumber;
			maxelem = current;
		}	
		current = (*current).next;
	}
	while (current!=NULL);
	
	//create a new entry in the node list
	NodeListElement *node;
	node = new NodeListElement;
	(*node).next = first;
	(*first).last = node;
	(*node).last = NULL;
	first = node;
 
	//fill entry with data
	node->NodeNumber = count;
	count++;
	NodeListElement *nextelem;
	nextelem = GetNeighborMaxErr(maxelem);
	(*((*node).vector)) = 0.5 * ((*((*maxelem).vector)) + (*((*nextelem).vector)));
	newEdge(maxelem, nextelem);	
	
	(*maxelem).DeltaError *= par.alpha;
	(*nextelem).DeltaError *= par.alpha;
	(*node).DeltaError = (*maxelem).DeltaError;
 }
 
 void GNG::newEdge(NodeListElement* nodeA, NodeListElement* nodeB)
 {
 	EdgeListElement *edgeAB, *edgeBA;
	edgeAB = new EdgeListElement;
	edgeBA = new EdgeListElement;
	
	(*edgeAB).next = (*nodeA).firstEdge;
	(*nodeA).firstEdge->last = edgeAB;
	(*edgeAB).last = NULL;
	(*edgeBA).next = (*nodeB).firstEdge;
	(*nodeA).firstEdge->last = edgeAB;
	(*edgeBA).last = NULL;
	
	(*edgeAB).connectA = nodeA;
	(*edgeAB).connectB = nodeB;
	
	(*edgeBA).connectA = nodeB;
	(*edgeBA).connectB = nodeA;
	
	(*nodeA).firstEdge = edgeAB;
	(*nodeB).firstEdge = edgeBA;
	
	(*edgeAB).age = 0;
	(*edgeBA).age = 0;
 }
 
 void GNG::removeEdge()
 {
 	NodeListElement *node;
	EdgeListElement *edge,*tmp;
	while (first!=NULL) 
	{
		node = first;
		edge = node->firstEdge;
		do
		{
			if (edge->age > par.age_max)
			{
				tmp = edge->next;
				
				delete edge;
				edge = tmp;
			}	
			else
				edge = edge->next;
		}
		while (edge!=NULL);	   
		first = first->next;
	}
 }
 
 NodeListElement* GNG::GetNeighborMaxErr(NodeListElement* center)
 {
 	oas_t tmp = 0;
 	EdgeListElement *currEdge;
	NodeListElement *maxNode;
	maxNode = NULL;
	currEdge = (*center).firstEdge;
	do
	{
		if ((*currEdge).connectB->DeltaError > tmp)
		{
			tmp = (*currEdge).connectB->DeltaError;
			maxNode =(*currEdge).connectB;
		}
		currEdge = (*currEdge).next;
	}
	while (currEdge!=NULL);
	
	return maxNode;
 }
 
 
 
 
 