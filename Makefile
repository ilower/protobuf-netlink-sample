######################################
#
#######################################
  
#target you can change test to what you want
#目标文件名，输入任意你想要的执行文件名
TARGET  := server client
  
#compile and lib parameter
#编译参数
CC      := g++
LIBS    := -L./netlink -lnetlink -lprotobuf
LDFLAGS :=
DEFINES :=
INCLUDE := -I. -I./netlink
CFLAGS  := -g -Wall -O3 $(DEFINES) $(INCLUDE)
CXXFLAGS:= $(CFLAGS) -DHAVE_CONFIG_H

#source file
#源文件，自动找所有.c和.cpp文件，并将目标定义为同名.o文件
SOURCE  := $(wildcard *.c) $(wildcard *.cpp) $(wildcard *.cc)
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(SOURCE))))
  
#i think you should do anything here
#下面的基本上不需要做任何改动了
.PHONY : objs clean rebuild libnetlink
	  
all : libnetlink $(TARGET) 
	  
objs : $(OBJS)
	  
rebuild: clean all
	                
clean :
	    rm -fr *.so
		rm -fr *.o
		rm -fr $(TARGET)
		make -C netlink clean

libnetlink :
		make -C netlink

server: Server.o $(filter-out Client.o,$(OBJS))
		$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

client: Client.o $(filter-out Server.o,$(OBJS))
		$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)
