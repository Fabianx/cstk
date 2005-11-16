# CSTK makefile, by kristof:
# tiny and clean :)

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)

include $(BASE_DIR)/export_vars

all:	sdm.o sdma.o sdmafct.o sosdm.o \
	sdm_arm.o sdma_arm.o sdmafct_arm.o sosdm_arm.o

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
sdm.o: 
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

sdma.o: 
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

sdmafct.o: 
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

sosdm.o: 
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

sdm_arm.o: 
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./sdm.cpp -I"$(BASE_DIR)" -o ./$@

sdma_arm.o: 
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./sdma.cpp -I"$(BASE_DIR)" -o ./$@

sdmafct_arm.o: 
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./sdmafct.cpp -I"$(BASE_DIR)" -o ./$@

sosdm_arm.o: 
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./sosdm.cpp -I"$(BASE_DIR)" -o ./$@
