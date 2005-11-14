# CSTK makefile, by kristof:
# tiny and clean :)

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ..;pwd)

include $(BASE_DIR)/export_vars

VECTOR_DIR=$(BASE_DIR)/cstk_base/vector

all:	setparse.o rs232setparse.o udpsetparse.o \
	logfilesetparse.o simsetparse.o channelsetparse.o \
	windowsetparse.o plotsetparse.o ksomsetparse.o \
	iparse.o wparse.o pparse.o

clean:
	@-rm -f ./*.o
	@-rm -f ./*~

##### Input Parsing ###############################################################

iparse.o: datacell.o binvector.o kvector.o dvector.o bufferparser.o \
	rs232parser.o udpparser.o logfileparser.o simparser.o \
	rs232setparse.o udpsetparse.o logfilesetparse.o simsetparse.o \
	channelsetparse.o inputcolumnsetparse.o 
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### Window Parsing ##############################################################

wparse.o: kvector.o peak.o plotsetparse.o windowsetparse.o \
	ksomsetparse.o kvplot.o clustplot.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### Params Parsing ##############################################################

%parse.o: setparse.o
	$(CC) $(CC_FLAGS) -c ./$*parse.cpp -I"$(BASE_DIR)" -o ./$@

###################################################################################

kprof.o:	peak.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@


##### Dependencies: ##############################################################
	
peak.o:
	@cd $(BASE_DIR)/algorithms/$*; make $@ BASE_DIR=$(BASE_DIR)

datacell.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)
	
%vector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

%parser.o:
	@cd $(BASE_DIR)/sensordata/$*parser; make $@ BASE_DIR=$(BASE_DIR)

%plot.o:
	@cd $(BASE_DIR)/viz/x11; make $@ BASE_DIR=$(BASE_DIR)

