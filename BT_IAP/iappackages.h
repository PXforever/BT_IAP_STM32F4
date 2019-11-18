#ifndef IAPPACKAGES_H
#define IAPPACKAGES_H

#include <QByteArray>
#include <QDebug>

#define PACKHEAD "\xff\xf0"
#define PACKTAIL "\xf0\xff"

class IapPackages
{
public:
    IapPackages();

public:
    QByteArray IapPack(QByteArray *origin,int part_num, int id, int size, QByteArray *crc);


};

#endif // IAPPACKAGES_H
