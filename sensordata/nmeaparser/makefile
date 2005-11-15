# CSTK makefile, by kristof:
# tiny and clean :)

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)

include $(BASE_DIR)/export_vars

VECTOR_DIR  =$(BASE_DIR)/cstk_base/vector

all:	nmeaparser.o

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
nmeaparser.o:	datacell.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)/" -o ./$*.o 
	
datacell.o:
	@cd $(VECTOR_DIR); make datacell.o BASE_DIR=$(BASE_DIR)
