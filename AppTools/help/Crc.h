#ifndef CRC_H
#define CRC_H


class Crc
{
public:
    static unsigned int CRC_modbus(unsigned char *ptr,int len);
};

#endif // CRC_H
