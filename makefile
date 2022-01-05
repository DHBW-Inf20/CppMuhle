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
	CPPFLAGS = -std=c++11 -Wall -Wextra -Wzero-as-null-pointer-constant -Wformat=2 -Wunused $(IFLAG) $(LFLAG)
else
	CPPFLAGS = $(CPPFLAGS)  $(IFLAG) $(LFLAG)
endif

ifdef OS
  WINDOWSLIBS =  -lwsock32 -lws2_32
endif

ifndef CXX
	CXX = g++
endif



build/Muhle.exe: build source/controller.cpp source/main.cpp source/muhle_logik.cpp source/konsolen_view.cpp
	$(CXX) $(CPPFLAGS) -o build/Muhle.exe source/muhle_logik.cpp  source/controller.cpp source/konsolen_view.cpp source/main.cpp

build:
	mkdir build

test: tests/test.cpp
	$(CXX) $(CPPFLAGS) -o tests/test.exe source/muhle_logik.cpp source/konsolen_view.cpp tests/test.cpp  -lboost_unit_test_framework