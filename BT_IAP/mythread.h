#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QDebug>
#include <QThread>
#include <QMutex>


class MyThread : public QThread
{

public:
    MyThread();
    void closeThread();

protected:
    virtual void run();

private:
    volatile bool isStop;       //isStop是易失性变量，需要用volatile进行申明
};

#endif // MYTHREAD_H
