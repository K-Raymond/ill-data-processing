all: TXPConfig progressThread

TXPConfig:
	genreflex TXPConfig.h --rootmap TXPConfig.rootmap
	g++ -std=c++11 -shared -fPIC -o libTXPConfig.so TXPConfig.cpp TXPConfig_rflx.cpp `root-config --cflags --libs`

progressThread:
	genreflex progressThread.h --rootmap progressThread.rootmap
	g++ -std=c++11 -fopenmp -shared -fPIC -o libprogressThread.so progressThread.cpp progressThread_rflx.cpp `root-config --cflags --libs`

clean:
	rm libTXPConfig.so
	rm TXPConfig_*
	*.rootmap
