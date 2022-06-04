
CXX=g++
CXXFLAGS=--std=c++17 -fpermissive -lstdc++fs


main: test main.cc mutations.c mutations.h utils.h utils.c
	$(CXX) $(CXXFLAGS) main.cc mutations.c utils.c -o main


test: test.c  
	$(CXX) $(CXXFLAGS) test.c -o test

.PHONY: clean
clean:
	-rm test main 
	-rm crashed/* crashs/*
