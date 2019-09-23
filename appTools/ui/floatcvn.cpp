#include "floatcvn.h"
#include "ui_floatcvn.h"
#include "helper/binaryCvn.h"

FloatCvn::FloatCvn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FloatCvn)
{
    ui->setupUi(this);

    QApplication::setFont(QFont ("微软雅黑", 10, QFont::Normal, false));
}

FloatCvn::~FloatCvn()
{
    delete ui;
}

void FloatCvn::on_convHex_clicked()
{
    if(ui->FloatLine->text().isEmpty())
    {
        return;
    }
    float f=ui->FloatLine->text().toFloat();
    unsigned int Hex=binaryCvn::fl(f);

    //qDebug()<<Hex;
    ui->HexLine->setText(QString("%1").arg(Hex,8,16,QChar('0')));
}

void FloatCvn::on_convFlaot_clicked()
{
    if(ui->HexLine->text().isEmpty())
    {
        return;
    }
    QString h=ui->HexLine->text();
    QByteArray buffer;
    buffer=binaryCvn::hexStrToByteArray(h);
    unsigned char buf[4];
    for(int i=0;i<4;i++)
    {
        buf[3-i]=static_cast<unsigned char>(buffer[i]);
        buf[3-i]&=0xff;
        //qDebug()<<hex<<buf[3-i];
    }
    float f=binaryCvn::ByteToFloat(buf);
    ui->FloatLine->setText(QString("%1").arg(static_cast<double>(f)));
}
















