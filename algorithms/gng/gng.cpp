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
 
 GNG::GNG(DVector& NodeA, DVector& NodeB, vei_t MaximumAge, oas_t DecreaseError, oas_t AlphaVal, oas_t EpsilonVals)
 {
 	create(NodeA, NodeB, MaximumAge, DecreaseError, AlphaVal, EpsilonVals);
 }
 
 void GNG::create(DVector& NodeA, DVector& NodeB, vei_t MaximumAge, oas_t DecreaseError, oas_t AlphaVal, oas_t EpsilonVals)
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
	(*nodeA).vector = new DVector(NodeA);
	(*nodeA).DeltaError = 0;
	(*nodeA).firstEdge = NULL;
	//create second node in neural gas network
	NodeListElement *nodeB;
	nodeB = new NodeListElement;
	(*nodeB).next = nodeA;
	(*nodeB).vector = new DVector(NodeB);
	(*nodeB).DeltaError = 0;
	(*nodeA).last = nodeB;
	(*nodeB).last = NULL;
	(*nodeB).firstEdge = NULL;
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
	while (currNode != NULL)
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
	(*mindisel0).DeltaError += (MinDis0 * MinDis0);
	if ((*mindisel0).vector == NULL)
		(*mindisel0).vector = new DVector;
		
	(*((*mindisel0).vector)) += par.epsilon_b * (input - (*((*mindisel0).vector)));
	EdgeListElement *currEdge;
	currEdge = (*mindisel0).firstEdge;
	while (currEdge != NULL)
	{
		(*currEdge).age++;
		if ((*currEdge).connectB->vector == NULL)
			(*currEdge).connectB->vector = new DVector;
			
		(*((*currEdge).connectB->vector)) +=  par.epsilon_n * (input - (*((*currEdge).connectB->vector)));
		if ((*currEdge).connectB == mindisel1)
			edgeex = true;
		currEdge = (*currEdge).next;
	}
	if (not edgeex)
		newEdge(mindisel0, mindisel1);
	
	if (currNode != NULL)
		delete currNode;
	
	//removeEdgeNode();
 }
 
 void GNG::newNode()
 {
 	NodeListElement *current;
	current = first;
	NodeListElement *maxelem;
	maxelem = NULL;
	oas_t MaxError = 0;
	vei_t NodeNum;
 	while (current!=NULL)
	{
		if ((*current).DeltaError>MaxError)
		{
			MaxError = (*current).DeltaError;
			NodeNum = (*current).NodeNumber;
			maxelem = current;
		}	
		current = (*current).next;
	}
	
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

	(*node).vector = new DVector;

	(*((*node).vector)) = 0.5 * ((*((*maxelem).vector)) + (*((*nextelem).vector)));
	newEdge(maxelem, node);	
	newEdge(nextelem, node);

	removeEdge(maxelem, nextelem);	

	(*maxelem).DeltaError *= par.alpha;
	(*nextelem).DeltaError *= par.alpha;
	(*node).DeltaError = (*maxelem).DeltaError;
 }
 
 void GNG::newEdge(NodeListElement* nodeA, NodeListElement* nodeB)
 {
 	EdgeListElement *edgeAB, *edgeBA;
	edgeAB = new EdgeListElement;
	edgeBA = new EdgeListElement;

	if ((*nodeA).firstEdge != NULL)
		(*nodeA).firstEdge->last = edgeAB;
		
	(*edgeAB).next = (*nodeA).firstEdge;
	(*nodeA).firstEdge = edgeAB;
	(*edgeAB).last = NULL;

	if ((*nodeB).firstEdge != NULL)
		(*nodeB).firstEdge->last = edgeBA;

	(*edgeBA).next = (*nodeB).firstEdge;
	(*nodeB).firstEdge = edgeBA;
	(*edgeBA).last = NULL;

	(*edgeAB).connectA = nodeA;
	(*edgeAB).connectB = nodeB;

	(*edgeBA).connectA = nodeB;
	(*edgeBA).connectB = nodeA;
	
	(*edgeAB).age = 0;
	(*edgeBA).age = 0;
 }
 
 void GNG::removeEdgeNode()
 {
 	NodeListElement *node;
	EdgeListElement *edge,*tmp;
	node = first;
	edge = node->firstEdge;
	printf("test1\n");
	while (node != NULL) 
	{
		printf("test2\n");
		while (edge!=NULL)
		{
			printf("test3\n");
			if (edge->age > par.age_max)
			{
				printf("test4\n");
				tmp = edge->next;
				printf("test5\n");
				removeEdge(edge->connectA,edge->connectB);
				printf("test6\n");
				edge = tmp;
				printf("test7\n");
			}	
			else
				edge = edge->next;
		} 
		node = node->next;
	}
	printf("test8\n");
 }
 
 void GNG::removeEdge(NodeListElement* nodeA, NodeListElement* nodeB)
 {
	EdgeListElement *edge,*tmp1,*tmp2;
	edge = nodeA->firstEdge;
	while (edge!=NULL)
	{
		if (edge->connectB == nodeB)
		{
			tmp1 = edge->next;
			tmp2 = edge->last;
			delete edge;
			if (tmp1 != NULL)
				tmp1->last = tmp2;
				
			tmp2->next = tmp1;
			edge = tmp1;
		}	
		else
			edge = edge->next;
	} 
	edge = nodeB->firstEdge;
	while (edge!=NULL)
	{
		if (edge->connectB == nodeA)
		{
			tmp1 = edge->next;
			tmp2 = edge->last;
			delete edge;
			if (tmp1 != NULL)
				tmp1->last = tmp2;

			tmp2->next = tmp1;
			edge = tmp1;
		}	
		else
			edge = edge->next;
	} 
 }
 
 NodeListElement* GNG::GetNeighborMaxErr(NodeListElement* center)
 {
 	oas_t tmp = 0;
 	EdgeListElement *currEdge;
	NodeListElement *maxNode;
	maxNode = NULL;
	currEdge = (*center).firstEdge;
	while (currEdge!=NULL)
	{
		if ((*currEdge).connectB->DeltaError > tmp)
		{
			tmp = (*currEdge).connectB->DeltaError;
			maxNode =(*currEdge).connectB;
		}
		currEdge = (*currEdge).next;
	}
	
	return maxNode;
 }
 
 void GNG::decreaseErrors()
 {
 	NodeListElement *current;
	current = first;
 	while (current!=NULL)
	{
		(*current).DeltaError *= par.d;
		current = (*current).next;
	}
 }
 
 DVector GNG::getFirst_node()
 {
 	currentlyReturned = first;
	return (*(first->vector));
 }

