#include "CrcWidget.h"
#include "ui_CrcWidget.h"
#include "../help/BinaryCvn.h"
#include "../help/Crc.h"

#include <QMessageBox>
#include <QDebug>

CrcWidget::CrcWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CrcWidget)
{
    ui->setupUi(this);
    QApplication::setFont(QFont ("微软雅黑", 10, QFont::Normal, false));
    QStringList comBoxItems;
    comBoxItems<<"CRC_modbus";
    ui->modelcbx->addItems(comBoxItems);
    ui->modelcbx->setCurrentIndex(0);
    QString crcLable;
    crcLable="Instruction\n\nName:CRC_modbus\n\nWidth:16\n\nPoly:0x8005\n\nInit:0xFFFF";
    ui->label->setText(crcLable);
}

CrcWidget::~CrcWidget()
{
    delete ui;
}

void CrcWidget::on_calculate_clicked()
{
    QString currentItems=ui->modelcbx->currentText();
    QString data=ui->data->toPlainText();
    if(data.isEmpty())
    {
        QMessageBox::warning(this, "警告", "请输入需要计算的CRC数据",
                             QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    QByteArray buffer=BinaryCvn::hexStrToByteArray(data);
    int size=buffer.size();
    unsigned char buffer_[100];
    unsigned int crc;
    unsigned int crc_[2];
    for(int i=0;i<size;i++)
        buffer_[i]=static_cast<unsigned char>(buffer[i]);
    if(currentItems=="CRC_modbus")
    {
        crc=Crc::CRC_modbus(buffer_,size);
        crc_[0]=crc>>8;
        crc_[1]=crc&0xff;
        qDebug()<<hex<<crc_[0]<<crc_[1];
        ui->display->append(QString("%1结果为：%2 %3").arg(currentItems).
                            arg(QString::number(crc_[0],16)).arg(QString::number(crc_[1],16)));
    }
}
