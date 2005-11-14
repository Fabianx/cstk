###################################################################################
# CSTK examples makefile, by martin:
#

CC=g++ -Wall -O3 -g
CC_ARM=/skiff/local/bin/arm-linux-g++ -Wall -O3 -g
BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)
VECTOR_DIR=$(BASE_DIR)/cstk_base/vector
SD_DIR=$(BASE_DIR)/sensordata
ALG_DIR=$(BASE_DIR)/algorithms

all:	kmeans_demo knn_demo sdma_demo sosdm_demo \
	kmeans_demo_arm knn_demo_arm sdma_demo_arm sosdm_demo_arm

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	@-rm -f ./*_demo
	
##### KMeans Demo ################################################################

kmeans_demo: kmeans_t.o
	$(CC) kmeans_t.o $(VECTOR_DIR)/dvector.o $(VECTOR_DIR)/datacell.o \
	      $(ALG_DIR)/kmeans/kmeans.o -o ./$@
	
kmeans_demo_arm: kmeans_t_arm.o
	$(CC_ARM) kmeans_t_arm.o $(VECTOR_DIR)/dvector_arm.o $(VECTOR_DIR)/datacell_arm.o \
	      $(ALG_DIR)/kmeans/kmeans_arm.o -o ./$@
	
kmeans_t.o: kmeans.o datacell.o dvector.o  
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

kmeans_t_arm.o: kmeans_arm.o datacell_arm.o dvector_arm.o  
	$(CC_ARM) -c ./kmeans_t.cpp -I"$(BASE_DIR)" -o ./$@

##### KNN Demo ###################################################################

knn_demo: knn_t.o
	$(CC) knn_t.o $(VECTOR_DIR)/dvector.o $(VECTOR_DIR)/datacell.o \
	      $(ALG_DIR)/knn/knn.o -o ./$@
	
knn_demo_arm: knn_t_arm.o
	$(CC_ARM) knn_t_arm.o $(VECTOR_DIR)/dvector_arm.o $(VECTOR_DIR)/datacell_arm.o \
	      $(ALG_DIR)/knn/knn_arm.o -o ./$@
	
knn_t.o: knn.o datacell.o dvector.o  
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

knn_t_arm.o: knn_arm.o datacell_arm.o dvector_arm.o  
	$(CC_ARM) -c ./knn_t.cpp -I"$(BASE_DIR)" -o ./$@

##### SDMA Demo #################################################################

sdma_demo:	sdma_t.o
	$(CC) sdma_t.o $(ALG_DIR)/sdm/sdmafct.o $(ALG_DIR)/sdm/sdma.o \
		$(VECTOR_DIR)/kvector.o $(VECTOR_DIR)/binvector.o \
		$(VECTOR_DIR)/datacell.o -o ./$@
	
sdma_demo_arm:	sdma_t_arm.o
	$(CC_ARM) sdma_t_arm.o $(ALG_DIR)/sdm/sdmafct_arm.o $(ALG_DIR)/sdm/sdma_arm.o \
		$(VECTOR_DIR)/kvector_arm.o $(VECTOR_DIR)/binvector_arm.o \
		$(VECTOR_DIR)/datacell_arm.o -o ./$@
	
sdma_t.o: sdmafct.o sdma.o datacell.o binvector.o kvector.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@
	
sdma_t_arm.o: sdmafct_arm.o sdma_arm.o datacell_arm.o binvector_arm.o kvector_arm.o
	$(CC_ARM) -c ./sdma_t.cpp -I"$(BASE_DIR)" -o ./$@
	
##### SOSDM Demo #################################################################

sosdm_demo:	sosdm_t.o
	$(CC) sosdm_t.o $(ALG_DIR)/sdm/sosdm.o $(ALG_DIR)/sdm/sdma.o $(VECTOR_DIR)/kvector.o \
	      $(VECTOR_DIR)/binvector.o $(VECTOR_DIR)/datacell.o -o ./$@
	
sosdm_demo_arm:	sosdm_t_arm.o
	$(CC_ARM) sosdm_t_arm.o $(ALG_DIR)/sdm/sosdm_arm.o $(ALG_DIR)/sdm/sdma_arm.o \
		$(VECTOR_DIR)/kvector_arm.o $(VECTOR_DIR)/binvector_arm.o \
		$(VECTOR_DIR)/datacell_arm.o -o ./$@
	
sosdm_t.o: sosdm.o sdma.o datacell.o binvector.o kvector.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@
	      
sosdm_t_arm.o: sosdm_arm.o sdma_arm.o datacell_arm.o binvector_arm.o kvector_arm.o
	$(CC_ARM) -c ./sosdm_t.cpp -I"$(BASE_DIR)" -o ./$@
	      
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

kvector_arm.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

binvector_arm.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

dvector_arm.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

datacell_arm.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

sdma_arm.o:
	@cd $(BASE_DIR)/algorithms/sdm/; make $@ BASE_DIR=$(BASE_DIR)
	
sdmafct_arm.o:
	@cd $(BASE_DIR)/algorithms/sdm/; make $@ BASE_DIR=$(BASE_DIR)
	
sosdm_arm.o:
	@cd $(BASE_DIR)/algorithms/sdm/; make $@ BASE_DIR=$(BASE_DIR)
	
kmeans_arm.o:
	@cd $(BASE_DIR)/algorithms/kmeans; make $@ BASE_DIR=$(BASE_DIR)
	
knn_arm.o:
	@cd $(BASE_DIR)/algorithms/knn; make $@ BASE_DIR=$(BASE_DIR)	


###################################################################################
