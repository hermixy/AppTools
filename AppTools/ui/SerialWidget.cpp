#include "SerialWidget.h"
#include "ui_SerialWidget.h"
#include "helper/myHelper.h"
#include "helper/BinaryCvn.h"
#include "helper/AppCfg.h"
#include <QSerialPortInfo>

#pragma execution_character_set("utf-8")

SerialWidget::SerialWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialWidget)
{
    ui->setupUi(this);

    myHelper::InCenter(this);
    initdata();
    initform();
    builtconnect();
    changeEnable(false);
}

SerialWidget::~SerialWidget()
{
    saveconfig();
    delete ui;
}

void SerialWidget::initdata()
{
    ok=false;
    recvcount = 0;
    sendcount = 0;
    waittime = 20;
}

void SerialWidget::initform()
{
    //查找可用的串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort port;
        port.setPort(info);
        if(port.open(QIODevice::ReadWrite))
        {
            //将串口号添加到portname
            ui->portname->addItem(port.portName());
            //关闭串口等待人为(打开串口按钮)打开
            port.close();
        }
    }

    QList<qint32> blist;
    QSerialPortInfo info;
    blist=info.standardBaudRates();
    QStringList baudlist;
    for(int i=0;i<blist.length();i++)
        baudlist.append(QString("%1").arg(blist.at(i)));
    ui->baudrate->addItems(baudlist);
    ui->baudrate->setCurrentIndex(6);

    QStringList stopBitsList;
    stopBitsList << "1";
#ifdef Q_OS_WIN
    stopBitsList << "1.5";
#endif
    stopBitsList << "2";
    ui->stopbit->addItems(stopBitsList);
    ui->stopbit->setCurrentIndex(0);

    QStringList dataBitsList;
    dataBitsList << "5" << "6" << "7" << "8";
    ui->databit->addItems(dataBitsList);
    ui->databit->setCurrentIndex(3);

    QStringList parityList;
    parityList << "无" << "奇" << "偶";
    ui->checkbit->addItems(parityList);
    ui->checkbit->setCurrentIndex(0);

    ui->hexsend->setChecked(true);
    ui->hexrecv->setChecked(true);
    ui->autosend->setChecked(false);

    QStringList sendInterval;//自动发送间隔
    for (int i = 100; i < 1000; i = i + 100) {
        sendInterval << QString::number(i);
    }
    for (int i = 1000; i <= 10000; i = i + 1000) {
        sendInterval << QString::number(i);
    }
    ui->autosendtime->addItems(sendInterval);
    ui->autosendtime->setCurrentIndex(9);

    QApplication::setFont(QFont ("微软雅黑", 10, QFont::Normal, false));

    AppCfg::readconfig();                          //读取配置
    ui->baudrate->setCurrentIndex(ui->baudrate->findText(QString::number(AppCfg::BaudRate)));
    ui->stopbit->setCurrentIndex(ui->stopbit->findText(QString::number(AppCfg::StopBit)));
    ui->databit->setCurrentIndex(ui->databit->findText(QString::number(AppCfg::DataBit)));
    ui->checkbit->findText(AppCfg::CheckData);
    ui->hexsend->setChecked(AppCfg::HexSend);
    ui->hexrecv->setChecked(AppCfg::HexRecv);
    ui->autosendtime->setCurrentIndex(ui->autosendtime->findText(QString::number(AppCfg::AutoSendTime)));
    ui->senddata->setText(AppCfg::Data);
}

void SerialWidget::builtconnect()
{
    //打开串口
    connect(ui->openbtn,SIGNAL(clicked()),this,SLOT(openserial()));
    //发送数据
    timesend = new QTimer(this);
    timesend->setInterval(ui->autosendtime->currentText().toInt());
    connect(timesend, SIGNAL(timeout()), this, SLOT(sendData()));
    connect(ui->sendbtn, SIGNAL(clicked()), this, SLOT(sendData()));
    //自动发送更改
    connect(ui->autosendtime, SIGNAL(currentIndexChanged(int)), this, SLOT(autosendrestart()));
    //串口设置更改
    connect(ui->portname,SIGNAL(currentIndexChanged(int)),this,SLOT(restart()));
    connect(ui->baudrate, SIGNAL(currentIndexChanged(int)), this, SLOT(restart()));
    connect(ui->stopbit, SIGNAL(currentIndexChanged(int)), this, SLOT(restart()));
    connect(ui->databit, SIGNAL(currentIndexChanged(int)), this, SLOT(restart()));
    connect(ui->checkbit, SIGNAL(currentIndexChanged(int)), this, SLOT(restart()));

    connect(ui->savedatabtn,&QPushButton::clicked,this,[this]
    {
        myHelper::savedata(ui->display,this);
    });
}

