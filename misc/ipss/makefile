# CSTK ipss makefile, by kristof:
#

CC=g++ -Wall -O3 -g
BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ..;pwd)
SUBDIRS = $(notdir $(subst .,,$(shell find . -maxdepth 1 -type d)))

all:	ipss

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	@-rm -f ./ipss

ipss:	
	$(CC) ipss.cpp -o ./ipss	
