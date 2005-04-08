# CSTK topoplot makefile, by kristof
#

CC=g++ -Wall -O3 -g
BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)
VECTOR_DIR=$(BASE_DIR)/cstk_base/vector
SD_DIR=$(BASE_DIR)/sensordata

2DPLOT_DEP = datacell.o kvector.o dvector.o iparse.o wparse.o pparse.o gng.o

all:	2Dplot

clean:
	@-rm -f ./*~
	@-rm -f ./*.o
	@-rm -f ./2Dplot.o
	
2Dplot:	2Dplot.o
	$(CC) $(VECTOR_DIR)/datacell.o \
	      $(VECTOR_DIR)/binvector.o \
	      $(VECTOR_DIR)/kvector.o \
	      $(VECTOR_DIR)/dvector.o \
	      $(BASE_DIR)/kprof/iparse.o \
	       $(SD_DIR)/rs232parser/rs232parser.o \
	       $(SD_DIR)/logfileparser/logfileparser.o \
	       $(SD_DIR)/udpparser/udpparser.o \
	       $(SD_DIR)/simparser/simparser.o \
	       $(SD_DIR)/bufferparser.o \
	       $(BASE_DIR)/kprof/setparse.o \
	       $(BASE_DIR)/kprof/rs232setparse.o \
	       $(BASE_DIR)/kprof/logfilesetparse.o \
	       $(BASE_DIR)/kprof/udpsetparse.o \
	       $(BASE_DIR)/kprof/simsetparse.o \
	       $(BASE_DIR)/kprof/channelsetparse.o \
	       $(BASE_DIR)/kprof/inputcolumnsetparse.o \
	      $(BASE_DIR)/kprof/wparse.o \
	       $(BASE_DIR)/viz/x11/x11plot.o \
	       $(BASE_DIR)/viz/x11/kvplot.o \
	       $(BASE_DIR)/viz/x11/clustplot.o \
	       $(BASE_DIR)/kprof/windowsetparse.o \
	       $(BASE_DIR)/kprof/plotsetparse.o \
	      $(BASE_DIR)/kprof/pparse.o \
	      $(BASE_DIR)/algorithms/gng/gng.o \
	      $(BASE_DIR)/algorithms/peak/peak.o  \
	      2Dplot.o -L/usr/X11R6/lib/ -lX11 -o ./$@

2Dplot.o: $(2DPLOT_DEP)
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

datacell.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

dvector.o:
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

gng.o:
	@cd $(BASE_DIR)/algorithms/$*; make $@ BASE_DIR=$(BASE_DIR)

iparse.o:
	@cd $(BASE_DIR)/kprof; make $@ BASE_DIR=$(BASE_DIR)

wparse.o:
	@cd $(BASE_DIR)/kprof; make $@ BASE_DIR=$(BASE_DIR)

pparse.o:
	@cd $(BASE_DIR)/kprof; make $@ BASE_DIR=$(BASE_DIR)

###################################################################################