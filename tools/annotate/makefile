###################################################################################
# CSTK annotate makefile, by kristof:
#

CC=g++ -Wall -O3 -g
BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)
VECTOR_DIR=$(BASE_DIR)/cstk_base/vector
SD_DIR=$(BASE_DIR)/sensordata

all: 	annotate 

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	@-rm -f ./annotate
	
##### Annotate Tool ##############################################################

annotate: annotate.o
	$(CC) $(VECTOR_DIR)/datacell.o \
	      $(VECTOR_DIR)/binvector.o \
	      $(VECTOR_DIR)/kvector.o \
	      $(VECTOR_DIR)/dvector.o \
	      $(SD_DIR)/rs232parser/rs232parser.o \
	      $(SD_DIR)/logfileparser/logfileparser.o \
	      $(SD_DIR)/udpparser/udpparser.o \
	      $(SD_DIR)/simparser/simparser.o \
	      $(SD_DIR)/bufferparser.o \
	      $(BASE_DIR)/kprof/iparse.o \
	      $(BASE_DIR)/kprof/setparse.o \
	      $(BASE_DIR)/kprof/rs232setparse.o \
	      $(BASE_DIR)/kprof/logfilesetparse.o \
	      $(BASE_DIR)/kprof/udpsetparse.o \
	      $(BASE_DIR)/kprof/simsetparse.o \
	      $(BASE_DIR)/kprof/channelsetparse.o \
	      $(BASE_DIR)/kprof/inputcolumnsetparse.o \
	      annotate.o -o ./$@
	
annotate.o: iparse.o datacell.o binvector.o kvector.o dvector.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

##### Dependencies: ##############################################################

datacell.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

binvector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

kvector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

dvector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

setparse.o:
	@cd $(BASE_DIR)/kprof; make $@ BASE_DIR=$(BASE_DIR)

iparse.o:
	@cd $(BASE_DIR)/kprof; make $@ BASE_DIR=$(BASE_DIR)


###################################################################################

