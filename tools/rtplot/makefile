# CSTK makefile, by kristof:
# tiny and clean :)

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)

include $(BASE_DIR)/export_vars

VECTOR_DIR=$(BASE_DIR)/cstk_base/vector
SD_DIR=$(BASE_DIR)/sensordata

all:	rtplot

clean:
	@-rm -f ./*~
	@-rm -f ./*.o
	@-rm -f ./rtplot
	
##### RTPlot Tool ################################################################

rtplot:	rtplot.o
	$(CC) $(CC_FLAGS) $(VECTOR_DIR)/datacell.o \
	      $(VECTOR_DIR)/binvector.o \
	      $(VECTOR_DIR)/kvector.o \
	      $(VECTOR_DIR)/dvector.o \
	      $(BASE_DIR)/kprof/iparse.o \
	       $(SD_DIR)/rs232parser/rs232parser.o \
	       $(SD_DIR)/logfileparser/logfileparser.o \
	       $(SD_DIR)/udpparser/udpparser.o \
	       $(SD_DIR)/simparser/simparser.o \
	       $(SD_DIR)/bufferparser/bufferparser.o \
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
	      $(BASE_DIR)/algorithms/peak/peak.o  \
	      rtplot.o -L/usr/X11R6/lib/ -lX11 -o ./$@

rtplot.o: datacell.o binvector.o kvector.o dvector.o iparse.o wparse.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### Dependencies: ##############################################################

%vector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

datacell.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)
	
%parse.o:
	@cd $(BASE_DIR)/kprof; make $@ BASE_DIR=$(BASE_DIR)

