# CSTK makefile, by kristof:
# tiny and clean :)

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)

include $(BASE_DIR)/export_vars

CXXFLAGS+=-I"$(BASE_DIR)"
CXXFLAGS+=-g
#CXXFLAGS+=-I"$(BASE_DIR)" -I"C:\\Program Files\\MATLAB\\R2008a\\extern\\include"
#CXXFLAGS+=-mno-cygwin

all:	GeneticAlgorithm.o GAHello.o GeneticAlgorithm_t.o
	g++ -o ga.exe GeneticAlgorithm.o GAHello.o GeneticAlgorithm_t.o ../../cstk_base/vector/*.o
#	g++ -o cmeans.exe cmeans.o cmeans_t.o cmeans_cstk.o cmeans_matlab.o ../../cstk_base/vector/*.o "C:\\Program Files\\MATLAB\\R2008a\\extern\\lib\\win32\\microsoft\\libeng.lib" "C:\\Program Files\\MATLAB\\R2008a\\extern\\lib\\win32\\microsoft\\libmx.lib"

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
