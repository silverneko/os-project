CXX = g++
CXXFLAGS = -std=c++11 -O0 -Wall

ifdef DEMO
CXXFLAGS += -D DEMO
endif

target = main
objects = main.o fifo.o rr.o sjf.o psjf.o

.PHONY: all clean

all: $(target)

$(target): $(objects)
	$(CXX) $(CXXFLAGS) $(objects) -o $@

$(objects): header.h

clean:
	rm -rf $(target) $(objects)
