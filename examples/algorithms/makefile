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

##### SDMA Demo #################################################################

sdma_demo:	sdma_t.o
	$(CC) sdma_t.o $(ALG_DIR)/sdm/sdmafct.o $(ALG_DIR)/sdm/sdma.o \
	$(VECTOR_DIR)/kvector.o $(VECTOR_DIR)/binvector.o $(VECTOR_DIR)/datacell.o -o ./$@
	
sdma_t.o: sdmafct.o sdma.o datacell.o binvector.o kvector.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@
	
##### SOSDM Demo #################################################################

sosdm_demo:	sosdm_t.o
	$(CC) sosdm_t.o $(ALG_DIR)/sdm/sosdm.o $(ALG_DIR)/sdm/sdma.o $(VECTOR_DIR)/kvector.o \
	      $(VECTOR_DIR)/binvector.o $(VECTOR_DIR)/datacell.o -o ./$@
	
sosdm_t.o: sosdm.o sdma.o datacell.o binvector.o kvector.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@
	      
##### Dependencies: ##############################################################

kvector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

binvector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

dvector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

datacell.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

sdma.o:
	@cd $(BASE_DIR)/algorithms/sdm/; make $@ BASE_DIR=$(BASE_DIR)
	
sdmafct.o:
	@cd $(BASE_DIR)/algorithms/sdm/; make $@ BASE_DIR=$(BASE_DIR)
	
sosdm.o:
	@cd $(BASE_DIR)/algorithms/sdm/; make $@ BASE_DIR=$(BASE_DIR)
	
kmeans.o:
	@cd $(BASE_DIR)/algorithms/$*; make $@ BASE_DIR=$(BASE_DIR)
	
knn.o:
	@cd $(BASE_DIR)/algorithms/$*; make $@ BASE_DIR=$(BASE_DIR)	

###################################################################################