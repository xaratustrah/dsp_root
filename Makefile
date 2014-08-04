CXX = $(shell root-config --cxx)
LD = $(shell root-config --ld)

OS_NAME:=$(shell uname -s | tr A-Z a-z)
ifeq ($(OS_NAME),darwin)
STDINCDIR := -I/opt/local/include
STDLIBDIR := -L/opt/local/lib
else
STDINCDIR := 
STDLIBDIR := 
endif

CPPFLAGS := $(shell root-config --cflags) $(STDINCDIR)
LDFLAGS := $(shell root-config --glibs) $(STDLIBDIR) -lgsl -lgslcblas -lfftw3 -lm

CPPFLAGS += -g

TARGET = spectrogramme signalgen spectrumgen

SRC = 

OBJ = $(SRC:.cpp=.o)

all : $(TARGET)

signalgen :
	$(CXX) $(CPPFLAGS) -o signalgen signalgen.cpp $(LDFLAGS)

spectrogramme :
	$(CXX) $(CPPFLAGS) -o spectrogramme spectrogramme.cpp $(LDFLAGS)

spectrumgen :
	$(CXX) $(CPPFLAGS) -o spectrumgen spectrumgen.cpp $(LDFLAGS)


clean :
	rm -f *.o $(TARGET) *~
