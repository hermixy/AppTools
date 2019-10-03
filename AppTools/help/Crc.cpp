#include "Crc.h"

unsigned int Crc::CRC_modbus(unsigned char *ptr,int len)
{
    unsigned char i;
    unsigned int crc = 0xFFFF;
    if (len == 0)
        len = 1;
    while (len--)
    {
        crc ^= *ptr;
        for (i = 0; i < 8; i++)
        {
            if (crc & 1) {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else {
                crc >>= 1;
            }
        }
        ptr++;
    }
    return(crc);
}
