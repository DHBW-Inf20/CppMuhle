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

ServerSOURCES := source/network/packet_factory.cpp source/network/net_client.cpp source/server/main_server.cpp source/server/network_controller.cpp source/logic/muhle_logik.cpp source/server/game_controller.cpp

ClientSOURCES := source/client/konsolen_view.cpp source/network/packet_factory.cpp source/network/net_client.cpp source/client/client_controller.cpp source/client/main_client.cpp

ServerOBJECTS := $(patsubst %.cpp,%.o,$(ServerSOURCES))
ClientOBJECTS := $(patsubst %.cpp,%.o,$(ClientSOURCES))

WARNING := -Wall -Wextra

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

# obj/%.o: %.cpp Makefile
#         $(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# build/server.exe: obj/muhle_logik.o obj/game_controller.o obj/network_controller.o obj/packet_factory.o obj/net_server.o source/server/main_server.cpp
# 	$(CXX) $(CPPFLAGS) -o build/server.exe obj/muhle_logik.o obj/game_controller.o obj/network_controller.o obj/packet_factory.o obj/net_server.o source/server/main_server.cpp $(WINDOWSLIBS) -lboost_system -lboost_thread -lpthread

# build/client.exe:  obj/konsolen_view.o obj/packet_factory.o obj/net_client.o obj/client_controller.o source/client/main_client.cpp
# 	$(CXX) $(CPPFLAGS) -o build/client.exe  obj/konsolen_view.o obj/packet_factory.o obj/net_client.o obj/client_controller.o source/client/main_client.cpp  $(WINDOWSLIBS) -lboost_system -lboost_thread -lpthread


# All Object files
# obj/muhle_logik.o : source/logic/muhle_logik.cpp
# 	$(CXX) $(CPPFLAGS) -c -o obj/muhle_logik.o source/logic/muhle_logik.cpp

# obj/game_controller.o : source/server/game_controller.cpp
# 	$(CXX) $(CPPFLAGS) -c -o obj/game_controller.o source/server/game_controller.cpp

# obj/konsolen_view.o : source/client/konsolen_view.cpp
# 	$(CXX) $(CPPFLAGS) -c -o obj/konsolen_view.o source/client/konsolen_view.cpp

# obj/packet_factory.o: source/network/packet_factory.cpp
# 	$(CXX) $(CPPFLAGS) -c -o obj/packet_factory.o source/network/packet_factory.cpp

# obj/net_client.o:source/network/net_client.cpp
# 	$(CXX) $(CPPFLAGS) -c -o obj/net_client.o source/network/net_client.cpp

# obj/net_server.o:source/network/net_server.cpp
# 	$(CXX) $(CPPFLAGS) -c -o obj/net_server.o source/network/net_server.cpp

# obj/client_controller.o:source/client/client_controller.cpp
# 	$(CXX) $(CPPFLAGS) -c -o obj/client_controller.o source/client/client_controller.cpp

# obj/network_controller.o:source/server/network_controller.cpp
# 	$(CXX) $(CPPFLAGS) -c -o obj/network_controller.o source/server/network_controller.cpp

