# CSTK rs232parser makefile, by kristof
#

CC=g++ -Wall -O3 -g
VECTOR_DIR  =$(BASE_DIR)/cstk_base/vector

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
rs232parser.o:	datacell.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)/" -o ./$*.o 
	
datacell.o:
	@cd $(VECTOR_DIR); make datacell.o BASE_DIR=$(BASE_DIR)

all:	rs232parser.o
