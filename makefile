Server:
	g++ -std=c++11 -o out/server src/network/server.cpp -lboost_system -lboost_thread -lpthread

Client:
	g++ -std=c++11 -o out/client src/network/client.cpp -lboost_system -lboost_thread -lpthread