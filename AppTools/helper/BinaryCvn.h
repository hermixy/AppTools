#ifndef BINARYCVN_H
#define BINARYCVN_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

class BinaryCvn: public QObject
{
public:
    //单精度浮点数 转 16进制
    static unsigned int floatToHex(float a);

    //16进制 转 单精度浮点数
    static float hexToFloat(unsigned char* a);

    //16进制QByteArray 转 16进制QString
    static QString byteArrayToHexStr(QByteArray data);

    //16进制QByteArray 转 Ascii码
    static QString byteArrayToAsciiStr(QByteArray data);

    //16进制QString 转 16进制QByteArray
    static QByteArray hexStrToByteArray(QString str);

    //16进制QString 转 Ascii码
    static QByteArray asciiStrToByteArray(QString str);

    //字符 转 char 16进制
    static char convertHexChar(char ch);

    //十进制转换16进制
    static QString decimalToStrHex(int decimal);

    //16进制QString 转 十进制
    static int strHexToDecimal(QString strHex);
};

#endif // BINARYCVN_H
