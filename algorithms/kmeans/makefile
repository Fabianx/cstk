# CSTK makefile, by kristof:
# tiny and clean :)

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)

include $(BASE_DIR)/export_vars

all:	kmeans.o kmeans_arm.o

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
kmeans.o: 
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

kmeans_arm.o: 
	$(CC_ARM) $(CC_ARM_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

