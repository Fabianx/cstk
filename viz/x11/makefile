# CSTK makefile, by kristof:
# tiny and clean :)

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)

include $(BASE_DIR)/export_vars

all:	x11plot.o kvplot.o clustplot.o \
	x11plot_arm.o kvplot_arm.o clustplot_arm.o 

clean:
	@-rm -f ./*.o
	@-rm -f ./*~

kvplot.o: kvector.o peak.o x11plot.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

clustplot.o: kvector.o x11plot.o
	$(CC) $(CC_FLAGS) -c ./$*.cpp -I"$(BASE_DIR)" -o ./$@

kvplot_arm.o: kvector_arm.o peak_arm.o x11plot_arm.o
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./kvplot.cpp -I"$(BASE_DIR)" -o ./$@

clustplot_arm.o: kvector_arm.o x11plot_arm.o
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./clustplot.cpp -I"$(BASE_DIR)" -o ./$@


kvecto%.o:
	@cd $(BASE_DIR)/cstk_base/vector; make $@

pea%.o:
	@cd $(BASE_DIR)/algorithms/peak; make $@

%.o:	%.cpp
	$(CC) $(CC_FLAGS) -c ./$< -I"$(BASE_DIR)" -o ./$@

%_arm.o:	%.cpp
	$(CC_ARM) $(CC_FLAGS_ARM) -c ./$< -I"$(BASE_DIR)" -o ./$@
