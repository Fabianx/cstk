# CSTK udpparser makefile, by kristof
#

CC=g++ -Wall -O3 -g
VECTOR_DIR  =$(BASE_DIR)/cstk_base/vector

all:	udpparser.o
 
clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
udpparser.o:	datacell.o bufferparser.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)/" -o ./$*.o 
	
datacell.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

bufferparser.o:
	@cd $(BASE_DIR)/sensordata; make $@ BASE_DIR=$(BASE_DIR)
