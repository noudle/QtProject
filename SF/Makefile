#aarch64-himix210-linux-
CROSS_COMPILE :=
CC := $(CROSS_COMPILE)gcc
CXX := $(CROSS_COMPILE)g++

#Q := @
Q := 
DEST:=app
SRC_DIRS := src common 

INCPATH := include
INCPATH  += $(foreach f,$(INCPATH),-I$(f))
INCPATH  += $(foreach f,$(SRC_DIRS),-I$(f))
LIBS := -lpthread 

EXTDIR  := .
INCPATH += -I$(EXTDIR)/include
LIBS    += -L$(EXTDIR)/lib


#CFLAGS 	 := -g $(INCPATH)
CFLAGS 	 := -std=gnu11 -g $(INCPATH)
CXXFLAGS := -std=gnu++11 -g $(INCPATH)
LDFLAGS  := $(LIBS)

SRCS := $(foreach f,$(SRC_DIRS),$(wildcard $(f)/*.cpp))
OBJS := $(SRCS:.cpp=.o)

all:$(DEST)


$(DEST):$(OBJS)
	$(Q)$(CXX) -o $@ $^ $(LDFLAGS) 
	
%.o:%.cpp 
	$(Q)$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	$(Q)rm -rf $(DEST) $(OBJS)

.PHONY:all clean cp
