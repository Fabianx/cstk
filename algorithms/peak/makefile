# CSTK makefile, by kristof:
# tiny and clean :)

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)

include $(BASE_DIR)/export_vars

all:	peak.o peak_arm.o

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	
peak.o: 
	$(CC) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

peak_arm.o: 
	$(CC_ARM) -c ./peak.cpp -I"$(BASE_DIR)" -o ./$@
