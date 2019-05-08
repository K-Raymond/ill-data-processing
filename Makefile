all: TXPConfig TProgView

TXPConfig:
	genreflex TXPConfig.h --rootmap TXPConfig.rootmap
	g++ -std=c++11 -shared -fPIC -o libTXPConfig.so TXPConfig.cpp TXPConfig_rflx.cpp `root-config --cflags --libs`

TProgView:
	genreflex TProgView.h --rootmap TProgView.rootmap
	g++ -std=c++11 -shared -fPIC -o libTProgView.so TProgView.cpp TProgView_rflx.cpp `root-config --cflags --libs`

clean:
	rm *_rflx.cpp *_rflx_rdict.pcm *_C.d *_C_ACLiC_dict_rdict.pcm *.rootmap *.so
