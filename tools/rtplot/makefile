# CSTK rtplot makefile, by kristof
#

CC=g++ -Wall -O3 -g
BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)
VECTOR_DIR=$(BASE_DIR)/cstk_base/vector
SD_DIR=$(BASE_DIR)/sensordata

RTPLOT_DEP = rs232parser.o logfileparser.o udpparser.o simparser.o \
	rs232setparse.o udpsetparse.o logfilesetparse.o simsetparse.o \
	channelsetparse.o inputcolumnsetparse.o plotsetparse.o \
	windowsetparse.o bufferparser.o datacell.o kprof.o kvector.o \
	kvplot.o

all:	rtplot

clean:
	@-rm -f ./*~
	@-rm -f ./*.o
	@-rm -f ./rtplot
	
rtplot:	rtplot.o
	$(CC) $(BASE_DIR)/kprof/kprof.o \
	      $(SD_DIR)/rs232parser/rs232parser.o \
	      $(VECTOR_DIR)/datacell.o \
	      $(VECTOR_DIR)/kvector.o \
	      $(BASE_DIR)/algorithms/peak/peak.o \
	      $(SD_DIR)/logfileparser/logfileparser.o \
	      $(SD_DIR)/udpparser/udpparser.o \
	      $(SD_DIR)/simparser/simparser.o \
	      $(SD_DIR)/bufferparser.o \
	      $(BASE_DIR)/kprof/setparse.o \
	      $(BASE_DIR)/kprof/rs232setparse.o \
	      $(BASE_DIR)/kprof/udpsetparse.o \
	      $(BASE_DIR)/kprof/logfilesetparse.o \
	      $(BASE_DIR)/kprof/simsetparse.o \
	      $(BASE_DIR)/kprof/channelsetparse.o \
	      $(BASE_DIR)/kprof/inputcolumnsetparse.o \
	      $(BASE_DIR)/kprof/windowsetparse.o \
	      $(BASE_DIR)/kprof/plotsetparse.o \
	      $(BASE_DIR)/viz/x11/x11plot.o \
	      $(BASE_DIR)/viz/x11/kvplot.o \
	      rtplot.o -L/usr/X11R6/lib/ -lX11 -o ./$@

rtplot.o: $(RTPLOT_DEP)
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@
	
##### Dependencies: ##############################################################

binvector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

dvector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

datacell.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)
	
kvector.o:
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

kprof.o:
	@cd $(BASE_DIR)/$*; make $@ BASE_DIR=$(BASE_DIR)

peak.o:
	@cd $(BASE_DIR)/algorithms/$*; make $@ BASE_DIR=$(BASE_DIR)
	
kvplot.o:
	@cd $(BASE_DIR)/viz/x11; make $@ BASE_DIR=$(BASE_DIR)

rs232setparse.o:
	@cd $(BASE_DIR)/kprof; make $@ BASE_DIR=$(BASE_DIR)
		
udpsetparse.o:
	@cd $(BASE_DIR)/kprof; make $@ BASE_DIR=$(BASE_DIR)
		
logfilesetparse.o:
	@cd $(BASE_DIR)/kprof; make $@ BASE_DIR=$(BASE_DIR)
		
simsetparse.o:
	@cd $(BASE_DIR)/kprof; make $@ BASE_DIR=$(BASE_DIR)
		
channelsetparse.o:
	@cd $(BASE_DIR)/kprof; make $@ BASE_DIR=$(BASE_DIR)
		
inputcolumnsetparse.o:
	@cd $(BASE_DIR)/kprof; make $@ BASE_DIR=$(BASE_DIR)
		
windowsetparse.o:
	@cd $(BASE_DIR)/kprof; make $@ BASE_DIR=$(BASE_DIR)
		
plotsetparse.o:
	@cd $(BASE_DIR)/kprof; make $@ BASE_DIR=$(BASE_DIR)
		
setparse.o:
	@cd $(BASE_DIR)/kprof; make $@ BASE_DIR=$(BASE_DIR)

