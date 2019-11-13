#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <thread>         // std::thread
#include <semaphore.h>

sem_t open_serial;

//void ThrSerialRead(WindowsSerial *serial)
//{

//}

void ThrSerial(MainWindow *window)
{
    int open_serial_flag = 0;
//WAITCOMMAND:
    for( open_serial_flag = 0; !open_serial_flag; sem_getvalue(&open_serial, &open_serial_flag))
    {
        //wait a connecting signal
    }
    if(open_serial_flag)
    {
        sem_wait( &open_serial);
        while ((1)) {
           //my_serial.receiveInfo();
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    bool res;
    res = sem_init( &open_serial, 0, 0);
    if(res)
    {
        qDebug()<<"applicate a semaphore failed..."<<endl;
    }

//create a new thread for serial conmmnication.
    std::thread serial1(ThrSerial, &w);
    serial1.detach();

    w.show();


    return a.exec();
}


