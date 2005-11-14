# CSTK makefile, by kristof:
# tiny and clean :)

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)

include $(BASE_DIR)/export_vars

VECTOR_DIR  =$(BASE_DIR)/cstk_base/vector

all:	bufferparser.o bufferparser_arm.o

clean:
	@-rm -f ./*.o
	@-rm -f ./*~

bufferparser.o:	datacell.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)/" -o ./$*.o 

bufferparser_arm.o:	datacell_arm.o
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./bufferparser.cpp -I"$(BASE_DIR)/" -o ./$*.o 
	
%.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)
