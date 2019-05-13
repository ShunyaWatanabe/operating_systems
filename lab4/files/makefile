output: main.o store.o append.o extract.o meta.o display.o
	g++ main.o store.o append.o extract.o meta.o display.o -o output

main.o: main.cpp
	g++ -g -std=c++11 -c main.cpp

store.o: store.cpp
	g++ -g -std=c++11 -c store.cpp 

append.o: append.cpp
	g++ -g -std=c++11 -c append.cpp
 
extract.o: extract.cpp
	g++ -g -std=c++11 -c extract.cpp
 
meta.o: meta.cpp
	g++ -g -std=c++11 -c meta.cpp 

display.o: display.cpp
	g++ -g -std=c++11 -c display.cpp 
clean:
	rm *.o output
