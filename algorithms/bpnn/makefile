# CSTK makefile, by kristof:
# tiny and clean :)

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)

include $(BASE_DIR)/export_vars

all:	bpnn.o bpnn_arm.o

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
bpnn.o: 
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

bpnn_arm.o: 
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./bpnn.cpp -I"$(BASE_DIR)" -o ./$@

