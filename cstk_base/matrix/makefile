# CSTK examples makefile, by Martin:
#

CC=g++ -Wall -O3 -g
BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
dvectormatrix.o:
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

all:	dvectormatrix.o