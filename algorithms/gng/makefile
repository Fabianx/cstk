# CSTK gng makefile, by martin:
#

CC=g++ -Wall -O3 -g
BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)

all:	gng.o

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
gng.o: 
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

