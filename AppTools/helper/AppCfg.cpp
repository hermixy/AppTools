#include "AppCfg.h"
#include "myHelper.h"

#pragma execution_character_set("utf-8")

//TCP
QString AppCfg::model = "TCPServer";
QString AppCfg::ip = "127.0.0.1";
int AppCfg::port = 8000;
bool AppCfg::hexsend = true;
bool AppCfg::hexrecv = true;
int AppCfg::autosendtime = 1000;
int AppCfg::autoconnecttime = 1000;
QString AppCfg::data="";

//Serial
int AppCfg::BaudRate=9600;
int AppCfg::StopBit=1;
int AppCfg::DataBit=8;
QString AppCfg::CheckData="无";
bool AppCfg::HexSend=true;                //16进制发送
bool AppCfg::HexRecv=true;                //16进制接收
int AppCfg::AutoSendTime=1000;            //自动发送间隔
QString AppCfg::Data="";

void AppCfg::readconfig()
{
    if (!checkconfig()) {
        qDebug()<<"check error";
        return;
    }

    QString fileName = QString("%1/cfg/config.ini").arg(AppPath);
    QSettings set(fileName, QSettings::IniFormat);

    set.beginGroup("tcpconfig");//向当前组追加前缀
    AppCfg::model = set.value("model").toString();
    AppCfg::ip = set.value("ip").toString();
    AppCfg::port = set.value("port").toInt();
    AppCfg::hexsend = set.value("hexsend").toBool();
    AppCfg::hexrecv = set.value("hexrecv").toBool();
    AppCfg::autosendtime = set.value("autosendtime").toInt();
    AppCfg::autoconnecttime = set.value("autoconnecttime").toInt();
    AppCfg::data=set.value("senddata").toString();
    set.endGroup();

    set.beginGroup("serialconfig");//向当前组追加前缀
    AppCfg::BaudRate = set.value("BaudRate").toInt();
    AppCfg::StopBit = set.value("StopBit").toInt();
    AppCfg::DataBit = set.value("DataBit").toInt();
    AppCfg::CheckData = set.value("CheckData").toString();
    AppCfg::HexSend = set.value("HexSend").toBool();
    AppCfg::HexRecv = set.value("HexRecv").toBool();
    AppCfg::AutoSendTime = set.value("AutoSendTime").toInt();
    AppCfg::Data=set.value("SendData").toString();
    set.endGroup();
}

void AppCfg::writeconfig()
{
    QString fileName = QString("%1/cfg/config.ini").arg(AppPath);
    QSettings set(fileName, QSettings::IniFormat);

    set.beginGroup("tcpconfig");
    set.setValue("model", AppCfg::model);
    set.setValue("ip", AppCfg::ip);
    set.setValue("port", AppCfg::port);
    set.setValue("hexsend", AppCfg::hexsend);
    set.setValue("hexrecv", AppCfg::hexrecv);
    set.setValue("autosendtime", AppCfg::autosendtime);
    set.setValue("autoconnecttime", AppCfg::autoconnecttime);
    set.setValue("senddata",AppCfg::data);
    set.endGroup();

    set.beginGroup("serialconfig");
    set.setValue("BaudRate", AppCfg::BaudRate);
    set.setValue("StopBit", AppCfg::StopBit);
    set.setValue("DataBit", AppCfg::DataBit);
    set.setValue("CheckData", AppCfg::CheckData);
    set.setValue("HexSend", AppCfg::HexSend);
    set.setValue("HexRecv", AppCfg::HexRecv);
    set.setValue("AutoSendTime", AppCfg::AutoSendTime);
    set.setValue("SendData",AppCfg::Data);
    set.endGroup();
}

void AppCfg::newconfig()
{
    writeconfig();
}


bool AppCfg::checkconfig()
{
    QString fileName = QString("%1/cfg/config.ini").arg(AppPath);

    //如果配置文件大小为0,则以初始值继续运行,并生成配置文件
    QFile file(fileName);

    if (file.size() == 0) {
        qDebug()<<"filesize=0";
        newconfig();
        return false;
    }
    return true;
}

void AppCfg::writeerror(QString str)
{
    QString fileName = QString("%1/cfg/Error_%2.txt").arg(AppPath).arg(QDATE);
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Append | QFile::Text);
    QTextStream stream(&file);
    stream << DATETIME << "  " << str << "\n";
}

void AppCfg::newdir(QString dirname)
{
    //如果路径中包含斜杠字符则说明是绝对路径
    //linux系统路径字符带有 /  windows系统 路径字符带有 :/
    if (!dirname.startsWith("/") && !dirname.contains(":/"))
    {
        dirname = QString("%1/%2").arg(AppPath).arg(dirname);
    }

    QDir dir(dirname);

    if (!dir.exists())
    {
        dir.mkpath(dirname);
    }
}

