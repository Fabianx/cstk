# CSTK ksom makefile, by kristof:
#

CC=g++ -Wall -O3 -g

all:	ksom.o

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
ksom.o: 
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

