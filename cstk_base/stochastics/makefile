# CSTK examples makefile, by Martin:
#

CC=g++ -Wall -O3 -g

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
datadef.o:
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

all:	datadef.o