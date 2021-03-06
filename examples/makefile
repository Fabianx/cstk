# CSTK makefile, by kristof:
# tiny and clean :)

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ..;pwd)

include $(BASE_DIR)/export_vars

VECTOR_DIR=$(BASE_DIR)/cstk_base/vector
SD_DIR=$(BASE_DIR)/sensordata

all: 	kvplot_demo iparse_demo logfileparser_demo nmeaparser_demo \
	udp_snd_demo udp_rec_demo \
	simparser_demo sensordata_demo peak_demo \
	bpnn_demo 

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	@-rm -f ./*_demo
	
##### PorcLogger ##################################################################

porclogger: porclogger.o
	$(CC) $(CC_FLAGS) porclogger.o $(SD_DIR)/rs232parser/rs232parser.o \
	      $(SD_DIR)/bufferparser/bufferparser.o \
	      $(VECTOR_DIR)/datacell.o \
	      $(VECTOR_DIR)/kvector.o \
	      -o ./$@
	
porclogger.o: rs232parser.o datacell.o bufferparser.o  kvector.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

##### Kubeviewer ##################################################################

kubeviewer: kubeviewer.o
	$(CC) $(CC_FLAGS) kubeviewer.o $(SD_DIR)/rs232parser/rs232parser.o \
	      $(SD_DIR)/bufferparser/bufferparser.o \
	      $(BASE_DIR)/algorithms/peak/peak.o \
	      $(VECTOR_DIR)/datacell.o \
	      $(VECTOR_DIR)/kvector.o \
	      $(BASE_DIR)/viz/x11/kvplot.o \
	      $(BASE_DIR)/viz/x11/x11plot.o \
	      $(CC_X11) -o ./$@
	
kubeviewer.o: rs232parser.o datacell.o bufferparser.o  kvector.o peak.o kvplot.o x11plot.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

##### Porc2viewer #################################################################

porc2viewer: porc2viewer.o
	$(CC) $(CC_FLAGS) porc2viewer.o $(SD_DIR)/rs232parser/rs232parser.o \
	      $(SD_DIR)/bufferparser/bufferparser.o \
	      $(BASE_DIR)/algorithms/peak/peak.o \
	      $(VECTOR_DIR)/datacell.o \
	      $(VECTOR_DIR)/kvector.o \
	      $(BASE_DIR)/viz/x11/kvplot.o \
	      $(BASE_DIR)/viz/x11/x11plot.o \
	      $(CC_X11) -o ./$@
	
porc2viewer.o: rs232parser.o datacell.o bufferparser.o  kvector.o peak.o kvplot.o x11plot.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

##### RS232Parser Demo ############################################################

rs232parser_demo: rs232parser_t.o
	$(CC) $(CC_FLAGS) rs232parser_t.o $(SD_DIR)/rs232parser/rs232parser.o \
	      $(SD_DIR)/bufferparser/bufferparser.o \
	      $(BASE_DIR)/algorithms/peak/peak.o \
	      $(VECTOR_DIR)/datacell.o \
	      $(VECTOR_DIR)/kvector.o \
	      $(BASE_DIR)/viz/x11/kvplot.o \
	      $(BASE_DIR)/viz/x11/x11plot.o \
	      $(CC_X11) -o ./$@
	
rs232parser_t.o: rs232parser.o datacell.o bufferparser.o  kvector.o peak.o kvplot.o x11plot.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

##### LogFileParser Demo ##########################################################

logfileparser_demo: logfileparser_t.o
	$(CC) $(CC_FLAGS) logfileparser_t.o $(SD_DIR)/logfileparser/logfileparser.o \
	      $(SD_DIR)/bufferparser/bufferparser.o \
	      $(VECTOR_DIR)/datacell.o -o ./$@
	
logfileparser_t.o: logfileparser.o datacell.o bufferparser.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### NMEAParser Demo #############################################################

nmeaparser_demo: nmeaparser_t.o
	$(CC) $(CC_FLAGS) nmeaparser_t.o $(SD_DIR)/nmeaparser/nmeaparser.o \
	      $(SD_DIR)/bufferparser/bufferparser.o \
	      $(SD_DIR)/logfileparser/logfileparser.o \
	      $(VECTOR_DIR)/datacell.o -o ./$@
	
nmeaparser_t.o: nmeaparser.o logfileparser.o datacell.o bufferparser.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### UDPParser Demos ############################################################

udp_snd_demo: udp_snd_t.o
	$(CC) $(CC_FLAGS) udp_snd_t.o $(SD_DIR)/udpparser/udpparser.o \
	      $(SD_DIR)/bufferparser/bufferparser.o \
	      $(VECTOR_DIR)/datacell.o -o ./$@
	
udp_snd_t.o: udpparser.o datacell.o bufferparser.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

udp_rec_demo: udp_rec_t.o
	$(CC) $(CC_FLAGS) udp_rec_t.o $(SD_DIR)/udpparser/udpparser.o \
	      $(SD_DIR)/bufferparser/bufferparser.o \
	      $(VECTOR_DIR)/datacell.o -o ./$@
	
udp_rec_t.o: udpparser.o datacell.o bufferparser.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### SimParser Demo #############################################################

simparser_demo: simparser_t.o
	$(CC) $(CC_FLAGS) simparser_t.o $(SD_DIR)/simparser/simparser.o \
	      $(VECTOR_DIR)/datacell.o -o ./$@
	
simparser_t.o: simparser.o datacell.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### Sensordata Demo ############################################################

sensordata_demo: sensordata_t.o
	$(CC) $(CC_FLAGS) $(SD_DIR)/bufferparser/bufferparser.o \
	      $(SD_DIR)/rs232parser/rs232parser.o \
	      $(SD_DIR)/logfileparser/logfileparser.o \
	      $(SD_DIR)/udpparser/udpparser.o \
	      $(SD_DIR)/simparser/simparser.o \
	      $(VECTOR_DIR)/datacell.o sensordata_t.o -o ./$@
	
sensordata_t.o:  rs232parser.o logfileparser.o simparser.o udpparser.o 
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### IParse Demo ################################################################

iparse_demo: iparse_t.o
	$(CC) $(CC_FLAGS) $(BASE_DIR)/kprof/iparse.o \
	      $(BASE_DIR)/kprof/setparse.o \
	      $(SD_DIR)/rs232parser/rs232parser.o $(BASE_DIR)/kprof/rs232setparse.o \
	      $(SD_DIR)/udpparser/udpparser.o $(BASE_DIR)/kprof/udpsetparse.o \
	      $(SD_DIR)/logfileparser/logfileparser.o $(BASE_DIR)/kprof/logfilesetparse.o \
	      $(SD_DIR)/simparser/simparser.o $(BASE_DIR)/kprof/simsetparse.o \
	      $(BASE_DIR)/kprof/channelsetparse.o $(BASE_DIR)/kprof/inputcolumnsetparse.o \
	      $(VECTOR_DIR)/datacell.o \
	      $(VECTOR_DIR)/binvector.o $(VECTOR_DIR)/kvector.o $(VECTOR_DIR)/dvector.o \
	      $(SD_DIR)/bufferparser/bufferparser.o \
	      iparse_t.o -o ./$@

iparse_t.o: rs232parser.o logfileparser.o udpparser.o simparser.o \
	rs232setparse.o logfilesetparse.o udpsetparse.o simsetparse.o \
	channelsetparse.o inputcolumnsetparse.o bufferparser.o \
	datacell.o binvector.o kvector.o dvector.o \
	iparse.o 
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### BPNN Demo ##################################################################

bpnn_demo: bpnn_t.o
	$(CC) $(CC_FLAGS) bpnn_t.o $(BASE_DIR)/algorithms/bpnn/bpnn.o -o ./$@
	
bpnn_t.o: bpnn.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### Peak Demo ##################################################################

peak_demo: peak_t.o
	$(CC) $(CC_FLAGS) peak_t.o $(BASE_DIR)/algorithms/peak/peak.o \
	      $(VECTOR_DIR)/kvector.o -o ./$@
	
peak_t.o: peak.o kvector.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### KVPlot Demo ################################################################

kvplot_demo: kvplot_t.o
	$(CC) $(CC_FLAGS) $(CC_FLAGS) kvplot_t.o \
	      $(BASE_DIR)/algorithms/peak/peak.o \
	      $(VECTOR_DIR)/datacell.o \
	      $(VECTOR_DIR)/kvector.o \
	      $(BASE_DIR)/viz/x11/kvplot.o \
	      $(BASE_DIR)/viz/x11/x11plot.o \
	      $(CC_X11) -o ./$@
	
kvplot_demo_arm: kvplot_t_arm.o
	$(CC_ARM) $(CC_FLAGS_ARM) $(CC_FLAGS_ARM) kvplot_t_arm.o \
	      $(BASE_DIR)/algorithms/peak/peak_arm.o \
	      $(VECTOR_DIR)/datacell_arm.o \
	      $(VECTOR_DIR)/kvector_arm.o \
	      $(BASE_DIR)/viz/x11/kvplot_arm.o \
	      $(BASE_DIR)/viz/x11/x11plot_arm.o \
	      $(CC_X11_ARM) -o ./$@
	
kvplot_t.o: datacell.o kvector.o peak.o kvplot.o x11plot.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

kvplot_t_arm.o: datacell_arm.o kvector_arm.o peak_arm.o kvplot_arm.o x11plot_arm.o
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./kvplot_t.cpp -I"$(BASE_DIR)" -o ./$@

##### Dependencies: ##############################################################

%vector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

%vector_arm.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

datacell.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)
	
datacell_arm.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)
	
kprof.o:
	@cd $(BASE_DIR)/$*;make $@ BASE_DIR=$(BASE_DIR)

bpnn.o:
	@cd $(BASE_DIR)/algorithms/$*; make $@ BASE_DIR=$(BASE_DIR)

peak.o:
	@cd $(BASE_DIR)/algorithms/$*; make $@ BASE_DIR=$(BASE_DIR)
	
peak_arm.o:
	@cd $(BASE_DIR)/algorithms/peak; make $@ BASE_DIR=$(BASE_DIR)
	
%parser.o:
	@cd $(BASE_DIR)/sensordata/$*parser; make $@ BASE_DIR=$(BASE_DIR)

%parser_arm.o:
	@cd $(BASE_DIR)/sensordata/$*parser; make $@ BASE_DIR=$(BASE_DIR)

%plot.o:
	@cd $(BASE_DIR)/viz/x11; make $@ BASE_DIR=$(BASE_DIR)

%plot_arm.o:
	@cd $(BASE_DIR)/viz/x11; make $@ BASE_DIR=$(BASE_DIR)

%parse.o:
	@cd $(BASE_DIR)/kprof; make $@ BASE_DIR=$(BASE_DIR)

%parse_arm.o:
	@cd $(BASE_DIR)/kprof; make $@ BASE_DIR=$(BASE_DIR)


###################################################################################

