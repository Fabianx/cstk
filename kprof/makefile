# CSTK kprof makefile, by kristof:
#

CC=g++ -Wall -O3 -g
BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ..;pwd)
VECTOR_DIR=$(BASE_DIR)/cstk_base/vector

all:	kprof.o setparse.o rs232setparse.o udpsetparse.o \
	logfilesetparse.o simsetparse.o channelsetparse.o \
	windowsetparse.o plotsetparse.o ksomsetparse.o \
	iparse.o wparse.o

iparse.o: datacell.o binvector.o kvector.o dvector.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

wparse.o: kvector.o peak.o plotsetparse.o windowsetparse.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

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


##### Dependencies: ##############################################################
	
peak.o:
	@cd $(BASE_DIR)/algorithms/$*; make $@ BASE_DIR=$(BASE_DIR)


datacell.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)
	
binvector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

kvector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

dvector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

bufferparser.o:
	@cd $(BASE_DIR)/sensordata; make $@ BASE_DIR=$(BASE_DIR)

rs232parser.o:
	@cd $(BASE_DIR)/sensordata/$*; make $@ BASE_DIR=$(BASE_DIR)

logfileparser.o:
	@cd $(BASE_DIR)/sensordata/$*; make $@ BASE_DIR=$(BASE_DIR)

nmeaparser.o:
	@cd $(BASE_DIR)/sensordata/$*; make $@ BASE_DIR=$(BASE_DIR)

udpparser.o:
	@cd $(BASE_DIR)/sensordata/$*; make $@ BASE_DIR=$(BASE_DIR)

simparser.o:
	@cd $(BASE_DIR)/sensordata/$*; make $@ BASE_DIR=$(BASE_DIR)


clean:
	@-rm -f ./*.o
	@-rm -f ./*~

