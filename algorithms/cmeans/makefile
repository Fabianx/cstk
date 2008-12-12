# CSTK makefile, by kristof:
# tiny and clean :)

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)

include $(BASE_DIR)/export_vars

all:	cmeans.o cmeans_arm.o

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
cmeans.o: 
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

cmeans_arm.o: 
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./cmeans.cpp -I"$(BASE_DIR)" -o ./$@

