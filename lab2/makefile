output: main.o built_in.o external.o history.o
	g++ main.o built_in.o external.o history.o -o output

main.o: main.cpp
	g++ -g -std=c++11 -c main.cpp

built_in.o: built_in.cpp
	g++ -g -std=c++11 -c built_in.cpp

external.o: external.cpp
	g++ -g -std=c++11 -c external.cpp

history.o: history.cpp
	g++ -g -std=c++11 -c history.cpp

clean:
	rm *.o output
