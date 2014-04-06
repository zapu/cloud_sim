SOURCES = main.cpp system.cpp tools.cpp
LIBS = -lboost_iostreams -lboost_system -lboost_filesystem
FLAGS = -static-libstdc++ -std=c++11 -Ofast

all: a.out

a.out: $(SOURCES)
	 g++49 $(FLAGS) $(SOURCES) $(LIBS) 
