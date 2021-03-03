
extension := .bin

$(info plataforma: $(OS))

ifeq ($(OS),Windows_NT)
	CXX := g++
	CXXFLAGS += -std=c++17
	extension := .exe
else
	CXX := clang++
	CXXFLAGS += -std=c++17 -stdlib=libc++
endif

sources := $(wildcard *.cpp)
files := $(foreach file,$(sources),$(dir $(file))/$(basename $(file)))
objects := $(foreach file,$(files),$(file).o)

lana : $(objects)
	$(CXX) $^ -o $@$(extension)

$(objects) :
	$(CXX) $(CXXFLAGS) -c $(sources)

ifeq ($(OS),Windows_NT)
clean:
	del *.o /q /s
else
clean:
	rm -f *.o
	rm -rf *.dSYM
endif