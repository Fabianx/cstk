# CSTK sensordata makefile, by kristof
#

CC=g++ -Wall -O3 -g
BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ..;pwd)
SUBDIRS = $(notdir $(subst .,,$(shell find . -maxdepth 1 -type d)))
VECTOR_DIR  =$(BASE_DIR)/cstk_base/vector

all:	bufferparser.o
	@-for i in $(SUBDIRS); do cd $$i; make all BASE_DIR=$(BASE_DIR); cd ..; done

clean:
	@-rm -f ./*~
	@-for i in $(SUBDIRS); do cd $$i; make clean; cd ..; done

bufferparser.o:	datacell.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)/" -o ./$*.o 
	
datacell.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)
