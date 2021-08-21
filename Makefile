
CXX := clang++
CXXFLAGS := -std=c++17 -stdlib=libc++

all : teste.cpp testef.hpp
	$(CXX) $(CXXFLAGS) -c testef.cpp -o testef.o
	$(CXX) $(CXXFLAGS) testef.o teste.cpp -o teste.bin

clean: testef.o teste.bin
	rm -f testef.o
	rm -f teste.bin

lana : lana.cpp
	$(CXX) $(CXXFLAGS) lana.cpp -o lana.bin

lana-clean :
	rm -rf lana.bin
