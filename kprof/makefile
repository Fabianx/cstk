# CSTK kprof makefile, by kristof:
#

CC=g++ -Wall -O3 -g
BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ..;pwd)

all:	kprof.o setparse.o rs232setparse.o udpsetparse.o \
	logfilesetparse.o simsetparse.o channelsetparse.o \
	windowsetparse.o plotsetparse.o ksomsetparse.o

kprof.o:	peak.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

rs232setparse.o: setparse.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

udpsetparse.o: setparse.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

logfilesetparse.o: setparse.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

simsetparse.o: setparse.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

channelsetparse.o: setparse.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

inputcolumnsetparse.o: setparse.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

windowsetparse.o: setparse.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

plotsetparse.o: setparse.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@
	
ksomsetparse.o: setparse.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

peak.o:
	@cd $(BASE_DIR)/algorithms/$*; make $@ BASE_DIR=$(BASE_DIR)

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
