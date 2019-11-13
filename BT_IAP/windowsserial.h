#ifndef SERIAL_H
#define SERIAL_H

#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QString>
#include <QObject>
#include <map>


class WindowsSerial : public QObject
{
    Q_OBJECT
public :
    explicit WindowsSerial(QObject *parent = nullptr);

signals:

public :
   QStringList GetPortList();


public slots:
   void receiveInfo();

private:
    QSerialPort m_serialPort;
    //QSerialPort Serial;


};

#endif // SERIAL_H
