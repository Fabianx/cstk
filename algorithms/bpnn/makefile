# CSTK bpnn makefile, by kristof:
#

CC=g++ -Wall -O3 -g

all:	bpnn.o

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
bpnn.o: 
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

