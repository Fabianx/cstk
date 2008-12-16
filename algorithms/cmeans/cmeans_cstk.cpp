#include "cmeans_cstk.h"

//#define DEBUG 1

CSTK_CMeansOptions::CSTK_CMeansOptions() : CMeansOptions()
{
	mexp = 2; // FIXME
	seldist = DIS_EUCL; // Used also by Matlab
}

static CSTK_CMeansOptions DefaultCSTK_CMeansOptions;

CSTK_CMeans::CSTK_CMeans(vector<DVector>* data, vei_t cluster_n, CSTK_CMeansOptions* algorithm_options) : CMeans(data, cluster_n, algorithm_options)
{
	if (algorithm_options == NULL)
		this->CMeans::options = &DefaultCSTK_CMeansOptions;

	options=(CSTK_CMeansOptions*)this->CMeans::options;

}

void CSTK_CMeans::run()
{
#ifdef DEBUG
	printf("It works! Input Vectors:\n");

	for (int i = 0; i < input.size(); i++)
		printf("%d: %s, ", i, input[i].to_string());
	printf("\n\n");
#endif

	InitMembershipTable();

	oas_t lastj, j;
	
	CalcClusterCenters(); // Centers kalkulieren

	lastj = QualityFct();
	objFcn.push_back(lastj);
	
	for( vei_t iteration = 0; iteration < options->maxIter; iteration++)
	{
		CalcClusterCenters(); // Centers kalkulieren
		CalcMF();	     // Neue Membershipfuction
		
		j = QualityFct();
		objFcn.push_back(j);
		if (options->displayInfo)
			printf("Iteration count = %d, obj. fcn = %f\n",iteration+1, j);
		
		oas_t diff = lastj - j;

		if ( diff < options->epsilon)
			break;

		lastj = j;
	}
}

void CSTK_CMeans::InitMembershipTable()
{
	srand(time(0));

	membershiptable_U.resize(input.size());

	// Initialisieren der Membership Table
	
	for(vei_t h=0; h < input.size(); h++)  
 	{ 
		vector<oas_t>* row = &(membershiptable_U[h]);
 		
		//  Membership zuf�llig initialisieren.
		row->resize(anzcluster);

		unsigned int max = RAND_MAX;
		
		vei_t c;
		for(c = 0; c<anzcluster-1; c++)
		{
			unsigned int r = rand() % max;
			(*row)[c] = (oas_t)(r % RAND_MAX) / RAND_MAX;
			max -= r;
		}
		(*row)[c] = (oas_t)max / RAND_MAX;

 	        // Normalize the table, so the sum of MFs for a particular 
		// data point will be equal to 1.
 	        //for(vei_t c=0; c<anzcluster; c++)
		//	(*row)[c] /= sum;
	}

#ifdef DEBUG
	printf("Initital membership table:\n");

	for(vei_t h=0; h < input.size(); h++)
	{
		vector<oas_t>* row = &(membershiptable_U[h]);
		printf("%d: ", h);
 	        
		for(vei_t c=0; c < anzcluster; c++)
		{
			printf("%f ", (*row)[c]);
		}
		printf("\n");
	}
#endif
}

oas_t CSTK_CMeans::QualityFct()
{
	oas_t erg = 0;

	// sum { h = 1 .. N } ( sum { c = 1 .. C } ( u_{h,c}^(m) * norm(x_h - c_c) 
	for(vei_t h=0; h<input.size(); h++) 
	{
		vector<oas_t>* row = &(membershiptable_U[h]);
		DVector* x = &(input[h]);

		for (vei_t c=0; c < anzcluster; c++)
			erg+= pow((*row)[c], options->exp)*pow(distance(*x, clusterCenters[c]),2);
	}

	return erg;

}

oas_t CSTK_CMeans::distance(DVector& vector, DVector& datav)
{
	switch (options->seldist)
	{
		case DIS_MANH:	return vector.dis_manh(datav);
		case DIS_CHEB:	return vector.dis_cheb(datav);
		case DIS_EUCL:	return vector.dis_eucl(datav);
		case DIS_MINK:	return vector.dis_mink(datav, options->mexp);
	}
	return vector.dis_manh(datav);
}

void CSTK_CMeans::CalcClusterCenters()
{
#ifdef DEBUG
	printf("--------\nCenters: ");
#endif

	for (vei_t c=0; c < anzcluster; c++)
	{
		DVector basis_sum;
		oas_t divisor_sum = 0;

		for(vei_t i=0; i<input.size(); i++) 
		{
			oas_t comp = pow(membershiptable_U[i][c], options->exp);
			//printf("comp = pow(%f, %f) = %f\n", membershiptable_U[i][c], options->exp, comp);

			if ( i == 0)
				basis_sum=(comp*input[i]);
			else
				basis_sum+=(comp*input[i]);
			divisor_sum+=comp;
			//printf("basis_sum = %s divisor_sum = %f\n", basis_sum.to_string(), divisor_sum);
		}

	 	// -> Does not work due to / not defined.
		// -> clusterCenters[c]=basis_sum/divisor_sum;
		
		DVector vec;
		vec.create(basis_sum.get_dim());
		for (vei_t i=0; i<basis_sum.get_dim(); i++)
         		vec.set_comp((get(basis_sum,i) / divisor_sum), F64B_TYPE, i);
		clusterCenters[c] = vec;
#ifdef DEBUG
		printf("%s, ", clusterCenters[c].to_string());
#endif
	}
#ifdef DEBUG
	printf("\n");
#endif
}

void CSTK_CMeans::CalcMF()
{
	for(vei_t h=0; h<input.size(); h++) 
	{
		vector<oas_t>* row = &(membershiptable_U[h]);
		DVector* x = &(input[h]);
		
		vector<oas_t> dist_x(anzcluster);

		for (vei_t l=0; l < anzcluster; l++)
			dist_x[l] = distance(*x, clusterCenters[l]);

		for (vei_t c=0; c < anzcluster; c++)
		{
			oas_t sum = 0;

			if (dist_x[c] == 0)
				sum = 1;
			else
			{
				for (vei_t l=0; l < anzcluster; l++)
				{
					sum+=pow(dist_x[c]/dist_x[l],2/(options->exp-1));
				}
			}

			(*row)[c]=1/sum;
		}
	}

	#ifdef DEBUG
	printf("\nU:\n\n");
	for(vei_t h=0; h < input.size(); h++)
	{
		vector<oas_t>* row = &(membershiptable_U[h]);
		printf("%d: ", h);
 	        
		for(vei_t c=0; c < anzcluster; c++)
		{
			printf("%f ", (*row)[c]);
		}
		printf("\n");
	}
	printf("\n");
	#endif
}

#if 0

void CMeansAlt::CalcMF()
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

double CMeansAlt::QualityFct()
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



void CMeansAlt::saveMemtoFile(vei_t iter)
{
	FILE *f = fopen("Mem.sav","w");
	fprintf(f,"#trainingcluster=%i, Cluster=%i, iteration=%i\n", anzvektor, anzcluster, iter);
	for (vei_t i=0; i<anzvektor; i++)
		for (vei_t j=0; j<anzcluster; j++)
			fprintf(f,"(%i,%i,%f)\n",i,j,membershiptable[i*(j+1)+j]);
	fclose(f);
}

void CMeansAlt::saveFinal(ve_t iter, double j)
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

vei_t CMeansAlt::retrieve_cluster(DVector& vec)
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

#endif
