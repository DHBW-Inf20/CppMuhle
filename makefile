Main: build/Muhle.exe

build/Muhle.exe: source/Controller.cpp source/Main.cpp source/MuhleLogik.cpp source/KonsolenView.cpp
	g++ -o build/Muhle.exe source/MuhleLogik.cpp  source/Controller.cpp source/KonsolenView.cpp source/Main.cpp