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
 
 GNG::GNG(GNG_PARAM parameters)
 {
 	par = parameters;
 }
 
 GNG::GNG(DVector& NodeA, DVector& NodeB, GNG_PARAM parameters)
 {
 	create(NodeA, NodeB, parameters);
 }
 
 void GNG::create(DVector& NodeA, DVector& NodeB, GNG_PARAM parameters)
 {
 	par = parameters;
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
 
 void GNG::savetoFile()
 {
 	FILE *f1 = fopen("node.sav","w");
	FILE *f2 = fopen("edge.sav","w");
	NodeListElement *currNode;
	EdgeListElement *currEdge;
	currNode = first;
	int i=0;
	fprintf(f1,"MaximumAge=%i, DecreaseError=%lf, AlphaVal=%lf, EpsilonValB=%lf, EpsilonValN=%lf, VecDim=%i\n",
		    par.age_max, par.d, par.alpha, par.epsilon_b, par.epsilon_n, first->vector->get_dim());
	while (currNode != NULL)
	{
		for (vei_t j=0; j<currNode->vector->get_dim(); j++)
			fprintf(f1,"(%i)(%i,%i,%i)%lf\n",currNode,i,j,currNode->vector->get_type(j),currNode->vector->get_comp(j));
		
		currEdge = (*currNode).firstEdge;
		while (currEdge != NULL)
		{
			fprintf(f2,"(%i,%i,%i)\n",i,currEdge->connectA,currEdge->connectB);
			currEdge = (*currEdge).next;
		}	
		
		i++;
		currNode = (*currNode).next;
	}
	fclose(f1);
	fclose(f2);
 }
 
 int GNG::restorefromFile()
 {
 	FILE *f = fopen("node.sav","r");
	if (!ferror(f))
	{
		oas_t out;	
		vei_t i, j, t, n;
		ves_t nodenum;
		first = NULL;
		fscanf(f,"MaximumAge=%hi, DecreaseError=%lf, AlphaVal=%lf, EpsilonValB=%lf, EpsilonValN=%lf, VecDim=%hi\n",
		    &par.age_max, &par.d, &par.alpha, &par.epsilon_b, &par.epsilon_n, &n);
		while (!feof(f))
		{
			
			NodeListElement *node;
			node = new NodeListElement;
			(*node).next = first;
			(*node).vector = new DVector(n);
			while ((i<n) or (!feof(f)))
			{
				fscanf(f,"(%li)(%hi,%hi,%hi)%lf\n",&nodenum,&i,&j,&t,&out);
				(*node).vector->set_comp(out,t,j);
			}
			(*node).DeltaError = 0;
			(*node).firstEdge = NULL;
			(*node).last = NULL;
			if (first!=NULL)
				(*first).last = node;
			first = node;
		}
		fclose(f);
	}
	return ferror(f);
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
		{;
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
	if ((not edgeex) and (mindisel1 != NULL))
		newEdge(mindisel0, mindisel1);
	if (currNode != NULL)
	{
		delete currNode;
		currNode = NULL;	
	}
	removeEdgeNode();
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
	(*node).firstEdge = NULL;
	first = node;
	//fill entry with data
	node->NodeNumber = count;
	count++;
	NodeListElement *nextelem;
	nextelem = GetNeighborMaxErr(maxelem);
	(*node).vector = new DVector;
	if (nextelem != NULL)
		(*((*node).vector)) = 0.5 * ((*((*maxelem).vector)) + (*((*nextelem).vector)));
	else
		(*((*node).vector)) = 0.5 * (*((*maxelem).vector));
	newEdge(maxelem, node);
	if (nextelem != NULL)	
	{
		newEdge(nextelem, node);
		removeEdge(maxelem, nextelem, false);	
		(*nextelem).DeltaError *= par.alpha;
	}
	(*maxelem).DeltaError *= par.alpha;
	(*node).DeltaError = (*maxelem).DeltaError;
	if (first == NULL)
		first = node;
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
	
	(*edgeBA).next = (*nodeB).firstEdge;
	if ((*nodeB).firstEdge != NULL)
		(*nodeB).firstEdge->last = edgeBA;
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
	while (node != NULL) 
	{
		while (edge != NULL)
		{
			if (edge->age > par.age_max)
			{
				tmp = edge->next;
				removeEdge(edge->connectA,edge->connectB, true);
				edge = tmp;
			}	
			if (edge!=NULL)
				edge = edge->next;
		} 
		node = node->next;
	}
 }
 
 void GNG::removeNode(NodeListElement* node)
 {
 	printf("Node gets deleted\n");
 	NodeListElement *tmp1, *tmp2;
	tmp1 = node->next;
	tmp2 = node->last;
	delete node;
	node = NULL;
	if (tmp1 != NULL)
		tmp1->last = tmp2;
	if (tmp2 != NULL)	
		tmp2->next = tmp1;
	else
		first = tmp1;
	EdgeListElement *edge, *temp1, *temp2;
 	edge = first->firstEdge;	
	while (edge!=NULL)
	{

		if (edge->connectB == node)
		{
			printf("Dead edge deleted\n");
			temp1 = edge->next;
			temp2 = edge->last;
			if ((temp2 == NULL) and (temp1 == NULL))
			{
				if (node == first)
					node->firstEdge = NULL;
				else
				 	removeNode(node);
			}
			delete edge;
			edge = NULL;
			if (temp1 != NULL)
				temp1->last = temp2;
			if (temp2 != NULL)	
				temp2->next = temp1;
			edge = temp1;
		}	
		else
			edge = edge->next;
	} 
 }
 
 void GNG::removeEdge(NodeListElement* nodeA, NodeListElement* nodeB, bool DelNodeIfNecessary)
 {
	EdgeListElement *edge,*tmp1,*tmp2;
	edge = nodeA->firstEdge;
	while (edge!=NULL)
	{
		if (edge->connectB == nodeB)
		{
			tmp1 = edge->next;
			tmp2 = edge->last;
			if (DelNodeIfNecessary) 
			{
				if ((tmp2 == NULL) and (tmp1 == NULL))
				 	if (nodeA != first)
				 		removeNode(nodeA);
				 	else
						nodeA->firstEdge = NULL;
				else
					if (nodeA->firstEdge == edge) 
						nodeA->firstEdge = edge->next;
			}
			else
				if (nodeA->firstEdge == edge) 
					nodeA->firstEdge = edge->next;
			
			delete edge;
			edge = NULL;
			if (tmp1 != NULL)
				tmp1->last = tmp2;
			if (tmp2 != NULL)	
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
			if (DelNodeIfNecessary) 
			{
				if ((tmp2 == NULL) and (tmp1 == NULL))
					if (nodeB != first)
						removeNode(nodeB);	
					else
						nodeB->firstEdge = NULL;	
				else
					if (nodeB->firstEdge == edge) 
						nodeB->firstEdge = edge->next;
			}
			else
				if (nodeB->firstEdge == edge) 
					nodeB->firstEdge = edge->next;		
				
			delete edge;
			edge = NULL;
			if (tmp1 != NULL)
				tmp1->last = tmp2;
			if (tmp2 != NULL)	
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
 
DVector* GNG::getFirst_node()
{
 	currentlyReturned = first;
	if (first != NULL)
		return first->vector;
	else
		return NULL;
}

DVector* GNG::getNext_node()
{
	if (currentlyReturned->next != NULL)
	{	
		currentlyReturned = currentlyReturned->next;
		return currentlyReturned->vector;
	}
	else 
		return NULL;
}
	
DVector* GNG::getFirst_edge()
{
	if (currentlyReturned != NULL)
	{
		if (currentlyReturned->firstEdge != NULL)
		{
			currentlyRetEdge = currentlyReturned->firstEdge;
			if (currentlyRetEdge->connectB->vector != NULL)
			{
				return currentlyRetEdge->connectB->vector;
			}
			else
				return NULL;
		}
		else
			return NULL;
	}
	else
		return NULL;
}

DVector* GNG::getNext_edge()
{
	if (currentlyRetEdge->next != NULL)
	{
		currentlyRetEdge = currentlyRetEdge->next;
		if (currentlyRetEdge->connectB->vector != NULL)
			return currentlyRetEdge->connectB->vector;
		else
			return NULL;
	}
	else
		return NULL;
}
 
 