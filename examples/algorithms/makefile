###################################################################################
# CSTK examples makefile, by martin:
#

CC=g++ -Wall -O3 -g

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)
VECTOR_DIR=$(BASE_DIR)/cstk_base/vector
SD_DIR=$(BASE_DIR)/sensordata
ALG_DIR=$(BASE_DIR)/algorithms

all:	kmeans_demo knn_demo

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	@-rm -f ./*_demo
	
##### KMeans Demo ################################################################

kmeans_demo: kmeans_t.o
	$(CC) kmeans_t.o $(VECTOR_DIR)/dvector.o $(VECTOR_DIR)/datacell.o \
	      $(ALG_DIR)/kmeans/kmeans.o -o ./$@
	
kmeans_t.o: kmeans.o datacell.o dvector.o  
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### KNN Demo ###################################################################

knn_demo: knn_t.o
	$(CC) knn_t.o $(VECTOR_DIR)/dvector.o $(VECTOR_DIR)/datacell.o \
	      $(ALG_DIR)/knn/knn.o -o ./$@
	
knn_t.o: knn.o datacell.o dvector.o  
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@
	

##### Dependencies: ##############################################################

dvector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

datacell.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)
	
kmeans.o:
	@cd $(BASE_DIR)/algorithms/$*; make $@ BASE_DIR=$(BASE_DIR)
	
knn.o:
	@cd $(BASE_DIR)/algorithms/$*; make $@ BASE_DIR=$(BASE_DIR)	

###################################################################################