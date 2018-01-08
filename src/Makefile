CC=gcc
CPP=g++ -g
dbg_option=
my_source=./
my_include=./
d5000_include=$(D5000_HOME)/src/include

COMMONDEF=-D_POSIX_REALTIME_SIGNALS -D_REENTRANT -DLINUX_AIO_NOTIFY_USING_THREAD -DLINUX_USING_QUEUED_IO
CFLAGS=-fpic $(dbg_option) -I $(my_include) -I $(d5000_include)
VPATH=$(my_source)

PlatformLibs=-lc -lm -lrt -lpthread -ldl -lsend_top3
D5000_LIB_PATH=$(D5000_HOME)/lib
MY_LIBS=

%.o:%.cpp
	$(CPP) -c  $(CFLAGS) $< -o $@
%.o:%.cc
	$(CPP) -c  $(CFLAGS) $< -o $@

source_objects=main.o\
			   alarmProc.o\
			   alarmVector.o\
			   myProc.o\
			   util.o\
			   getCpuCoreNum.o\
			   getMemTotal.o

object_files=$(source_objects) 

#final_object=control

#all : $(final_object)

.PHONY : all clean rebuild 
	
exe2_top3 : $(object_files)
	$(CPP) -o $@ $(object_files) -L$(D5000_LIB_PATH) $(MY_LIBS) $(PlatformLibs)
	@rm -rf $(object_files)	
	@echo make ok.

clean :	
	@rm -rf $(object_files)	
#	@rm -rf $(final_object)

rebuild	: clean	all
