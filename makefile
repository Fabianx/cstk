# CSTK main makefile, by kristof:
#

CC=g++ -Wall -O3 -g
BASE_DIR = $(shell pwd)
SUBDIRS = $(notdir $(subst .,,$(shell find . -maxdepth 1 -type d)))

install:	all
	@-mkdir -p $(BASE_DIR)/../bin
	@-mv $(BASE_DIR)/tools/rtplot/rtplot $(BASE_DIR)/../bin
	@-mv $(BASE_DIR)/tools/topoplot/topoplot $(BASE_DIR)/../bin
	@-mv $(BASE_DIR)/gui/*.py $(BASE_DIR)/../bin
	

all:
	-@echo home directory is $(BASE_DIR)
	-@echo "BASE_DIR=$(BASE_DIR)" > .dirs
	@-for i in $(SUBDIRS); do cd $$i; make all BASE_DIR=$(BASE_DIR); cd ..; done

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	@-for i in $(SUBDIRS); do cd $$i; make clean; cd ..; done
	