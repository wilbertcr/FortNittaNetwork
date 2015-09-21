#include "Thread.h"

#include <sched.h>
#include <errno.h>


CThread::CThread(size_t stacksize, bool startsuspended): DThreadLog(){
    DTerminated = true;
    DDetached = false;
    DStackSize = stacksize;
    DThreadLog.open("ThreadLog.txt", std::ofstream::app);
    if(!startsuspended){
        Resume();
    }
}

CThread::~CThread(){
    if(!DTerminated && !DDetached){
        Join();
    }
}

void *CThread::EntryPoint(void *arg){
    CThread *CallingThread = (CThread *)arg;
    CallingThread->DReturnValue = CallingThread->Execute();
    CallingThread->DTerminated = true;
    return (void *)CallingThread->DReturnValue;
}

void CThread::Exit(void *retval){
    DReturnValue = retval;
    pthread_exit(retval);
}

bool CThread::Resume(){
    
    if(DTerminated){
        pthread_attr_t ThreadAttributes;
        int ReturnValue;
        DThreadLog << "Creating network thread.\n";
        if(pthread_attr_init(&ThreadAttributes)){
            DThreadLog << "Initializing attributes failed.\n";
            DThreadLog.flush();
            return false;
        }
        ReturnValue = pthread_attr_setstacksize(&ThreadAttributes, DStackSize);
        if(ReturnValue && (ENOSYS != ReturnValue)){
            DThreadLog << "Setting stacksize failed.\n";
            DThreadLog.flush();
            return false;
        }
        DThreadLog << "Thread Stack set.\n";
        DThreadLog.flush();
        DDetached = false;
        DTerminated = false;
        ReturnValue = pthread_create(&DThread, &ThreadAttributes, EntryPoint, (void *)this);
        if(ReturnValue){
            DThreadLog << "Creating Thread failed.\n";
            DThreadLog.flush();
            pthread_attr_destroy(&ThreadAttributes);
            DTerminated = true;
            return false;
        }
        DThreadLog << "Created Thread Successfully.\n";
        DThreadLog.flush();
        pthread_attr_destroy(&ThreadAttributes);
        return true;
    }
    DThreadLog << "Thread  is terminated.failed.\n";
    DThreadLog.flush();
    return false;
}

void CThread::Detach(){
    DDetached = true;
    pthread_detach(DThread);
}

void *CThread::Join(){
    void *ReturnValue;

    pthread_join(DThread, &ReturnValue);
    return ReturnValue;
}

bool CThread::Terminated(){
    return DTerminated;
}