#ifndef THREADMUTEX_H
#define THREADMUTEX_H

#include <pthread.h>

class CThreadMutex{
    protected:
        pthread_mutex_t DMutex;
    public:
        CThreadMutex(){
            pthread_mutex_init(&DMutex, NULL);
        };
        
        ~CThreadMutex(){
            pthread_mutex_destroy(&DMutex);
        };

        void Lock(){
            pthread_mutex_lock(&DMutex);
        };

        bool TryLock(){
            return 0 == pthread_mutex_trylock(&DMutex);
        };
        
        void Unlock(){
            pthread_mutex_unlock(&DMutex);
        };
};

#endif
