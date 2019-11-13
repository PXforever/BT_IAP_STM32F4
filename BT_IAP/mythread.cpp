#include "mythread.h"

MyThread::MyThread()
{
    isStop = false;
}

void MyThread::closeThread()
{
    isStop = true;
}

void MyThread::run()
{
    while (1)
    {
        if(isStop)
            return;
        qDebug()<<tr("mythread QThread::currentThreadId()==")<<QThread::currentThreadId();
        sleep(1);
    }
}

