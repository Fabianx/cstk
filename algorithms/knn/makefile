# CSTK knn makefile, by kristof:
#

CC=g++ -Wall -O3 -g
BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)

all:	knn.o

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
knn.o: 
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

