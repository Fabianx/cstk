# CSTK makefile, by kristof:
# tiny and clean :)

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../../..;pwd)

include $(BASE_DIR)/export_vars

VECTOR_DIR=$(BASE_DIR)/cstk_base/vector
SD_DIR=$(BASE_DIR)/sensordata

all:	binvector_demo bvector_demo dvector_demo kvector_demo datacell_demo \
	binvector_demo_arm bvector_demo_arm dvector_demo_arm kvector_demo_arm \
	datacell_demo_arm 

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
	@-rm -f ./*_demo
	@-rm -f ./*_demo_arm
	
##### Datacell Demo ###############################################################

datacell_demo: datacell_t.o
	$(CC) $(CC_FLAGS) ./datacell_t.o $(VECTOR_DIR)/datacell.o -o ./$@
	
datacell_demo_arm: datacell_t_arm.o
	$(CC_ARM) $(CC_FLAGS_ARM) ./datacell_t_arm.o $(VECTOR_DIR)/datacell_arm.o -o ./$@
	
datacell_t.o: datacell.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@
	
datacell_t_arm.o: datacell_arm.o
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./datacell_t.cpp -I"$(BASE_DIR)" -o ./$@
	
##### BinVector Demo ##############################################################

binvector_demo: binvector_t.o
	$(CC) $(CC_FLAGS) ./binvector_t.o $(VECTOR_DIR)/binvector.o -o ./$@
	
binvector_demo_arm: binvector_t_arm.o
	$(CC_ARM) $(CC_FLAGS_ARM) ./binvector_t_arm.o $(VECTOR_DIR)/binvector_arm.o -o ./$@
	
binvector_t.o: binvector.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

binvector_t_arm.o: binvector_arm.o
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./binvector_t.cpp -I"$(BASE_DIR)" -o ./$@
	
##### BVector Demo ################################################################

bvector_demo: 
	$(CC) $(CC_FLAGS) ./bvector_t.cpp -I"$(BASE_DIR)" -o ./$@
	
bvector_demo_arm: 
	$(CC_ARM) $(CC_FLAGS_ARM) ./bvector_t.cpp -I"$(BASE_DIR)" -o ./$@
	
##### DVector Demo ################################################################

dvector_demo: dvector_t.o
	$(CC) $(CC_FLAGS) ./dvector_t.o $(VECTOR_DIR)/dvector.o $(VECTOR_DIR)/datacell.o \
	      -o ./$@
	
dvector_demo_arm: dvector_t_arm.o
	$(CC_ARM) $(CC_FLAGS_ARM) ./dvector_t_arm.o $(VECTOR_DIR)/dvector_arm.o \
		$(VECTOR_DIR)/datacell_arm.o -o ./$@
	
dvector_t.o: dvector.o datacell.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

dvector_t_arm.o: dvector_arm.o datacell_arm.o
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./dvector_t.cpp -I"$(BASE_DIR)" -o ./$@

##### KVector Demo ################################################################

kvector_demo: kvector_t.o
	$(CC) $(CC_FLAGS) ./kvector_t.o $(VECTOR_DIR)/kvector.o -o ./$@
	
kvector_demo_arm: kvector_t_arm.o
	$(CC_ARM) $(CC_FLAGS_ARM) ./kvector_t_arm.o $(VECTOR_DIR)/kvector_arm.o -o ./$@
	
kvector_t.o: kvector.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@
	
kvector_t_arm.o: kvector.o
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./kvector_t.cpp -I"$(BASE_DIR)" -o ./$@
	
##### Dependencies: ##############################################################

%vector.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

%vector_arm.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

datacell.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)
	
datacell_arm.o:
	@cd $(VECTOR_DIR); make $@ BASE_DIR=$(BASE_DIR)

###################################################################################
