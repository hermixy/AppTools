#include "helper/myHelper.h"
#include "helper/AppCfg.h"
#include "TcpWidget.h"
#include "ui_TcpWidget.h"
#include "helper/BinaryCvn.h"

#include <QNetworkInterface>
#include <QTcpSocket>

#ifdef Q_OS_WIN
#pragma execution_character_set("utf-8")
#endif

TcpWidget::TcpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpWidget)
{
    ui->setupUi(this);
    tcpserver = new QTcpServer(this);
    tcpclient = new QTcpSocket(this);   //实例化tcpClient
    initform();
    choose();
    builtconnect();
}

TcpWidget::~TcpWidget()
{
    saveconfig();
    delete ui;
}

void TcpWidget::initform()
{
    QStringList modellist;
    modellist<<"TCPServer"<<"TCPClient";
    ui->model->addItems(modellist);
    //ui->model->setCurrentIndex(0);

    //IP输入框正则
    QRegExp regExp("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
    QValidator *validator = new QRegExpValidator(regExp, ui->ip);
    ui->ip->setValidator(validator);
    ui->port->setValidator(new QIntValidator(0, 65536,ui->port));

    //ui->port->setText("8000");

    QStringList sendInterval;//自动发送间隔
    for (int i = 100; i < 1000; i = i + 100) {
        sendInterval << QString::number(i);
    }
    for (int i = 1000; i <= 10000; i = i + 1000) {
        sendInterval << QString::number(i);
    }
    ui->autosendtime->addItems(sendInterval);
    //ui->autosendtime->setCurrentIndex(9);
    ui->autoconnecttime->addItems(sendInterval);
    //ui->autoconnecttime->setCurrentIndex(9);

    ui->allconnect->addItem("全部连接");

    //ui->hexsend->setChecked(true);
    //ui->hexrecv->setChecked(true);
    ui->autosend->setChecked(false);
    ui->autosend->setEnabled(false);
    ui->sendbtn->setEnabled(false);

    //ui->display->setFontPointSize(10);//设置字体大小
    QApplication::setFont(QFont ("微软雅黑", 10, QFont::Normal, false));

    AppCfg::readconfig();                          //读取配置
    ui->model->findText(AppCfg::model);
    ui->ip->setText(AppCfg::ip);
    ui->port->setText(QString::number(AppCfg::port));
    ui->hexrecv->setChecked(AppCfg::hexrecv);
    ui->hexsend->setChecked(AppCfg::hexsend);
    ui->autosendtime->setCurrentIndex(ui->autosendtime->findText(QString::number(AppCfg::autosendtime)));
    ui->autoconnecttime->setCurrentIndex(ui->autoconnecttime->findText(QString::number(AppCfg::autosendtime)));
    ui->senddata->setText(AppCfg::data);
    //qDebug()<<AppCfg::port;
}

void TcpWidget::initdata()
{
    ok=false;
    recvcount = 0;
    sendcount = 0;
    ui->sendcount->setText("发送 : 0 字节");
    ui->recvcount->setText("接收 : 0 字节");

    ui->iplist->clear();
    QStringList iplistbox;
    QList<QHostAddress> iplist = QNetworkInterface::allAddresses(); //获得IP
    foreach(QHostAddress address,iplist)
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            iplistbox<<address.toString();
        }
    }
    ui->iplist->addItems(iplistbox);
    //ui->ip->setText(iplistbox[0]);
}

void TcpWidget::choose()
{
    initdata();
    if(ui->model->currentIndex()==0)
    {
        ui->listen->setText("监听");
        ui->ip->hide();
        ui->iplist->show();
        ui->allconnect->show();
        ui->autoconnect->hide();
        ui->autoconnecttime->hide();
        ui->ip->setReadOnly(true);
        ui->iplabel->setText("本机IP列表:");
        ui->portlabel->setText("本机端口:");
    }
    else if(ui->model->currentIndex()==1)
    {
        ui->listen->setText("连接");
        ui->ip->show();
        ui->iplist->hide();
        ui->allconnect->hide();
        ui->autoconnect->show();
        ui->autoconnecttime->show();
        ui->ip->setReadOnly(false);
        ui->iplabel->setText("服务器IP:");
        ui->portlabel->setText("服务器端口:");
        tcpclient->abort();                 //取消原有连接
    }
}

