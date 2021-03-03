
CXX := clang++
CXXFLAGS += -std=c++17 -stdlib=libc++

sources := $(wildcard *.cpp)
files := $(foreach file,$(sources),$(dir $(file))/$(basename $(file)))
objects := $(foreach file,$(files),$(file).o)

lana : $(objects)
	$(CXX) $^ -o $@.bin

$(objects) :
	$(CXX) $(CXXFLAGS) -c $(sources)

clean:
	rm -f *.o
	rm -rf *.dSYM
