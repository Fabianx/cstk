# CSTK kprof makefile, by kristof:
#

CC=g++ -Wall -O3 -g
BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ..;pwd)

all:	kprof.o setparse.o rs232setparse.o logfilesetparse.o

kprof.o:
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

rs232setparse.o: setparse.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

logfilesetparse.o: setparse.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
