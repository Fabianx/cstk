# CSTK makefile, by kristof:
# tiny and clean :)

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)

include $(BASE_DIR)/export_vars

all:	ksom.o ksom_arm.o

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
ksom.o: 
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

ksom_arm.o: 
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./ksom.cpp -I"$(BASE_DIR)" -o ./$@

