# CSTK makefile, by kristof:
# tiny and clean :)

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)

include $(BASE_DIR)/export_vars

VECTOR_DIR  =$(BASE_DIR)/cstk_base/vector

all:	simparser.o simparser_arm.o
 
clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
simparser.o:	datacell.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)/" -o ./$*.o 
	
simparser_arm.o:	datacell_arm.o
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./simparser.cpp -I"$(BASE_DIR)/" -o ./$*.o 
	
datacel%.o:
	@cd $(VECTOR_DIR); make datacell.o BASE_DIR=$(BASE_DIR)
