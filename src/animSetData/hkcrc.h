#ifndef HKCRC_H
#define HKCRC_H

#include <QByteArray>

//Not my code...
class HkCRC
{
public:
    HkCRC();
private:
    int reflectByte(int c);
    void reflect(int * crc, int bitnum, int startLSB);
    QByteArray compute(QByteArray input);
private:
    int crc_order;
    QByteArray crc_poly;
    QByteArray initial_value;
    QByteArray final_value;
};

#endif // HKCRC_H
