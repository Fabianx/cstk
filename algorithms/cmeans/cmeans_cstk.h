//Basics
#ifndef CSTK_CMEANS_H
#define CSTK_CMEANS_H

#include "cmeans.h"
#include "cstk_base/types.h"
#include "cstk_base/vector/dvector.h" 
#include "cstk_base/matrix/dvectormatrix.h"

#define DIS_MANH 0
#define DIS_CHEB 1
#define DIS_EUCL 2
#define DIS_MINK 3

// FIXME: Add correct defaults
#define DEFAULT_CMEANS_FUZZYFYER	0.3
#define DEFAULT_CMEANS_SELDIST		DIS_EUCL

class CSTK_CMeansOptions : public CMeansOptions
{
	public:
		vei_t mexp; /**< mink exponent */
		vei_t seldist; /**< selected Distance function */

		CSTK_CMeansOptions();
};

class CSTK_CMeans : public CMeans
{
	public:
		CSTK_CMeans(vector<DVector>* data, vei_t cluster_n, CSTK_CMeansOptions* algorithm_options = NULL);

		virtual void run();

	protected:

		CSTK_CMeansOptions* options;

	private: 
		
		/** Tabelle zufällig initialisieren */
		void InitMembershipTable();
		oas_t QualityFct(); // Guete der Clusterung
		oas_t distance(DVector& vector, DVector& datav);

		void CalcClusterCenters(); // Berechnung der Center
		void CalcMF(); // Update Membershipfunction
};

#endif
	
