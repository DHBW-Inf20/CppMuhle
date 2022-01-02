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

Server:
	$(CXX) $(CPPFLAGS) -o build/server source/network/packet_factory.cpp source/network/net_server.cpp $(WINDOWSLIBS) -lboost_system -lboost_thread -lpthread 

Client:
	$(CXX) $(CPPFLAGS) -o build/client source/network/packet_factory.cpp source/network/net_client.cpp $(WINDOWSLIBS) -lboost_system -lboost_thread -lpthread



Main: build/Muhle.exe
build/Muhle.exe: source/Controller.cpp source/Main.cpp source/MuhleLogik.cpp source/KonsolenView.cpp
	$(CXX) $(CPPFLAGS) -o build/Muhle.exe source/MuhleLogik.cpp  source/Controller.cpp source/KonsolenView.cpp source/Main.cpp

