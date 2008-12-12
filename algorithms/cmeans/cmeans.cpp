//Basics
#include <cstdlib>
#include <ctime>
//#include <vektor>

#include "cmeans.h"
#include "cstk_base/types.h"
#include "cstk_base/vector/dvector.h"
#include "cstk_base/matrix/dvectormatrix.h"

CMeans::CMeans(vei_t anzahl, f_64b fuzzyfyer, vei_t seldist, vei_t exp, vei_t maxIter)
{
	clusterCenters = new DVector[anzahl];
	anzcluster = anzahl;
	anzvektor =0;
	fuzzyness = fuzzyfyer;
	selected_dist = seldist;
	maxIteration = maxIter;
	mexp= exp;
	//vector<DVector> input; // evtl. wegmachen
}


//ve_t anzclusters; //"c" 
void CMeans::read_vec(DVector& vec)
{
	if (anzvektor==0)
	{
		input = new DVectorList;
		(*input).vector=new DVector(vec);
		(*input).next= NULL;
		(*input).last= NULL;
	}
	else
	{
		while (input->next!=0){input = input->next;};
		DVectorList *temp;
		temp = new DVectorList;
		(*temp).vector = new DVector(vec);
		(*temp).last= input;
		(*input).next=temp;
	};
	anzvektor++;
}


void CMeans::InitMem()
{
	membershiptable= new double [anzvektor*anzcluster];
	srand(time(0));
	double sum;	
	//initialisieren der membership
	for(vei_t h=0;h<anzvektor; h++)  
 	   {  
 	        //  membership zufällig initialisieren.
 	        sum = 0;  
 	        for(vei_t c=0;c<anzcluster;c++)  
 	          {  
	          	membershiptable[h*(c+1)+c] = 0.01 + rand()%3;  // Anordnung {V1C1,V1C2 ...VNC1,VNC2}
 	        	sum += membershiptable[h*(c+1)+c];  
 	          };  
 	        // Normalize the table, so the sum of MFs for a particular data point will be equal to 1.
 	        for(vei_t c=0;c<anzcluster;c++)
			{ 
			membershiptable[h*(c+1)+c] /= sum;
 	 		} ;
 	    };
}

void CMeans::run()
{
	InitMem();
	saveMemtoFile(0);
	double lastj,j;
	lastj = QualityFct();
	for(iteration=0; iteration<maxIteration; iteration++)
		{
			CalcclusterCenter();//Centers kalkulieren
			CalcMF();//neue Membershipfuction
			j = QualityFct();
			if ((lastj -j)< EPSILON) break;
			lastj = j;
		};
	saveFinal(iteration, j);	
}
	
void CMeans::CalcclusterCenter()
{
	f_64b basis, divisor;
	//while(!input.last){input =input.last};
	vei_t dim = input->vector->get_dim();
	for (vei_t c=0; c<anzcluster; c++)
    	{	
			
		for(vei_t a=0; a < dim; a++) 
		{
			basis=divisor=0;
			while(input->last!=0){input= input->last;};
			for(vei_t b=0; b<anzvektor; b++)
			{	
				basis+= (f_64b) pow(membershiptable [b*(c+1)+c], (double)fuzzyness) * input->vector->get_comp(a);
				divisor+= (f_64b) pow(membershiptable [b*(c+1)+c], (double)fuzzyness);
				if (input->next!=NULL)
				{
				input=input->next;
				};								 
        		};  
 	        clusterCenters[c].set_comp((f_64b)basis/divisor, input->vector->get_type(a),a) ;//type eintrrag evtl. bearbeiten
       		};
	};
}

void CMeans::CalcMF()
{
	double basis, divisor;
	while(input->last){input =input->last;};	
	for (vei_t b=0; b<anzvektor; b++)
	{
	basis=divisor=0;
		for(vei_t c=0; c < anzcluster; c++) 
		{
			basis=	pow((double)(1/ distance((*input->vector), clusterCenters[c])), (double) (2/(fuzzyness-1)));
			for (vei_t a=0; a<anzcluster; a++)
			{
				divisor += pow((double)(1/ distance((*input->vector), clusterCenters[a])), (double) (2/(fuzzyness-1)));
			};		
			membershiptable[b*(c+1)+c]= basis/divisor;
		};
	if (input->next!=NULL){input=input->next;};
	};	
}

double CMeans::QualityFct()
{
	double erg = 0;	
	for (vei_t c=0; c < anzcluster; c++)
	{
		while(input->last!=NULL){input =input->last;};
		for(vei_t b=0; b<anzvektor; b++) 
		{
			erg+= pow(membershiptable[b*(c+1)+c], fuzzyness)*distance((*input->vector),clusterCenters[c]);
			if (input->next!=NULL){input=input->next;};
		};
	};
return erg;
}

oas_t CMeans::distance(DVector& vector, DVector& datav)
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


void CMeans::saveMemtoFile(vei_t iter)
{
	FILE *f = fopen("Mem.sav","w");
	fprintf(f,"#trainingcluster=%i, Cluster=%i, iteration=%i\n", anzvektor, anzcluster, iter);
	for (vei_t i=0; i<anzvektor; i++)
		for (vei_t j=0; j<anzcluster; j++)
			fprintf(f,"(%i,%i,%f)\n",i,j,membershiptable[i*(j+1)+j]);
	fclose(f);
}

void CMeans::saveFinal(ve_t iter, double j)
{
	FILE *f = fopen("final.sav","w");
	fprintf(f, "#Clustercenter=%i, benötigte Iterationen= %i, Qualität= %g", anzcluster, iter, j);
	for (vei_t i=0; i<anzcluster; i++)
	{
		fprintf(f, "%i- %s", i, clusterCenters[i].to_string());
	}; 
	fprintf(f,"#trainingcluster=%i, Cluster=%i", anzvektor, anzcluster);
	for (vei_t i=0; i<anzvektor; i++)
		for (vei_t j=0; j<anzcluster; j++)
			fprintf(f,"(%i,%i,%f)\n",i,j,membershiptable[i*(j+1)+j]);
	fclose(f);
}

vei_t CMeans::retrieve_cluster(DVector& vec)
{
	oas_t dis = 2*f_64b_max;
	vei_t elem=-1;
	for (vei_t j=0; j<anzcluster; j++)
	{
		if (vec.dis_eucl(clusterCenters[j])<dis)
		{
			dis = vec.dis_eucl(clusterCenters[j]);
			elem = j;
		};		
	};	
	
	return elem;
}


/*
double Cmeans::distance(double[] vec1, double[] vec2)
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
