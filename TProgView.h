#include <stdio.h>
#include <stdlib.h>
#include <atomic>
#include <unistd.h>

class progressThread {
    public:
    progressThread() {}
    progressThread( long long Total )
    {
        fTotal = Total;
        fcount = 0;
        fGo = false;
    }
    
    // Start and stop thread/progress bar
    void start();
    void stop();

    void inc( long long num ); // Incriment fcount

    private:
    pthread_t fThreadId;
    long long fTotal;
    volatile long long fcount;
    
    void ThreadPrint();
    void updatePercent();
    bool fGo;
};
