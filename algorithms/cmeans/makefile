# CSTK makefile, by kristof:
# tiny and clean :)

BASE_DIR= $(if(BASE_DIR==NULL)$(shell cd ../..;pwd)

include $(BASE_DIR)/export_vars

CXXFLAGS+=-I"$(BASE_DIR)"

all:	cmeans.o cmeans_cstk.o
	g++ -o cmeans.exe cmeans.o cmeans_cstk.o ../../cstk_base/vector/*.o

clean:
	@-rm -f ./*.o
	@-rm -f ./*~
