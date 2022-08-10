all : fileDir Switch System Network.out

fileDir:
	@mkdir -p build
	@mkdir -p bin
	@mkdir -p namedPipe
	@mkdir -p Test_Files

# Switch : 
Switch : build/Switch_Main.o build/Switch.o build/Tools.o
	g++ build/Switch_Main.o build/Switch.o build/Tools.o -o bin/Switch
	
build/Switch_Main.o : src/Switch_Main.cpp src/Switch.cpp src/Switch.h
	g++ -c src/Switch_Main.cpp -o build/Switch_Main.o

build/Switch.o :  src/Switch.cpp src/Switch.h
	g++ -c src/Switch.cpp -o build/Switch.o
	
# System : 
System : build/System_Main.o build/System.o build/Tools.o
	g++ build/System_Main.o build/System.o build/Tools.o -o bin/System
	
build/System_Main.o : src/System_Main.cpp src/System.cpp src/System.h
	g++ -c src/System_Main.cpp -o build/System_Main.o

build/System.o :  src/System.cpp src/System.h
	g++ -c src/System.cpp -o build/System.o

# LoadBalancer
Network.out :  build/Main.o build/LoadBalancer.o build/Tools.o

	g++ build/Main.o build/LoadBalancer.o build/Tools.o -o  Network.out

build/Main.o : src/Main.cpp src/LoadBalancer.h src/LoadBalancer.cpp 
	g++ -c src/Main.cpp -o build/Main.o

build/Tools.o : src/Tools.cpp src/Tools.h
	g++ -c src/Tools.cpp -o build/Tools.o
	
build/LoadBalancer.o : src/LoadBalancer.cpp src/LoadBalancer.h
	g++ -c src/LoadBalancer.cpp -o build/LoadBalancer.o

clean :
	rm -f -r build
	rm -f -r bin
	rm -f -r namedPipe
	rm -f -r Test_Files
	rm -f Network.out
