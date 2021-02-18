
CXX := clang++
CXXFLAGS := -std=c++17 -stdlib=libc++ -g

lana.bin : $(wildcard *.cpp)
	$(CXX) $(CXXFLAGS) $(wildcard *.cpp) -o $@
