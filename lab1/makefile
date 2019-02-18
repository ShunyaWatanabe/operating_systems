output: main.o built_in.o
	g++ main.o built_in.o -o output

main.o: main.cpp
	g++ -g -std=c++11 -c main.cpp

built_in.o: built_in.cpp
	g++ -g -std=c++11 -c built_in.cpp

clean:
	rm *.o output
