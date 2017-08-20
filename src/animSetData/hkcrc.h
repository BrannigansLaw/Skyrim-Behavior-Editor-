#ifndef HKCRC_H
#define HKCRC_H

#include <QByteArray>

//Not my code...
class HkCRC
{
public:
    HkCRC();
    QByteArray compute(QByteArray input);
private:
    int reflectByte(int c);
    void reflect(int * crc, int bitnum, int startLSB);
private:
    int crc_order;
    QByteArray crc_poly;
    QByteArray initial_value;
    QByteArray final_value;
};

#endif // HKCRC_H