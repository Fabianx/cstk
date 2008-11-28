//Basics
#include <cstdlib>
#include <ctime>


#include "cmeans.h"
#include "cstk_base/types.h"
#include "cstk_base/vector/dvector.h"  

#define MaxCluster 1000 

//ve_t anzclusters; //"c" 
double [][] membershiptable;
//hilfsvariabeln
int iteration;//falls es früher abbricht, evtl brauch man das noch ;)
int dim;//zahl der dimensionen, später anders zu lösen
double [][] vektors[][dim]

private vei_t anzvektor=0, mexp=0; 
vector <dvektor> input;

void Cmeans::read_vec(dVector vec)
{
	input.push_back(vec);
	anzvektor++;
}

CMeans::Init(vei_t anzahl, f_64b fuzzyfyer, vei_t seldist, vei_t exp)
{
	clusterCenters = new DVector[anzahl];
	anzcluster = buckets;
	fuzzyness = fuzzyfyer;
	selected_dist = seldist;
	mexp= exp;	
}

void Cmeans::InitMem()
{
	vektors = vektors;
	new double[][] membership [anzvektors] [anzclusters];


	//initialisieren der membership
	for(vei_t h=0;h<anzvektor)  
 	   {  
 	        //  membership zufällig initialisieren.
 	        float sum = 0;  
 	        for(vei_t c=0;c<anzclusters;c++)  
 	          {  
	          membership[h][c] = 0.01 + srand((unsigned) sec);  
 	          sum += membership[h][c];  
 	          }  
 	        // Normalize so the sum of MFs for a particular data point will be equal to 1.
 	        for(vei_t c=0;c<anzclusters;c++)
			{ 
			membership[h][c] /= sum;  
 	 		}  
 	    }
}

void Cmeans::Run()
{
	double last
	lastj = QualityFct()
	for(iteration=0; iteration<=maxIteration; iteration++)
		{
			CalcclusterCenter();//Centers kalkulieren
			CalcMF();//neue Membershipfuction
			j = QualityFct();
			If (lastj -j)< epsilon break;
			lastj = j;
		}
}
	
void Cmeans::CalcclusterCenter()
{
	f64_b basis, divisor;
	for (int c=0; c<anzclusters; c++)
    	{	
			
		for(vei_t a=0; a < input[1].get_dim(); a++) 
		{
			basis=divisor=0;
			for(int b=0; b<anzvektors; a++)
			{	
				basis+= (f_64b) pow(membership [b][c], (double)fuzzyness) * input[b].getcomp(a);
				divisor+= (f_64b) pow(membership [b][c], (double)fuzzyness);				 
        		}  
 	        clusterCenters[c].void set_comp((f_64b)basis/divisor, input[1].get_type(a),a) ;//type eintrrag evtl. bearbeiten
       		}  
	}
}

void Cmeans::CalcMF()
{
	double basis, divisor;
	for (int b=0; b<anzvektors; a++)
	{
	basis=divisor=0;	
		for(vei_t c=0; c < anzclusters; c++) 
		{
			basis=	pow(double)(1/ distance(input[b], clusterCenter[c])), (double) (2/(fuzzyness-1)));
			for (vei_t a=0; a<anzclusters; a++)
			{
				divisor += pow((double)(1/ distance(input[b], clusterCenter[a]), (double) (2/(fuzzyness-1)));
			}		
			membership[b][c]= basis/divisor
		}
	}	
}

double Cmeans::QualityFct()
{
	double erg = 0;	
	for (vei_t c=0; c < clusters; c++)
	{
		for(vei_t b=0; b<anzvektors; a++) 
		{
			erg+= pow(membership[b][c], fuzzyness)*distance(vektors[b],clusterCenter[a])
		}
	}
return erg;
}

oas_t KMeans::distance(DVector& vector, DVector& datav, ve_t seldis, vei_t exp)
{
	switch (selected_dist)
	{
		case DIS_MANH:	return vector.dis_manh(datav);
		case DIS_CHEB:	return vector.dis_cheb(datav);
		case DIS_EUCL:	return vector.dis_eucl(datav);
		case DIS_MINK:	return vector.dis_mink(datav, mexp);
	}
	return vector.dis_manh(datav);
}

/*double Cmeans::distance(double[] vec1, double[] vec2)
{
	double sum=0;	
	for (i=0; i < dim; i++)
		if vec[i]>=vec2[i]
		{
		 	sum += pow((vec[i] -vec2[i]), 2);
		}else
		{
			sum += pow((vec2[i] -vec1[i]), 2);
		}
	return sqrt(sum)
}*/
