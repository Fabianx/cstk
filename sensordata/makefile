# CSTK makefile, by kristof:
# tiny and clean :)

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ..;pwd)

include $(BASE_DIR)/export_vars

SUBDIRS = $(notdir $(subst .,,$(shell find . -maxdepth 1 -type d)))

all:	
	@-for i in $(SUBDIRS); do cd $$i; make all BASE_DIR=$(BASE_DIR); cd ..; done

clean:
	@-rm -f ./*~
	@-for i in $(SUBDIRS); do cd $$i; make clean; cd ..; done

