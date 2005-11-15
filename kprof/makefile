# CSTK makefile, by kristof:
# tiny and clean :)

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ..;pwd)

include $(BASE_DIR)/export_vars

VECTOR_DIR=$(BASE_DIR)/cstk_base/vector

all:	setparse.o rs232setparse.o udpsetparse.o \
	logfilesetparse.o simsetparse.o channelsetparse.o \
	windowsetparse.o plotsetparse.o ksomsetparse.o \
	iparse.o wparse.o pparse.o \
	setparse_arm rs232setparse_arm.o udpsetparse_arm.o \
	logfilesetparse_arm.o simsetparse_arm.o channelsetparse_arm.o \
	windowsetparse_arm.o plotsetparse_arm.o ksomsetparse_arm.o \
	iparse_arm.o wparse_arm.o pparse_arm.o

clean:
	@-rm -f ./*.o
	@-rm -f ./*~

##### Input Parsing ###############################################################

iparse.o: datacell.o binvector.o kvector.o dvector.o bufferparser.o \
	rs232parser.o udpparser.o logfileparser.o simparser.o \
	rs232setparse.o udpsetparse.o logfilesetparse.o simsetparse.o \
	channelsetparse.o inputcolumnsetparse.o 
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

iparse_arm.o: datacell_arm.o binvector_arm.o kvector_arm.o dvector_arm.o bufferparser_arm.o \
	rs232parser_arm.o udpparser_arm.o logfileparser_arm.o simparser_arm.o \
	rs232setparse_arm.o udpsetparse_arm.o logfilesetparse_arm.o simsetparse_arm.o \
	channelsetparse_arm.o inputcolumnsetparse_arm.o 
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./iparse.cpp -I"$(BASE_DIR)" -o ./$@

##### Window Parsing ##############################################################

wparse.o: kvector.o peak.o plotsetparse.o windowsetparse.o \
	ksomsetparse.o kvplot.o clustplot.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

wparse_arm.o: kvector_arm.o peak_arm.o plotsetparse_arm.o windowsetparse_arm.o \
	ksomsetparse_arm.o kvplot_arm.o clustplot_arm.o
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./wparse.cpp -I"$(BASE_DIR)" -o ./$@

##### Params Parsing ##############################################################

setparse.o:
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

setparse_arm.o:
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./setparse.cpp -I"$(BASE_DIR)" -o ./$@

%parse.o: setparse.o
	$(CC) $(CC_FLAGS) -c ./$*parse.cpp -I"$(BASE_DIR)" -o ./$@

%parse_arm.o: setparse_arm.o
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./$*parse.cpp -I"$(BASE_DIR)" -o ./$@

###################################################################################

kprof.o:	peak.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

kprof_arm.o:	peak.o
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@


##### Dependencies: ##############################################################
	
peak.o:
	@cd $(BASE_DIR)/algorithms/$*; make $@ BASE_DIR=$(BASE_DIR)

peak_arm.o:
	@cd $(BASE_DIR)/algorithms/peak; make $@ BASE_DIR=$(BASE_DIR)

datacell.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)
	
datacell_arm.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)
	
%vector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

%vector_arm.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

%parser.o:
	@cd $(BASE_DIR)/sensordata/$*parser; make $@ BASE_DIR=$(BASE_DIR)

%parser_arm.o:
	@cd $(BASE_DIR)/sensordata/$*parser_arm; make $@ BASE_DIR=$(BASE_DIR)

%plot.o:
	@cd $(BASE_DIR)/viz/x11; make $@ BASE_DIR=$(BASE_DIR)

%plot_arm.o:
	@cd $(BASE_DIR)/viz/x11; make $@ BASE_DIR=$(BASE_DIR)

