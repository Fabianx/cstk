# CSTK algorithm examples makefile, by martin:
#

CC=g++ -Wall -O3 -g

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../../..;pwd)
VECTOR_DIR=$(BASE_DIR)/cstk_base/vector
SD_DIR=$(BASE_DIR)/sensordata
STO_DIR=$(BASE_DIR)/cstk_base/stochastics
MATRIX_DIR=$(BASE_DIR)/cstk_base/matrix

all:	dvectormatrix_demo

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	@-rm -f ./*_demo
	
##### KMeans Demo ##############################################################

dvectormatrix_demo: dvectormatrix_t.o
	$(CC) ./dvectormatrix_t.o $(VECTOR_DIR)/datacell.o $(VECTOR_DIR)/dvector.o $(MATRIX_DIR)/dvectormatrix.o -o ./dvectormatrix_demo
	
dvectormatrix_t.o: dvectormatrix.o dvector.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o
	
##### Dependencies ##############################################################

dvector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)
	
dvectormatrix.o:
	@cd $(MATRIX_DIR); make $@ BASE_DIR=$(BASE_DIR)