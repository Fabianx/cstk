# CSTK sdm makefile, by kristof:
#

CC=g++ -Wall -O3 -g

all:	sdm.o sdma.o sdmafct.o sosdm.o

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
sdm.o: 
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

sdma.o: 
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

sdmafct.o: 
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

sosdm.o: 
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o
