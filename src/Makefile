#Global vars
CPP=g++ -g
LD=g++
BIT_MODE=-m64
BIN_DIR=../bin

#Global setup
INCLUDE += -I .
INCLUDE += -I ../include
CPPFLAGS = $(BIT_MODE) $(INCLUDE)
LIBS += -L../lib 
LIBS += -ljlogger
LIBS += -lpthread
LIBS += -Wl,-rpath,'$$ORIGIN/../lib'
LDFLAGS = $(BIT_MODE) $(LIBS)

TARGET= $(BIN_DIR)/jlogger

SRCS = $(shell ls *.cpp)
OBJS = $(SRCS:.cpp=.o)

all:$(TARGET)

clean:
	$(RM) -f $(OBJS); rm -f $(TARGET)

$(TARGET): $(OBJS)
	${LD} $(OBJS) -o $@ $(LDFLAGS)
.cpp.o:
	${CPP} $(CPPFLAGS) -c $< -o $@


