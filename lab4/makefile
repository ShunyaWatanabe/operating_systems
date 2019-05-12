output: main.o func.o 
	g++ main.o func.o -o output

main.o: main.cpp
	g++ -g -std=c++11 -c main.cpp

func.o: func.cpp
	g++ -g -std=c++11 -c func.cpp 

clean:
	rm *.o output
