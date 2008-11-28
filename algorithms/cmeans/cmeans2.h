//Basics
#ifndef CMEANS_H
#define CMEANS_H

#include <cstdlib>
#include <ctime>


#include "cmeans.h"
#include "cstk_base/types.h"
#include "cstk_base/vector/dvector.h" 

#define DIS_MANH 0
#define DIS_CHEB 1
#define DIS_EUCL 2
#define DIS_MINK 3
#define EXP 2
#define epsilon 0,05 //min verbesserung durchgang
#define maxIteration 30
#define fuzzyness 2.0 //m




class KMeans 
{
	public:
		Init(ve_t anzclusters, ve_t anzvektors, double[][] vektoren); //table anlegen und zufällig generieren
		run();/prozessing des c-means algo 
				
		// void savetoFile();
		// int restorefromFile();
				
		
	private:
		CalcclusterCenter();//berechnung der center
		CalcMF(); //update Abhängigkeitsfkt
		QualityFct(); // güte der clusterung
		double distance(double[] vec1, double[] vec2; //interne einbauen		
		double [][] membershiptable;
		double[][] vektors;
};

#endif
	
