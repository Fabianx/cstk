# CSTK algorithm examples makefile, by martin:
#

CC=g++ -Wall -O3 -g

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../../..;pwd)
VECTOR_DIR=$(BASE_DIR)/cstk_base/vector
SD_DIR=$(BASE_DIR)/sensordata
STO_DIR=$(BASE_DIR)/cstk_base/stochastics
MATRIX_DIR=$(BASE_DIR)/cstk_base/matrix

all:	stochastics_demo

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	@-rm -f ./*_demo
	
##### KMeans Demo ##############################################################

stochastics_demo: stochastics_t.o
	$(CC) ./stochastics_t.o $(VECTOR_DIR)/datacell.o $(VECTOR_DIR)/dvector.o $(MATRIX_DIR)/dvectormatrix.o $(STO_DIR)/covariance/covariance.o $(STO_DIR)/mvg/mvg.o -o ./stochastics_demo
	
stochastics_t.o: covariance.o mvg.o dvectormatrix.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o
	
##### Dependencies ##############################################################

dvector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)
	
covariance.o:
	@cd $(STO_DIR)/$*; make $@ BASE_DIR=$(BASE_DIR)

mvg.o:
	@cd $(STO_DIR)/$*; make $@ BASE_DIR=$(BASE_DIR)
	
dvectormatrix.o:
	@cd $(MATRIX_DIR); make $@ BASE_DIR=$(BASE_DIR)