# CSTK tools makefile, by kristof
#

CC=g++ -Wall -O3 -g
BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ..;pwd)
SUBDIRS = $(notdir $(subst .,,$(shell find . -maxdepth 1 -type d)))

all:
	@-for i in $(SUBDIRS); do cd $$i; make all BASE_DIR=$(BASE_DIR); cd ..; done

clean:
	@-rm -f ./*~
	@-for i in $(SUBDIRS); do cd $$i; make clean; cd ..; done
	
	