void TcpWidget::builtconnect()
{    //
    connect(ui->model, SIGNAL(currentIndexChanged(int)), this, SLOT(choose()));
    connect(ui->listen,SIGNAL(clicked()),this,SLOT(tcpopen()));

    connect(tcpserver, SIGNAL(newConnection()), this, SLOT(NewConnectionSlot()));

    timesend = new QTimer(this);
    timesend->setInterval(ui->autosendtime->currentText().toInt());
    connect(ui->autosendtime, SIGNAL(currentIndexChanged(int)), this, SLOT(autosendrestart()));
    //发送数据
    connect(timesend, SIGNAL(timeout()), this, SLOT(senddata()));
    connect(ui->sendbtn, SIGNAL(clicked()), this, SLOT(senddata()));

    timeconnect=new QTimer(this);
    timeconnect->setInterval(ui->autoconnecttime->currentText().toInt());
    connect(ui->autoconnecttime,SIGNAL(currentIndexChanged(int)), this, SLOT(autoconnectrestart()));
    connect(timeconnect, SIGNAL(timeout()), this, SLOT(connectrestart()));
    connect(tcpclient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(ReadError(QAbstractSocket::SocketError)));
    connect(tcpclient, SIGNAL(readyRead()), this, SLOT(client_read()));

    connect(ui->savedata,&QPushButton::clicked,[this]
    {
        myHelper::savedata(ui->display,this);
    });
}

void TcpWidget::change(bool b)
{
    ui->model->setEnabled(b);
    ui->ip->setEnabled(b);
    ui->port->setEnabled(b);
    ui->sendbtn->setEnabled(!b);
    ui->autosend->setEnabled(!b);
}

void TcpWidget::tcpopen()
{
    if(ui->listen->text()=="监听")
    {
        if(ui->port->text().isEmpty()){
            append(2,"端口号为空!");
            return;
        }
        ok = tcpserver->listen(QHostAddress::Any, quint16(ui->port->text().toInt()));
        if(ok)
        {
            ui->listen->setText("断开");
            change(false);
            append(3,"监听成功!");
        }
        else
        {
            append(2,"监听失败，更换端口号,请重试!");
        }
    }
    else if(ui->listen->text()=="断开")
    {
        change(true);
        ui->autoconnect->setChecked(false);
        timesend->stop();
        for(int i=0; i<client.length(); i++)//断开所有连接
        {
            ui->allconnect->removeItem(ui->allconnect->findText(tr("%1:%2")\
                                                                .arg(client[i]->peerAddress().toString().split("::ffff:")[1])\
                                       .arg(client[i]->peerPort())));
            QString msg1=QString("%1[%2:%3]").arg("IP").arg(client[i]->peerAddress().toString().split("::ffff:")[1])\
                    .arg(client[i]->peerPort());
            append(4,msg1);
            //tcpClient123.at(i)->close();
            client[i]->deleteLater();
            client.removeAt(i);  //从保存的客户端列表中取去除
        }
        tcpserver->close();     //不再监听端口
        ui->listen->setText("监听");
        append(3,"监听关闭!");
        ok=false;
    }
    else if(ui->listen->text()=="连接")
    {
        if(ui->port->text().isEmpty()||ui->port->text().isEmpty())
        {
            append(2,"服务器IP或者端口号为空!");
            return;
        }
        tcpclient->connectToHost(ui->ip->text(), quint16(ui->port->text().toInt()));
        if (tcpclient->waitForConnected(1000))  // 连接成功则进入if{}
        {
            ui->listen->setText("断开连接");
            change(false);
            append(3,"连接到服务器成功!");
        }
        else
            append(2,"连接到服务器失败!");
    }
    else if(ui->listen->text()=="断开连接")
    {
        tcpclient->disconnectFromHost();
        if (tcpclient->state() == QAbstractSocket::UnconnectedState|| tcpclient->waitForDisconnected(1000))  //已断开连接则进入if{}
        {
            ui->listen->setText("连接");
            ui->autoconnect->setChecked(false);
            change(true);
            timeconnect->stop();
            timesend->stop();
            ui->autosend->setChecked(false);
            append(3,"断开连接成功!");
        }
    }
}

// newConnection -> newConnectionSlot 新连接建立的槽函数
void TcpWidget::NewConnectionSlot()
{
    currentclient = tcpserver->nextPendingConnection();
    client.append(currentclient);
    ui->allconnect->addItem(tr("%1:%2").arg(currentclient->peerAddress().toString().split("::ffff:")[1])\
            .arg(currentclient->peerPort()));
    QString msg2=QString("%1[%2:%3]").arg("IP").arg(currentclient->peerAddress().toString().split("::ffff:")[1])\
            .arg(currentclient->peerPort());
    append(4,msg2);

    connect(currentclient, SIGNAL(readyRead()), this, SLOT(server_read()));
    connect(currentclient, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));
}

