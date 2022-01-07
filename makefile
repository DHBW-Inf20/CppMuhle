ifdef includePath
    IncludeFlag = -I "$(includePath)"
endif

ifdef libraryPath
	LibraryFlag = -L "$(libraryPath)"
endif

ifdef OS
	WINDOWSLIBS = -lwsock32 -lws2_32
endif

ifndef CXXFLAGS
	CXXFLAGS =-std=c++11 $(IncludeFlag) $(LibraryFlag)
else
	CXXFLAGS = $(CXXFLAGS) $(IncludeFlag) $(LibraryFlag)
endif

ifndef CXX
	CXX = g++
endif 

ServerSOURCES := source/network/packet_factory.cpp source/network/net_server.cpp source/server/main_server.cpp source/server/network_controller.cpp source/logic/muhle_logik.cpp source/server/game_controller.cpp

ClientSOURCES := source/client/konsolen_view.cpp source/network/packet_factory.cpp source/network/net_client.cpp source/client/client_controller.cpp source/client/main_client.cpp

ServerOBJECTS := $(patsubst %.cpp,%.o,$(ServerSOURCES))
ClientOBJECTS := $(patsubst %.cpp,%.o,$(ClientSOURCES))

# WARNING := -Wall -Wextra

all: Server Client

Server: build/server.exe
Client: build/client.exe


build/server.exe: $(ServerOBJECTS)
	$(CXX) $(CXXFLAGS) $(WARNING) -o build/server.exe $(ServerOBJECTS) $(WINDOWSLIBS) -lboost_system -lboost_thread -lpthread

build/client.exe: $(ClientOBJECTS)
	$(CXX) $(CXXFLAGS) $(WARNING) -o build/client.exe $(ClientOBJECTS) $(WINDOWSLIBS) -lboost_system -lboost_thread -lpthread



ServerDEPENDS := $(patsubst %.cpp,%.d,$(ServerSOURCES))
ClientDEPENDS := $(patsubst %.cpp,%.d,$(ClientSOURCES))
-include $(ServerDEPENDS)
-include $(ClientDEPENDS)


%.o: %.cpp makefile
	$(CXX) $(WARNING) $(CXXFLAGS) -MMD -MP -c $< -o $@