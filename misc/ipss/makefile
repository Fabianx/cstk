# CSTK ipss makefile, by kristof:
#

CC=g++ -Wall -O3 -g
SUBDIRS = $(notdir $(subst .,,$(shell find . -maxdepth 1 -type d)))

all:	ipss

clean:
	@-rm -f ./*.o
	@-rm -f ./*~

ipss:	
	$(CC) ipss.cpp -o ./ipss	