// disconnected -> disconnectedSlot 客户端断开连接的槽函数
void TcpWidget::disconnectedSlot()
{
    //由于disconnected信号并未提供SocketDescriptor，所以需要遍历寻找
    for(int i=0; i<client.length(); i++)
    {
        if(client[i]->state() == QAbstractSocket::UnconnectedState)
        {
            // 删除存储在combox中的客户端信息
            ui->allconnect->removeItem(ui->allconnect->findText(tr("%1:%2")\
                                                                .arg(client[i]->peerAddress().toString().split("::ffff:")[1])\
                                       .arg(client[i]->peerPort())));
            //
            QString msg3=QString("%1[%2:%3]").arg("IP").arg(client[i]->peerAddress().toString().split("::ffff:")[1])\
                    .arg(client[i]->peerPort());
            append(4,msg3);
            // 删除存储在tcpClient123列表中的客户端信息
            client[i]->deleteLater();
            client.removeAt(i);
        }
    }
}

void TcpWidget::ReadError(QAbstractSocket::SocketError)
{
    tcpclient->disconnectFromHost();
    ui->listen->setText(tr("连接"));
    change(true);
    timesend->stop();
    ui->autosend->setChecked(false);
    append(2,QString("failed to connect server because %1,Please Try Again").arg(tcpclient->errorString()));
    append(2,"连接服务器失败，请重试!");
}

// 客户端数据可读信号，对应的读数据槽函数
void TcpWidget::server_read()
{
    // 由于readyRead信号并未提供SocketDecriptor，所以需要遍历所有客户端
    for(int i=0; i<client.length(); i++)
    {
        QByteArray data = client[i]->readAll();
        if(data.isEmpty())    continue;

        QString buffer;//储存数据
        if (ui->hexrecv->isChecked()) {
            buffer = BinaryCvn::byteArrayToHexStr(data);//16进制接受
        }
        else {
            //buffer = myHelper::byteArrayToAsciiStr(data);//ascii码接受
            buffer =data;
        }

        static QString IP_Port;
        IP_Port = tr("[%1:%2]:").arg(client[i]->peerAddress().toString().split("::ffff:")[1])\
                .arg(client[i]->peerPort());

        QString buffer1=QString("%1%2%3").arg("IP ").arg(IP_Port).arg(buffer);

        append(1, buffer1);

        recvcount = recvcount + data.size();//接收数据计数
        ui->recvcount->setText(QString("接收 : %1 字节").arg(recvcount));
    }
}

void TcpWidget::senddata()
{
    QString data = ui->senddata->toPlainText();
    if (data.isEmpty()) {
        append(2,"发送数据为空！");
        ui->senddata->setFocus();
        return;
    }

    QByteArray buffer;
    if (ui->hexsend->isChecked()) {
        buffer = BinaryCvn::hexStrToByteArray(data);
    } else {
        //buffer = myHelper::asciiStrToByteArray(data);
        buffer=data.toUtf8();
    }
    append(0, data);
    sendcount = sendcount + buffer.size();
    ui->sendcount->setText(QString("发送 : %1 字节").arg(sendcount));

    if(ui->model->currentIndex()==0)
    {
        if(client.length()==0)
        {
            append(2,"当前无客户端在线，请停止发送数据,无效！");
            return;
        }
        //全部连接
        if(ui->allconnect->currentIndex() == 0)
        {
            for(int i=0; i<client.length(); i++)
                client[i]->write(buffer); //qt5除去了.toAscii()
        }
        //指定连接
        else
        {
            QString clientIP = ui->allconnect->currentText().split(":")[0];
            int clientPort = ui->allconnect->currentText().split(":")[1].toInt();
            //        qDebug() << clientIP;
            //        qDebug() << clientPort;
            for(int i=0; i<client.length(); i++)
            {
                if(client[i]->peerAddress().toString().split("::ffff:")[1]==clientIP\
                        && client[i]->peerPort()==clientPort)
                {
                    client[i]->write(buffer);
                    return; //ip:port唯一，无需继续检索
                }
            }
        }
    }
    else if(ui->model->currentIndex()==1)
    {
        tcpclient->write(buffer);
    }
}

