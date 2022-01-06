ifdef includePath
    IncludeFlag = -I "$(includePath)"
endif

ifdef libraryPath
	LibraryFlag = -L "$(libraryPath)"
endif

ifdef OS
	WINDOWSLIBS = -lwsock32 -lws2_32
endif

ifndef CPPFLAGS
	CPPFLAGS = -Wall -std=c++11 $(IncludeFlag) $(LibraryFlag)
else
	CPPFLAGS = $(CPPFLAGS) $(IncludeFlag) $(LibraryFlag)
endif

ifndef CXX
	CXX = g++
endif 

all: Server Client

Server: build/server.exe
Client: build/client.exe

build/server.exe:   source/network/packet_factory.cpp source/network/net_client.cpp source/server/main_server.cpp
	$(CXX) $(CPPFLAGS) -o build/server.exe source/network/packet_factory.cpp source/network/net_server.cpp source/server/main_server.cpp $(WINDOWSLIBS) -lboost_system -lboost_thread -lpthread

build/client.exe:  source/network/packet_factory.cpp source/network/net_client.cpp source/MainClient.cpp
	$(CXX) $(CPPFLAGS) -o build/client.exe source/network/packet_factory.cpp source/network/net_client.cpp source/MainClient.cpp $(WINDOWSLIBS) -lboost_system -lboost_thread -lpthread
