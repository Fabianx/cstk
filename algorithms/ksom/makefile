# CSTK ksom makefile, by kristof:
#

CC=g++ -Wall -O3 -g
BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ..;pwd)

all:	ksom.o

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
ksom.o: 
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

