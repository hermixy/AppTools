#include "FloatWidget.h"
#include "ui_FloatWidget.h"
#include "helper/BinaryCvn.h"

FloatWidget::FloatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FloatWidget)
{
    ui->setupUi(this);

    QApplication::setFont(QFont ("微软雅黑", 10, QFont::Normal, false));
}

FloatWidget::~FloatWidget()
{
    delete ui;
}

void FloatWidget::on_convHex_clicked()
{
    if(ui->FloatLine->text().isEmpty())
    {
        return;
    }
    float f=ui->FloatLine->text().toFloat();
    unsigned int Hex=BinaryCvn::floatToHex(f);

    //qDebug()<<Hex;
    ui->HexLine->setText(QString("%1").arg(Hex,8,16,QChar('0')));
}

void FloatWidget::on_convFlaot_clicked()
{
    if(ui->HexLine->text().isEmpty())
    {
        return;
    }
    QString h=ui->HexLine->text();
    QByteArray buffer;
    buffer=BinaryCvn::hexStrToByteArray(h);
    unsigned char buf[4];
    for(int i=0;i<4;i++)
    {
        buf[3-i]=static_cast<unsigned char>(buffer[i]);
        buf[3-i]&=0xff;
        //qDebug()<<hex<<buf[3-i];
    }
    float f=BinaryCvn::hexToFloat(buf);
    ui->FloatLine->setText(QString("%1").arg(static_cast<double>(f)));
}
