void SerialWidget::openserial()
{
    if(ui->openbtn->text()=="打开串口")
    {
        myserial = new QSerialPort(this);
        //设置串口名
        myserial->setPortName(ui->portname->currentText());
        ok = myserial->open(QIODevice::ReadWrite);
        if (ok)
        {
            //清空缓冲区
            myserial->flush();
            //设置波特率
            myserial->setBaudRate(ui->baudrate->currentText().toInt());
            //设置停止位
            switch(ui->stopbit->currentIndex())
            {
            case 0:myserial->setStopBits(QSerialPort::OneStop);break;
#ifdef Q_OS_WIN
            case 1:myserial->setStopBits(QSerialPort::OneAndHalfStop);break;
            case 2:myserial->setStopBits(QSerialPort::TwoStop);break;
#else
            case 1:myserial->setStopBits(QSerialPort::TwoStop);break;
#endif
            default: break;
            }
            //设置数据位
            switch(ui->databit->currentIndex())
            {
            case 0:myserial->setDataBits(QSerialPort::Data5);break;
            case 1:myserial->setDataBits(QSerialPort::Data6);break;
            case 2:myserial->setDataBits(QSerialPort::Data7);break;
            case 3:myserial->setDataBits(QSerialPort::Data8);break;
            default: break;
            }
            //设置校验位
            switch(ui->checkbit->currentIndex())
            {
            case 0:myserial->setParity(QSerialPort::NoParity);break;
            case 1:myserial->setParity(QSerialPort::EvenParity);break;
            case 2:myserial->setParity(QSerialPort::OddParity);break;
            default: break;
            }
            //设置流控制
            myserial->setFlowControl(QSerialPort::NoFlowControl);
            ui->openbtn->setText("关闭串口");
            changeEnable(true);
            append(2,"打开串口成功！");
            //连接信号槽
            QObject::connect(myserial, &QSerialPort::readyRead, this, &SerialWidget::readData);
        }
        else
        {
            append(2,"打开串口失败，请重试！");
        }
    }
    else
    {
        //关闭串口
        myserial->clear();
        myserial->deleteLater();
        myserial->close();
        ui->openbtn->setText("打开串口");
        changeEnable(false);
        append(2,"关闭串口成功！");
    }
}

void SerialWidget::readData()
{
    if (myserial->bytesAvailable() <= 0) {
        return;
    }

    myHelper::sleep(waittime);
    QByteArray data = myserial->readAll();
    int dataLen = data.length();

    if (dataLen <= 0) {
        return;
    }

    QString buffer;//储存数据

    if (ui->hexrecv->isChecked()) {
        buffer = BinaryCvn::byteArrayToHexStr(data);//16进制接受
    } else {
        buffer = BinaryCvn::byteArrayToAsciiStr(data);//ascii码接受
    }

    append(1, buffer);
    recvcount = recvcount + data.size();//接收数据计数
    ui->recvcountbtn->setText(QString("接收 : %1 字节").arg(recvcount));
}

void SerialWidget::sendData()
{
    QString str = ui->senddata->toPlainText();

    if (str.isEmpty()) {
        append(2,"发送数据为空！");
        ui->senddata->setFocus();
        return;
    }

    sendData(str);
}

void SerialWidget::sendData(QString data)
{
    if (myserial == nullptr || !myserial->isOpen()) {//串口未打开
        return;
    }

    QByteArray buffer;

    if (ui->hexsend->isChecked()) {
        buffer = BinaryCvn::hexStrToByteArray(data);
    } else {
        buffer = BinaryCvn::asciiStrToByteArray(data);
    }

    myserial->write(buffer);
    append(0, data);
    sendcount = sendcount + buffer.size();
    ui->sendcountbtn->setText(QString("发送 : %1 字节").arg(sendcount));
}

void SerialWidget::changeEnable(bool b)
{
    ui->autosend->setEnabled(b);
    ui->sendbtn->setEnabled(b);
}

void SerialWidget::on_autosend_stateChanged(int arg1)
{
    if (arg1 == 0)
        timesend->stop();
    else
        timesend->start();
}

