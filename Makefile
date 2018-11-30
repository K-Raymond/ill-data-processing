
detCal:
	genreflex detCal.h --rootmap detCal.rootmap
	g++ -std=c++11 -shared -fPIC -o libdetCal.so detCal.cpp detCal_rflx.cpp `root-config --cflags --libs`

clean:
	rm libdetCal.so
	rm detCal_*
	*.rootmap
