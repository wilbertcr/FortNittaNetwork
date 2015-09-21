#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <chrono>
#include <sstream>
#include <iostream>
#include <fstream>

class CThread{
    private:
        pthread_t DThread;
        size_t DStackSize;
        volatile void *DReturnValue;
        volatile bool DTerminated;
        volatile bool DDetached;

        static void *EntryPoint(void *arg);

    protected:
        void Exit(void *retval);

        virtual void *Execute() = 0;

    public:
        CThread(size_t stacksize, bool startsuspended);
        ~CThread();

        bool Resume();
        void Detach();
        void *Join();
        bool Terminated();
        std::ofstream DThreadLog;
};

#endif

