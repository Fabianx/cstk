# CSTK nmeaparser makefile, by kristof
#

CC=g++ -Wall -O3 -g
VECTOR_DIR  =$(BASE_DIR)/cstk_base/vector

all:	nmeaparser.o

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
nmeaparser.o:	datacell.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)/" -o ./$*.o 
	
datacell.o:
	@cd $(VECTOR_DIR); make datacell.o BASE_DIR=$(BASE_DIR)
