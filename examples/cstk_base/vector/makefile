###################################################################################
# CSTK examples makefile, by martin:
#

CC=g++ -Wall -O3 -g

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../../..;pwd)
VECTOR_DIR=$(BASE_DIR)/cstk_base/vector
SD_DIR=$(BASE_DIR)/sensordata

all:	binvector_demo bvector_demo dvector_demo kvector_demo

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	@-rm -f ./*_demo
	
##### BinVector Demo ##############################################################

binvector_demo: binvector_t.o
	$(CC) ./binvector_t.o $(VECTOR_DIR)/binvector.o -o ./binvector_demo
	
binvector_t.o: binvector.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o
	
##### BVector Demo ################################################################

bvector_demo: 
	$(CC) ./bvector_t.cpp -I"$(BASE_DIR)" -o ./bvector_demo
	
##### DVector Demo ################################################################

dvector_demo: dvector_t.o
	$(CC) ./dvector_t.o $(VECTOR_DIR)/dvector.o $(VECTOR_DIR)/datacell.o \
	      -o ./dvector_demo
	
dvector_t.o: dvector.o datacell.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

##### KVector Demo ################################################################

kvector_demo: kvector_t.o
	$(CC) ./kvector_t.o $(VECTOR_DIR)/kvector.o -o ./kvector_demo
	
kvector_t.o: kvector.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

	
##### Dependencies: ##############################################################

binvector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

dvector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

datacell.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)
	
kvector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

###################################################################################