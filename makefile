ifdef includePath
	IFLAG = -I "$(includePath)"
else
$(info Info: If the Build is failing, try adding an includePath to the Boost-Include-Header-Files )
endif

ifdef libPath
	LFLAG = -L "$(libPath)"
else
$(info If the Build is failing, try adding an libPath to the Boost-Libraries )

endif

ifndef CPPFLAGS
	CPPFLAGS = -std=c++11 $(IFLAG) $(LFLAG)
else
	CPPFLAGS = $(CPPFLAGS)  $(IFLAG) $(LFLAG)
endif

ifdef OS
  WINDOWSLIBS =  -lwsock32 -lws2_32
endif

ifndef CXX
	CXX = g++
endif



build/Muhle.exe: source/Controller.cpp source/Main.cpp source/MuhleLogik.cpp source/KonsolenView.cpp
	$(CXX) $(CPPFLAGS) -o build/Muhle.exe source/MuhleLogik.cpp  source/Controller.cpp source/KonsolenView.cpp source/Main.cpp


test: tests/test.cpp
	$(CXX) $(CPPFLAGS) -o tests/test.exe source/MuhleLogik.cpp source/KonsolenView.cpp tests/test.cpp  -lboost_unit_test_framework