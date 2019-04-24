all: coordinator cashier client script server remove


coordinator: coordinator.o
	g++ -lpthread coordinator.o -o coordinator

coordinator.o: coordinator.cpp
	g++ -std=c++11 -g -c coordinator.cpp


cashier: cashier.o
	g++ -lpthread cashier.o -o cashier

cashier.o: cashier.cpp
	g++ -std=c++11 -g -c cashier.cpp


client: client.o
	g++ -lpthread client.o -o client

client.o: client.cpp
	g++ -std=c++11 -g -c client.cpp

server: server.o
	g++ -lpthread server.o -o server

server.o: server.cpp
	g++ -std=c++11 -g -c server.cpp

script: script.cpp
	g++ -std=c++11 -o script script.cpp

remove.o: remove.cpp
	g++ -std=c++11 -g -c remove.cpp

remove: remove.cpp
	g++ -std=c++11 -o remove remove.cpp

clean:
	rm *.o coordinator cashier client script server
