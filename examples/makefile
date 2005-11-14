###################################################################################
# CSTK examples makefile, by kristof:
#

CC=g++ -Wall -O3 -g
CC_ARM=/skiff/local/bin/arm-linux-g++ -Wall -O3 -g
BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ..;pwd)
VECTOR_DIR=$(BASE_DIR)/cstk_base/vector
SD_DIR=$(BASE_DIR)/sensordata

all: 	kvplot_demo iparse_demo \
	logfileparser_demo nmeaparser_demo \
	udp_snd_demo udp_rec_demo \
	simparser_demo sensordata_demo bpnn_demo peak_demo 

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	@-rm -f ./*_demo
	
##### RS232Parser Demo ############################################################

rs232parser_demo: rs232parser_t.o
	$(CC) rs232parser_t.o $(SD_DIR)/rs232parser/rs232parser.o \
	      $(SD_DIR)/bufferparser.o \
	      $(VECTOR_DIR)/datacell.o \
	      -o ./$@
	
rs232parser_t.o: rs232parser.o datacell.o bufferparser.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

##### Porcupine Demo ############################################################

porcupine_demo: porcupine_t.o
	$(CC) porcupine_t.o $(SD_DIR)/rs232parser/rs232parser.o \
	      $(SD_DIR)/bufferparser.o \
	      $(VECTOR_DIR)/datacell.o \
	      $(BASE_DIR)/algorithms/peak/peak.o \
	      $(VECTOR_DIR)/kvector.o \
	      $(BASE_DIR)/viz/x11/kvplot.o \
	      $(BASE_DIR)/viz/x11/x11plot.o \
	      -L/usr/X11R6/lib/ -lX11 -o ./$@
	
porcupine_t.o: rs232parser.o datacell.o bufferparser.o  kvector.o peak.o kvplot.o x11plot.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

	
##### LogFileParser Demo ##########################################################

logfileparser_demo: logfileparser_t.o
	$(CC) logfileparser_t.o $(SD_DIR)/logfileparser/logfileparser.o \
	      $(SD_DIR)/bufferparser.o \
	      $(VECTOR_DIR)/datacell.o -o ./$@
	
logfileparser_t.o: logfileparser.o datacell.o bufferparser.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### NMEAParser Demo #############################################################

nmeaparser_demo: nmeaparser_t.o
	$(CC) nmeaparser_t.o $(SD_DIR)/nmeaparser/nmeaparser.o \
	      $(SD_DIR)/bufferparser.o \
	      $(SD_DIR)/logfileparser/logfileparser.o \
	      $(VECTOR_DIR)/datacell.o -o ./$@
	
nmeaparser_t.o: nmeaparser.o logfileparser.o datacell.o bufferparser.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### UDPParser Demos ############################################################

udp_snd_demo: udp_snd_t.o
	$(CC) udp_snd_t.o $(SD_DIR)/udpparser/udpparser.o \
	      $(SD_DIR)/bufferparser.o \
	      $(VECTOR_DIR)/datacell.o -o ./$@
	
udp_snd_t.o: udpparser.o datacell.o bufferparser.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

udp_rec_demo: udp_rec_t.o
	$(CC) udp_rec_t.o $(SD_DIR)/udpparser/udpparser.o \
	      $(SD_DIR)/bufferparser.o \
	      $(VECTOR_DIR)/datacell.o -o ./$@
	
udp_rec_t.o: udpparser.o datacell.o bufferparser.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### SimParser Demo #############################################################

simparser_demo: simparser_t.o
	$(CC) simparser_t.o $(SD_DIR)/simparser/simparser.o \
	      $(VECTOR_DIR)/datacell.o -o ./$@
	
simparser_t.o: simparser.o datacell.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### Sensordata Demo ############################################################

sensordata_demo: sensordata_t.o
	$(CC) $(SD_DIR)/bufferparser.o \
	      $(SD_DIR)/rs232parser/rs232parser.o \
	      $(SD_DIR)/logfileparser/logfileparser.o \
	      $(SD_DIR)/udpparser/udpparser.o \
	      $(SD_DIR)/simparser/simparser.o \
	      $(VECTOR_DIR)/datacell.o sensordata_t.o -o ./$@
	
sensordata_t.o:  rs232parser.o logfileparser.o simparser.o udpparser.o 
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### IParse Demo ################################################################

iparse_demo: iparse_t.o
	$(CC) $(BASE_DIR)/kprof/iparse.o \
	      $(BASE_DIR)/kprof/setparse.o \
	      $(SD_DIR)/rs232parser/rs232parser.o $(BASE_DIR)/kprof/rs232setparse.o \
	      $(SD_DIR)/udpparser/udpparser.o $(BASE_DIR)/kprof/udpsetparse.o \
	      $(SD_DIR)/logfileparser/logfileparser.o $(BASE_DIR)/kprof/logfilesetparse.o \
	      $(SD_DIR)/simparser/simparser.o $(BASE_DIR)/kprof/simsetparse.o \
	      $(BASE_DIR)/kprof/channelsetparse.o $(BASE_DIR)/kprof/inputcolumnsetparse.o \
	      $(VECTOR_DIR)/datacell.o \
	      $(VECTOR_DIR)/binvector.o $(VECTOR_DIR)/kvector.o $(VECTOR_DIR)/dvector.o \
	      $(SD_DIR)/bufferparser.o \
	      iparse_t.o -o ./$@

iparse_t.o: rs232parser.o logfileparser.o udpparser.o simparser.o \
	rs232setparse.o logfilesetparse.o udpsetparse.o simsetparse.o \
	channelsetparse.o inputcolumnsetparse.o bufferparser.o \
	datacell.o binvector.o kvector.o dvector.o \
	iparse.o 
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### BPNN Demo ##################################################################

bpnn_demo: bpnn_t.o
	$(CC) bpnn_t.o $(BASE_DIR)/algorithms/bpnn/bpnn.o -o ./$@
	
bpnn_t.o: bpnn.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### Peak Demo ##################################################################

peak_demo: peak_t.o
	$(CC) peak_t.o $(BASE_DIR)/algorithms/peak/peak.o \
	      $(VECTOR_DIR)/kvector.o -o ./$@
	
peak_t.o: peak.o kvector.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### KVPlot Demo ################################################################

kvplot_demo: kvplot_t.o
	$(CC) kvplot_t.o \
	      $(BASE_DIR)/algorithms/peak/peak.o \
	      $(VECTOR_DIR)/datacell.o \
	      $(VECTOR_DIR)/kvector.o \
	      $(BASE_DIR)/viz/x11/kvplot.o \
	      $(BASE_DIR)/viz/x11/x11plot.o \
	      -L/usr/X11R6/lib/ -lX11 -o ./$@
	
kvplot_demo_arm: kvplot_t_arm.o
	$(CC_ARM) kvplot_t_arm.o \
	      $(BASE_DIR)/algorithms/peak/peak_arm.o \
	      $(VECTOR_DIR)/datacell_arm.o \
	      $(VECTOR_DIR)/kvector_arm.o \
	      $(BASE_DIR)/viz/x11/kvplot_arm.o \
	      $(BASE_DIR)/viz/x11/x11plot_arm.o \
	      -L/skiff/local/arm-linux/lib/X11/ -lX11 -o ./$@
	
kvplot_t.o: datacell.o kvector.o peak.o kvplot.o x11plot.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

kvplot_t_arm.o: datacell_arm.o kvector_arm.o peak_arm.o kvplot_arm.o x11plot_arm.o
	$(CC_ARM) -c ./kvplot_t.cpp -I"$(BASE_DIR)" -o ./$@

##### Dependencies: ##############################################################

binvector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

dvector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

datacell.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)
	
datacell_arm.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)
	
kvector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

kvector_arm.o:
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

bpnn.o:
	@cd $(BASE_DIR)/algorithms/$*; make $@ BASE_DIR=$(BASE_DIR)

peak.o:
	@cd $(BASE_DIR)/algorithms/$*; make $@ BASE_DIR=$(BASE_DIR)
	
peak_arm.o:
	@cd $(BASE_DIR)/algorithms/peak; make $@ BASE_DIR=$(BASE_DIR)
	
kvplot.o:
	@cd $(BASE_DIR)/viz/x11; make $@ BASE_DIR=$(BASE_DIR)

kvplot_arm.o:
	@cd $(BASE_DIR)/viz/x11; make $@ BASE_DIR=$(BASE_DIR)

x11plot.o:
	@cd $(BASE_DIR)/viz/x11; make $@ BASE_DIR=$(BASE_DIR)

x11plot_arm.o:
	@cd $(BASE_DIR)/viz/x11; make $@ BASE_DIR=$(BASE_DIR)

iparse.o:
	@cd $(BASE_DIR)/kprof; make $@ BASE_DIR=$(BASE_DIR)

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


###################################################################################