DVector GNG::getNext_node()
{
	if (currentlyReturned->next != NULL)
	{
		currentlyReturned = currentlyReturned->next;
		return (*(currentlyReturned->vector));
	}
	else 
	{
		DVector tmp(*(currentlyReturned->vector));
		tmp.set_comp(0,U8B_TYPE,0);
		return tmp;
	}
}
	
DVector GNG::getFirst_edge()
{
	if (currentlyReturned->firstEdge != NULL)
	{
		currentlyRetEdge = currentlyReturned->firstEdge;
		if (currentlyRetEdge->connectB->vector != NULL)
			return (*(currentlyRetEdge->connectB->vector));
		else
		{
			DVector tmp((*(currentlyReturned->vector)));
			tmp.set_comp(0,U8B_TYPE,0);
			return tmp;
		}
	}
	else
	{
		DVector tmp((*(currentlyReturned->vector)));
		tmp.set_comp(0,U8B_TYPE,0);
		return tmp;
	}
}

DVector GNG::getNext_edge()
{
	if (currentlyRetEdge->next != NULL)
	{
		currentlyRetEdge = currentlyRetEdge->next;
		if (currentlyRetEdge->connectB->vector != NULL)
			return (*(currentlyRetEdge->connectB->vector));
		else
		{
			DVector tmp((*(currentlyReturned->vector)));
			tmp.set_comp(0,U8B_TYPE,0);
			return tmp;
		}
	}
	else 
	{
		DVector tmp((*(currentlyReturned->vector)));
		tmp.set_comp(0,U8B_TYPE,0);
		return tmp;
	}
}
 
 