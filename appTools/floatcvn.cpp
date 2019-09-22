#include "floatcvn.h"
#include "ui_floatcvn.h"
#include "binaryCvn.h"

FloatCvn::FloatCvn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FloatCvn)
{
    ui->setupUi(this);

    QApplication::setFont(QFont ("微软雅黑", 10, QFont::Normal, false));

    connect(ui->convHex,&QPushButton::clicked,[this]
    {
        if(ui->FloatLine->text().isEmpty())
        {
            return;
        }
        float f=ui->FloatLine->text().toFloat();
        unsigned int Hex=binaryCvn::fl(f);

        //qDebug()<<Hex;
        ui->HexLine->setText(QString("%1").arg(Hex,8,16,QChar('0')));

    });

    connect(ui->convFlaot,&QPushButton::clicked,[this]
    {
        if(ui->HexLine->text().isEmpty())
        {
            return;
        }
        unsigned int h=ui->HexLine->text().toUInt();
        qDebug()<<h;
        float f=binaryCvn::ByteToFloat(&h);
        qDebug()<<f;
        ui->FloatLine->setText(QString("%1").arg(static_cast<double>(f)));
    });
}

FloatCvn::~FloatCvn()
{
    delete ui;
}
