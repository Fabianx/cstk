###################################################################################
# CSTK examples makefile, by kristof:
#

CC=g++ -Wall -O3 -g

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ..;pwd)
VECTOR_DIR=$(BASE_DIR)/cstk_base/vector
SD_DIR=$(BASE_DIR)/sensordata

all: 	rs232parser_demo setparam_demo \
	logfileparser_demo nmeaparser_demo udp_snd_demo udp_rec_demo \
	simparser_demo sensordata_demo kprof_demo bpnn_demo peak_demo kvplot_demo \

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	@-rm -f ./*_demo
	
##### RS232Parser Demo ############################################################

rs232parser_demo: rs232parser_t.o
	$(CC) rs232parser_t.o $(SD_DIR)/rs232parser/rs232parser.o \
	      $(VECTOR_DIR)/datacell.o \
	      -o ./$@
	
rs232parser_t.o: rs232parser.o datacell.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

##### LogFileParser Demo ##########################################################

logfileparser_demo: logfileparser_t.o
	$(CC) logfileparser_t.o $(SD_DIR)/logfileparser/logfileparser.o \
	      $(VECTOR_DIR)/datacell.o -o ./$@
	
logfileparser_t.o: logfileparser.o datacell.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### NMEAParser Demo #############################################################

nmeaparser_demo: nmeaparser_t.o
	$(CC) nmeaparser_t.o $(SD_DIR)/nmeaparser/nmeaparser.o \
	      $(SD_DIR)/logfileparser/logfileparser.o \
	      $(VECTOR_DIR)/datacell.o -o ./$@
	
nmeaparser_t.o: nmeaparser.o logfileparser.o datacell.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### UDPParser Demos ############################################################

udp_snd_demo: udp_snd_t.o
	$(CC) udp_snd_t.o $(SD_DIR)/udpparser/udpparser.o \
	      $(VECTOR_DIR)/datacell.o -o ./$@
	
udp_snd_t.o: udpparser.o datacell.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

udp_rec_demo: udp_rec_t.o
	$(CC) udp_rec_t.o $(SD_DIR)/udpparser/udpparser.o \
	      $(VECTOR_DIR)/datacell.o -o ./$@
	
udp_rec_t.o: udpparser.o datacell.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### SimParser Demo #############################################################

simparser_demo: simparser_t.o
	$(CC) simparser_t.o $(SD_DIR)/simparser/simparser.o \
	      $(VECTOR_DIR)/datacell.o -o ./$@
	
simparser_t.o: simparser.o datacell.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### Sensordata Demo ############################################################

sensordata_demo: sensordata_t.o
	$(CC) $(SD_DIR)/rs232parser/rs232parser.o \
	      $(SD_DIR)/logfileparser/logfileparser.o \
	      $(SD_DIR)/udpparser/udpparser.o \
	      $(SD_DIR)/simparser/simparser.o \
	      $(VECTOR_DIR)/datacell.o sensordata_t.o -o ./$@
	
sensordata_t.o:  rs232parser.o logfileparser.o simparser.o udpparser.o 
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### KProf Demo #################################################################

kprof_demo: kprof_t.o
	$(CC) kprof_t.o $(BASE_DIR)/kprof/kprof.o -o ./$@
	
kprof_t.o: kprof.o
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

##### SetParam Demo ##############################################################

setparam_demo: setparam_t.o
	$(CC) $(SD_DIR)/rs232parser/rs232parser.o \
	      $(VECTOR_DIR)/datacell.o \
	      $(SD_DIR)/logfileparser/logfileparser.o \
	      $(SD_DIR)/udpparser/udpparser.o \
	      $(SD_DIR)/simparser/simparser.o \
	      $(BASE_DIR)/kprof/rs232setparse.o \
	      setparam_t.o -o ./$@
	
setparam_t.o: rs232parser.o logfileparser.o udpparser.o simparser.o \
	rs232setparse.o datacell.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### KVPlot Demo ################################################################

kvplot_demo: kvplot_t.o
	$(CC) kvplot_t.o $(BASE_DIR)/algorithms/peak/peak.o \
	      $(BASE_DIR)/viz/x11/kvplot.o $(BASE_DIR)/viz/x11/x11plot.o \
	      $(VECTOR_DIR)/kvector.o -L/usr/X11R6/lib/ -lX11 -o ./$@
	
kvplot_t.o: peak.o kvector.o kvplot.o
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
	
kvplot.o:
	@cd $(BASE_DIR)/viz/x11; make $@ BASE_DIR=$(BASE_DIR)

rs232setparse.o:
	@cd $(BASE_DIR)/kprof; make $@ BASE_DIR=$(BASE_DIR)
		

###################################################################################

