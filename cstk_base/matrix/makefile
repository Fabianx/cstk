# CSTK makefile, by kristof:
# tiny and clean :)

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)

include $(BASE_DIR)/export_vars

all:	dvectormatrix.o dvectormatrix_arm.o

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
dvectormatrix.o:
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$*.o

dvectormatrix_arm.o:
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./dvectormatrix.cpp -I"$(BASE_DIR)" -o ./$*.o
