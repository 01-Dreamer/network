all: server client
server:server.cpp
	g++ server.cpp -o server -std=c++11 -pthread
client:client.cpp
	g++ client.cpp -o client -std=c++11

clean:
	rm -f server
	rm -f client

.PHONY:clean
