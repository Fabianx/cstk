# CSTK x11 makefile, by kristof:
#

CC=g++ -Wall -O3 -g
BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)

all:	x11plot.o kvplot.o clustplot.o

x11plot.o:
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

kvplot.o: kvector.o peak.o x11plot.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

clustplot.o: kvector.o x11plot.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

clean:
	@-rm -f ./*.o
	@-rm -f ./*~

kvector.o:
	@cd $(BASE_DIR)/cstk_base/vector; make $@

peak.o:
	@cd $(BASE_DIR)/algorithms/peak; make $@

