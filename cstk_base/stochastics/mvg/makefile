# CSTK examples makefile, by Martin:
#

CC=g++ -Wall -O3 -g

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../../..;pwd)
VECTOR_DIR=$(BASE_DIR)/cstk_base/vector
SD_DIR=$(BASE_DIR)/sensordata
STO_DIR=$(BASE_DIR)/cstk_base/matrix


clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
mvg.o: dvectormatrix.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o
	
dvectormatrix.o:
	@cd $(STO_DIR); make $@ BASE_DIR=$(BASE_DIR)

all:	mvg.o