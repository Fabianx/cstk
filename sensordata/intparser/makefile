# CSTK tmp makefile, by kristof
#

CC=g++ -Wall -O3 -g
SUBDIRS = $(notdir $(subst .,,$(shell find . -maxdepth 1 -type d)))

all:
	@-for i in $(SUBDIRS); do cd $$i; make all BASE_DIR=$(BASE_DIR); cd ..; done

clean:
	@-rm -f ./*~
	@-for i in $(SUBDIRS); do cd $$i; make clean; cd ..; done
	