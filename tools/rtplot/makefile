# CSTK rtplot makefile, by kristof
#

CC=g++ -Wall -O3 -g
BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)
VECTOR_DIR=$(BASE_DIR)/cstk_base/vector
SD_DIR=$(BASE_DIR)/sensordata

RTPLOT_DEP = peak.o kvector.o kvplot.o datacell.o kprof.o rs232parser.o \
	udpparser.o logfileparser.o simparser.o

all:	rtplot

clean:
	@-rm -f ./*~
	
rtplot:	rtplot.o
	$(CC)  -L/usr/X11R6/lib/ -lX11 \
	$(VECTOR_DIR)/kvector.o rtplot.o $(BASE_DIR)/algorithms/peak/peak.o \
	$(BASE_DIR)/viz/x11/kvplot.o $(BASE_DIR)/viz/x11/x11plot.o \
	$(BASE_DIR)/kprof/kprof.o $(SD_DIR)/rs232parser/rs232parser.o \
	$(SD_DIR)/logfileparser/logfileparser.o \
	$(SD_DIR)/udpparser/udpparser.o \
	$(SD_DIR)/simparser/simparser.o \
	$(VECTOR_DIR)/datacell.o \
	-o ./$@

rtplot.o: $(RTPLOT_DEP)
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

datacell.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

kvector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

rs232parser.o:
	@cd $(BASE_DIR)/sensordata/$*; make $@ BASE_DIR=$(BASE_DIR)

logfileparser.o:
	@cd $(BASE_DIR)/sensordata/$*; make $@ BASE_DIR=$(BASE_DIR)

udpparser.o:
	@cd $(BASE_DIR)/sensordata/$*; make $@ BASE_DIR=$(BASE_DIR)

simparser.o:
	@cd $(BASE_DIR)/sensordata/$*; make $@ BASE_DIR=$(BASE_DIR)

kprof.o:
	@cd $(BASE_DIR)/$*; make $@ BASE_DIR=$(BASE_DIR)

peak.o:
	@cd $(BASE_DIR)/algorithms/$*; make $@ BASE_DIR=$(BASE_DIR)
	
kvplot.o:
	@cd $(BASE_DIR)/viz/x11; make $@ BASE_DIR=$(BASE_DIR)
	
