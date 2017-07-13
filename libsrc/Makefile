#Global vars
CPP=g++ -g
AR=ar crv
LD=g++
BIT_MODE=-m64
BIN_DIR=.

#Global setup
INCLUDE += -I .
INCLUDE += -I ../include
CPPFLAGS = $(BIT_MODE) $(INCLUDE)
LIBS += -L. 
LIBS += -lpthread 
LDFLAGS = $(BIT_MODE) $(LIBS)

TARGET_SO = ../lib/libjlogger.so
TARGET_A = ../lib/libjlogger.a

SRCS = $(shell ls *.cpp)
OBJS = $(SRCS:.cpp=.o)

all:$(TARGET_SO) $(TARGET_A)

clean:
	$(RM) -f $(OBJS); rm -f $(TARGET_SO) $(TARGET_A)
$(TARGET_A): $(OBJS)
	$(AR) $(TARGET_A) $(OBJS)
$(TARGET_SO):
	$(CPP) $(CPPFLAGS) $(SRCS) -fPIC -shared -o $(TARGET_SO) $(LDFLAGS)
$(TARGET_LIB): 
	$(TARGET_SO) $(TARGET_A)
$(OBJS): %.o:%.cpp
	$(CPP) -c $(CPPFLAGS) $< -o $@ 
