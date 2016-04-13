CXX = g++
CXXFLAGS = -std=c++11 -O0 -Wall

target = main
objects = main.o fifo.o

all: $(target)

$(target): $(objects)
	$(CXX) $(CXXFLAGS) $(objects) -o $@

$(objects): header.h

clean:
	rm $(target) $(objects)
