# CSTK makefile, by kristof:
# tiny and clean :)

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ..;pwd)

include $(BASE_DIR)/export_vars

all:	binvector.o datacell.o dvector.o kvector.o \
	binvector_arm.o datacell_arm.o dvector_arm.o kvector_arm.o

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
%vector.o:
	$(CC) -c ./$*vector.cpp -I"$(BASE_DIR)" -o ./$@

%vector_arm.o:
	$(CC_ARM) -c ./$*vector.cpp -I"$(BASE_DIR)" -o ./$@

datacell.o:
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

datacell_arm.o:
	$(CC_ARM) -c ./datacell.cpp -I"$(BASE_DIR)" -o ./$@

dvector.o: datacell.o
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

dvector_arm.o: datacell_arm.o
	$(CC_ARM) -c ./datacell.cpp -I"$(BASE_DIR)" -o ./$@