void TcpWidget::client_read()
{
    QByteArray data = tcpclient->readAll();
    if(data.isEmpty())    return;

    QString buffer;//储存数据
    if (ui->hexrecv->isChecked()) {
        buffer = BinaryCvn::byteArrayToHexStr(data);//16进制接受
    } else {
        //buffer = myHelper::byteArrayToAsciiStr(data);//ascii码接受
        buffer=data;
    }
    append(1, buffer);

    recvcount = recvcount + data.size();//接收数据计数
    ui->recvcount->setText(QString("接收 : %1 字节").arg(recvcount));
}

void TcpWidget::on_autosend_stateChanged(int arg1)
{
    if (arg1 == 0)
        timesend->stop();
    else
        timesend->start();
}

void TcpWidget::autosendrestart()
{
    if (ui->autosend->isChecked()){
        timesend->stop();
        timesend->setInterval(ui->autosendtime->currentText().toInt());
        timesend->start();
    }
}

void TcpWidget::append(quint8 type, QString msg)
{   
    QString str;

    if (type == 0) {
        str = ">> 网络发送 :";
        ui->display->setTextColor(QColor("black"));
    }else if (type == 1) {
        str = "<< 网络接收 :";
        ui->display->setTextColor(QColor("dodgerblue"));
    }else if (type == 2) {
        str = "<< 提示信息 :";
        ui->display->setTextColor(QColor("red"));
    }else if (type == 3) {
        str = "<< 提示信息 :";
        ui->display->setTextColor(QColor("green"));
    }else if (type == 4) {
        str = ">> 断开链接:";
        ui->display->setTextColor(QColor("red"));
    }
    ui->display->append(QString("时间[%1] %2 %3").arg(DATETIME).arg(str).arg(msg));
}

void TcpWidget::autoconnectrestart()
{
    if (ui->autoconnect->isChecked()){
        timeconnect->stop();
        timeconnect->setInterval(ui->autoconnecttime->currentText().toInt());
        timeconnect->start();
    }
}

void TcpWidget::on_autoconnect_stateChanged(int arg1)
{
    if (arg1 == 0)
    {
        timeconnect->stop();
        if(ui->listen->text()=="连接")
        {
            change(true);
        }
        else if(ui->listen->text()=="断开连接")
        {
            change(false);
        }
    }
    else
    {
        timeconnect->start();
        ui->model->setEnabled(false);
        ui->ip->setEnabled(false);
        ui->port->setEnabled(false);
        if(ui->listen->text()=="连接")
        {
            ui->sendbtn->setEnabled(false);
            ui->autosend->setEnabled(false);
        }
        else if(ui->listen->text()=="断开连接")
        {
            ui->sendbtn->setEnabled(true);
            ui->autosend->setEnabled(true);
        }
    }
}

void TcpWidget::connectrestart()
{
    if(ui->listen->text()=="连接")
    {
        tcpclient->connectToHost(ui->ip->text(), quint16(ui->port->text().toInt()));
        if (tcpclient->waitForConnected(1000))  // 连接成功则进入if{}
        {
            ui->listen->setText("断开连接");
            change(false);
            append(3,"连接到服务器成功!");
        }
        else {
            append(2,"服务器未打开，连接到服务器失败！");
        }
    }
}

void TcpWidget::on_sendcount_clicked()
{
    sendcount = 0;
    ui->sendcount->setText("发送 : 0 字节");
}

void TcpWidget::on_recvcount_clicked()
{
    recvcount = 0;
    ui->recvcount->setText("接收 : 0 字节");
}

/*
void TcpWidget::on_savedata_clicked()
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
        append(3,"保存文件成功！");
    }
    else
    {
        append(2,"你没有保存任何文件！");
    }
}
*/
void TcpWidget::on_cleardata_clicked()
{
    ui->display->clear();
    sendcount = 0;
    ui->sendcount->setText("发送 : 0 字节");
    recvcount = 0;
    ui->recvcount->setText("接收 : 0 字节");
}

void TcpWidget::saveconfig()
{
    AppCfg::model=ui->model->currentText();
    AppCfg::ip=ui->ip->text().toUtf8();
    AppCfg::port=ui->port->text().toInt();
    AppCfg::hexrecv=ui->hexrecv->isChecked();
    AppCfg::hexsend=ui->hexsend->isChecked();
    AppCfg::autosendtime=ui->autosendtime->currentText().toInt();
    AppCfg::autoconnecttime=ui->autoconnecttime->currentText().toInt();
    AppCfg::data=ui->senddata->toPlainText();
    AppCfg::writeconfig();
}
