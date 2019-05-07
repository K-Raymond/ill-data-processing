#include "./progressThread.h"

void progressThread::ThreadPrint( void )
{
    while( fGo )
    {
        sleep(2);
        updatePercent();
    }
}

void progressThread::start()
{
    updatePercent();
    fGo = true;
#pragma omp task firstprivate(fGo)
    ThreadPrint();
    return;
}

void progressThread::inc( long long amount )
{
    fcount += amount;
}

void progressThread::stop()
{
    fGo = false;
    updatePercent();
}

void progressThread::updatePercent()
{
    printf("\rProgress %f, %lld of %lld", 100*fcount/(double)fTotal,
            fcount, fTotal);
}

