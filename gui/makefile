# CSTK gui makefile, by kristof:
#

BASEDIR = $(shell pwd)
SUBDIRS = $(notdir $(subst .,,$(shell find . -maxdepth 1 -type d)))

all:
	@-chmod a+x *.py

clean:
	@-rm -f ./*.pyc
	@-rm -f ./*~
	