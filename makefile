Server:
	g++ -std=c++11 -o out/server src/network/packet_factory.cpp src/network/server.cpp -lboost_system -lboost_thread -lpthread

Client:
	g++ -std=c++11 -o out/client src/network/packet_factory.cpp src/network/client.cpp -lboost_system -lboost_thread -lpthread