void SerialWidget::restart()
{
    if(ui->openbtn->text()=="打开串口")
        return;
    else {
        if(ui->autosend->isChecked())
            timesend->stop();
        myserial->clear();
        myserial->deleteLater();
        myserial->close();
        ok=false;
        changeEnable(false);
    }
    myserial = new QSerialPort(this);
    //设置串口名
    myserial->setPortName(ui->portname->currentText());
    ok = myserial->open(QIODevice::ReadWrite);
    if (ok)
    {
        //清空缓冲区
        myserial->flush();
        //设置波特率
        myserial->setBaudRate(ui->baudrate->currentText().toInt());
        //设置停止位
        switch(ui->stopbit->currentIndex())
        {
        case 0:myserial->setStopBits(QSerialPort::OneStop);break;
#ifdef Q_OS_WIN
        case 1:myserial->setStopBits(QSerialPort::OneAndHalfStop);break;
        case 2:myserial->setStopBits(QSerialPort::TwoStop);break;
#else
        case 1:myserial->setStopBits(QSerialPort::TwoStop);break;
#endif
        default: break;
        }
        //设置数据位
        switch(ui->databit->currentIndex())
        {
        case 0:myserial->setDataBits(QSerialPort::Data5);break;
        case 1:myserial->setDataBits(QSerialPort::Data6);break;
        case 2:myserial->setDataBits(QSerialPort::Data7);break;
        case 3:myserial->setDataBits(QSerialPort::Data8);break;
        default: break;
        }
        //设置校验位
        switch(ui->checkbit->currentIndex())
        {
        case 0:myserial->setParity(QSerialPort::NoParity);break;
        case 1:myserial->setParity(QSerialPort::EvenParity);break;
        case 2:myserial->setParity(QSerialPort::OddParity);break;
        default: break;
        }
        //设置流控制
        myserial->setFlowControl(QSerialPort::NoFlowControl);
        ui->openbtn->setText("关闭串口");
        changeEnable(true);
        append(2,"串口更改设置成功！");
        //连接信号槽
        QObject::connect(myserial, &QSerialPort::readyRead, this, &SerialWidget::readData);
        if(ui->autosend->isChecked())
            timesend->start();
    }
    else
    {
        if(ui->autosend->isChecked())
            ui->autosend->setChecked(false);
        ui->openbtn->setText("打开串口");
        changeEnable(false);
        append(2,"串口设置更改失败，请重新打开串口！");
    }
}

void SerialWidget::autosendrestart()
{
    if (ui->autosend->isChecked()){
        timesend->stop();
        timesend->setInterval(ui->autosendtime->currentText().toInt());
        timesend->start();
    }
}


void SerialWidget::append(quint8 type, QString msg)
{
    QString str;

    if (type == 0)
    {
        str = ">> 串口发送 :";
        ui->display->setTextColor(QColor("dodgerblue"));
    }
    else if (type == 1)
    {
        str = "<< 串口接收 :";
        ui->display->setTextColor(QColor("green"));
    }
    else if (type == 2)
    {
        str = ">> 提示信息 :";
        ui->display->setTextColor(QColor("red"));
    }
    ui->display->append(QString("时间[%1] %2 %3").arg(DATETIME).arg(str).arg(msg));
}

void SerialWidget::on_sendcountbtn_clicked()
{
    sendcount = 0;
    ui->sendcountbtn->setText("发送 : 0 字节");
}

void SerialWidget::on_recvcountbtn_clicked()
{
    recvcount = 0;
    ui->recvcountbtn->setText("接收 : 0 字节");
}

/*
void SerialWidget::on_savedatabtn_clicked()
{
    QString tempData = ui->display->toPlainText();//以纯文本的形式返回文本编辑的文本。

    if (tempData == "")
    {
        append(2,"没有数据可以保存！");
        return;
    }

    QString path = QFileDialog::getSaveFileName(this,
                                                tr("Open File"),
                                                ".",
                                                tr("Text Files(*.txt)"));
    if(!path.isEmpty())
    {
        QFile file(path);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, tr("Write File"),
                                 tr("Can't open file:\n%1").arg(path));
            return;
        }
        QTextStream out(&file);
        out << tempData;
        file.close();
        append(2,"保存文件成功！");
    }
    else
    {
        append(2,"你没有保存任何文件！");
    }
}
*/

void SerialWidget::on_cleardatabtn_clicked()
{
    ui->display->clear();
    sendcount = 0;
    ui->sendcountbtn->setText("发送 : 0 字节");
    recvcount = 0;
    ui->recvcountbtn->setText("接收 : 0 字节");
}

void SerialWidget::on_checkport_clicked()
{
    ui->openbtn->setEnabled(false);
    ui->portname->clear();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort port;
        port.setPort(info);
        if(port.open(QIODevice::ReadWrite))
        {
            //将串口号添加到portname
            ui->portname->addItem(port.portName());
            //关闭串口等待人为(打开串口按钮)打开
            port.close();
        }
    }
    ui->openbtn->setEnabled(true);
}

void SerialWidget::saveconfig()
{
    AppCfg::BaudRate = ui->baudrate->currentText().toInt();
    AppCfg::StopBit = ui->stopbit->currentText().toInt();
    AppCfg::DataBit = ui->databit->currentText().toInt();
    AppCfg::CheckData =ui->checkbit->currentText().toUtf8();
    AppCfg::HexSend = ui->hexsend->isChecked();
    AppCfg::HexRecv = ui->hexrecv->isChecked();
    AppCfg::AutoSendTime =ui->autosendtime->currentText().toInt();
    AppCfg::Data=ui->senddata->toPlainText();
    AppCfg::writeconfig();
}
