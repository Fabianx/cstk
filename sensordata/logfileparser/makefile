# CSTK makefile, by kristof:
# tiny and clean :)

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)

include $(BASE_DIR)/export_vars

VECTOR_DIR  =$(BASE_DIR)/cstk_base/vector

all:	logfileparser.o

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
logfileparser.o:	datacell.o bufferparser.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)/" -o ./$*.o 
	
logfileparser.o:	datacell_arm.o bufferparser_arm.o
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./$*.cpp -I"$(BASE_DIR)/" -o ./$*.o 
	
datacell.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

bufferparser.o:
	@cd $(BASE_DIR)/sensordata/bufferparser; make $@ BASE_DIR=$(BASE_DIR)

datacell_arm.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

bufferparser_arm.o:
	@cd $(BASE_DIR)/sensordata/bufferparser; make $@ BASE_DIR=$(BASE_DIR)
