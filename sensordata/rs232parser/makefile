# CSTK rs232parser makefile, by kristof
#

CC=g++ -Wall -O3 -g
VECTOR_DIR  =$(BASE_DIR)/cstk_base/vector

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
rs232parser.o:	datacell.o bufferparser.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)/" -o ./$*.o 
	
datacell.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

bufferparser.o:
	@cd $(BASE_DIR)/sensordata; make $@ BASE_DIR=$(BASE_DIR)

all:	rs232parser.o

