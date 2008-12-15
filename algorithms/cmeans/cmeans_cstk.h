//Basics
#ifndef CSTK_CMEANS_H
#define CSTK_CMEANS_H

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

#include "cmeans.h"

// FIXME: Add correct defaults
#define DEFAULT_CMEANS_FUZZYFYER	0.3
#define DEFAULT_CMEANS_SELDIST		DIS_EUCL

class CSTK_CMeansOptions : public CMeansOptions
{
	public:
		f_64b fuzzyfyer; /**< fuzzyfyer */
		ve_t seldist; /**< selected Distance function */

		CSTK_CMeansOptions();
};

class CSTK_CMeans : public CMeans
{
	public:
		CSTK_CMeans(list<DVector>* data, vei_t cluster_n, CSTK_CMeansOptions* algorithm_options = NULL);

		virtual void run();

	protected:
		/*void InitMem(); //table anlegen und zufällig generieren
		
		void saveMemtoFile(vei_t iter);		
		void saveFinal(ve_t iter, double j);*/

		CSTK_CMeansOptions* options;

	private:

/*		void CalcclusterCenter();//berechnung der center
		void CalcMF(); //update Abhängigkeitsfkt
		double QualityFct(); // güte der clusterung

		oas_t distance(DVector& vector, DVector& datav);*/

};

#endif
	
