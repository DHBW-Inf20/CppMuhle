Server:
	g++ -std=c++11 -o out/server source/network/packet_factory.cpp source/network/net_server.cpp -lboost_system -lboost_thread -lpthread

Client:
	g++ -std=c++11 -o out/client source/network/packet_factory.cpp source/network/net_client.cpp -lboost_system -lboost_thread -lpthread