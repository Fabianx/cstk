# CSTK main makefile, by kristof:
#

CC=g++ -Wall -O3 -g
BASE_DIR = $(shell pwd)
SUBDIRS = $(notdir $(subst .,,$(shell find . -maxdepth 1 -type d)))

all:
	-@echo home directory is $(BASE_DIR)
	-@echo "BASE_DIR=$(BASE_DIR)" > .dirs
	@-for i in $(SUBDIRS); do cd $$i; make all BASE_DIR=$(BASE_DIR); cd ..; done

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	@-for i in $(SUBDIRS); do cd $$i; make clean; cd ..; done
	