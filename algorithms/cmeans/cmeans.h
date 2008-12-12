//Basics
#ifndef CMEANS_H
#define CMEANS_H

#include <cstdlib>
#include <ctime>
//#include <vector>


#include "cstk_base/types.h"
#include "cstk_base/vector/dvector.h" 
#include "cstk_base/matrix/dvectormatrix.h"

#define DIS_MANH 0
#define DIS_CHEB 1
#define DIS_EUCL 2
#define DIS_MINK 3
#define EPSILON 0.05 //min verbesserung durchgang



class CMeans 
{
	public:
		void read_vec(DVector& vec);
		CMeans(vei_t anzahl, f_64b fuzzyfyer, vei_t seldist, vei_t exp, vei_t maxIter);
		void InitMem(); //table anlegen und zufällig generieren
		void run();//prozessing des c-means algo 
		vei_t iteration;	
		vei_t retrieve_cluster(DVector& vec);//spuckt den nächsten aus		
		void saveMemtoFile(vei_t iter);		
		void saveFinal(ve_t iter, double j);
		DVector* clusterCenters;		
		
	private:
		//typedef struct Position{DVektor& posi;};			


		void CalcclusterCenter();//berechnung der center
		void CalcMF(); //update Abhängigkeitsfkt
		double QualityFct(); // güte der clusterung
		oas_t distance(DVector& vector, DVector& datav);
		double* membershiptable;
		vei_t anzvektor;
		vei_t mexp;
		vei_t anzcluster, maxIteration, selected_dist;
		DVectorList *input;	
		//std::vector <DVector> input;
		double fuzzyness;
};

#endif
	
