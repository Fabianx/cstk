/***************************************************************************
                              gng.h  -  v.1.00
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
 
 #ifndef GNG_H
 #define GNG_H
 
 #include <stdlib.h> 
 #include <math.h>

 #include "cstk_base/vector/dvector.h"  
 #include "cstk_base/vector/kvector.h"  
 #include "cstk_base/matrix/dvectormatrix.h" 
 
 class NodeListElement;
 
 typedef struct GNG_PARAM
 {
 	vei_t age_max;
 	oas_t d;
	oas_t alpha;
	oas_t epsilon_b, epsilon_n;
 };
 
 class EdgeListElement
 {
 	public:
 		NodeListElement *connectA;
		NodeListElement *connectB;
		EdgeListElement *next;
		EdgeListElement *last;
		vei_t age;
 };
 
 class NodeListElement
 {
 	public:
		NodeListElement() {vector=NULL;next=NULL;}
		NodeListElement(vei_t nsize) {vector = new DVector(nsize);}
		~NodeListElement() {if (vector) delete vector;}
		void create(vei_t nsize) {vector = new DVector(nsize);}
		DVector *vector;
		NodeListElement *next;
		NodeListElement *last;
		vei_t NodeNumber;
		oas_t DeltaError;
		EdgeListElement *firstEdge;
 };
 
 class GNG
 {
 public:
 	GNG();
	GNG(DVector NodeA, DVector NodeB, vei_t MaximumAge, oas_t DecreaseError, oas_t AlphaVal, oas_t EpsilonVals);
	~GNG();
 	
	void create(DVector NodeA, DVector NodeB, vei_t MaximumAge, oas_t DecreaseError, oas_t AlphaVal, oas_t EpsilonVals);
	void feed(DVector& input);
 private:
	NodeListElement *first;
	GNG_PARAM par;
	vei_t count;
	NodeListElement *GetNeighborMaxErr(NodeListElement* center);
	void newNode();
	void newEdge(NodeListElement* nodeA, NodeListElement* nodeB);
	void removeEdge();
 };
 
 #endif
 