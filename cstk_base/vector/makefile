# CSTK examples makefile, by kristof:
#

CC=g++ -Wall -O3 -g

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
binvector.o:
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

datacell.o:
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

dvector.o: datacell.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

kvector.o:
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

all:	binvector.o datacell.o dvector.o kvector.o

