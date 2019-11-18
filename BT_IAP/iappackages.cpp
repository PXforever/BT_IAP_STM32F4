#include "iappackages.h"

IapPackages::IapPackages()
{

}

QByteArray IapPackages::IapPack(QByteArray *origin, int part_num, int id, int size, QByteArray *crc)
{
    QByteArray packed_date;
    packed_date.insert(0, PACKHEAD);
    packed_date[2] = part_num >> 24;
    packed_date[3] = part_num >> 16;
    packed_date[4] = part_num >> 8;
    packed_date[5] = part_num;

    packed_date[6] = id >> 24;
    packed_date[7] = id >> 16;
    packed_date[8] = id >> 8;
    packed_date[9] = id;

    packed_date[10] = size >> 24;
    packed_date[11] = size >> 16;
    packed_date[12] = size >> 8;
    packed_date[13] = size;

    packed_date.append( *origin);

    packed_date.append(PACKTAIL);

   // qDebug() << "packed is :" <<packed_date;

    return packed_date;
}
