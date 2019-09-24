#include "appCfg.h"
#include "myHelper.h"

#pragma execution_character_set("utf-8")

//TCP
QString appCfg::model = "TCPServer";
QString appCfg::ip = "127.0.0.1";
int appCfg::port = 8000;
bool appCfg::hexsend = true;
bool appCfg::hexrecv = true;
int appCfg::autosendtime = 1000;
int appCfg::autoconnecttime = 1000;
QString appCfg::data="";

//Serial
int appCfg::BaudRate=9600;
int appCfg::StopBit=1;
int appCfg::DataBit=8;
QString appCfg::CheckData="无";
bool appCfg::HexSend=true;                //16进制发送
bool appCfg::HexRecv=true;                //16进制接收
int appCfg::AutoSendTime=1000;            //自动发送间隔
QString appCfg::Data="";

void appCfg::readconfig()
{
    if (!checkconfig()) {
        qDebug()<<"check error";
        return;
    }

    QString fileName = QString("%1/cfg/config.ini").arg(AppPath);
    QSettings set(fileName, QSettings::IniFormat);

    set.beginGroup("tcpconfig");//向当前组追加前缀
    appCfg::model = set.value("model").toString();
    appCfg::ip = set.value("ip").toString();
    appCfg::port = set.value("port").toInt();
    appCfg::hexsend = set.value("hexsend").toBool();
    appCfg::hexrecv = set.value("hexrecv").toBool();
    appCfg::autosendtime = set.value("autosendtime").toInt();
    appCfg::autoconnecttime = set.value("autoconnecttime").toInt();
    appCfg::data=set.value("senddata").toString();
    set.endGroup();

    set.beginGroup("serialconfig");//向当前组追加前缀
    appCfg::BaudRate = set.value("BaudRate").toInt();
    appCfg::StopBit = set.value("StopBit").toInt();
    appCfg::DataBit = set.value("DataBit").toInt();
    appCfg::CheckData = set.value("CheckData").toString();
    appCfg::HexSend = set.value("HexSend").toBool();
    appCfg::HexRecv = set.value("HexRecv").toBool();
    appCfg::AutoSendTime = set.value("AutoSendTime").toInt();
    appCfg::Data=set.value("SendData").toString();
    set.endGroup();
}

void appCfg::writeconfig()
{
    QString fileName = QString("%1/cfg/config.ini").arg(AppPath);
    QSettings set(fileName, QSettings::IniFormat);

    set.beginGroup("tcpconfig");
    set.setValue("model", appCfg::model);
    set.setValue("ip", appCfg::ip);
    set.setValue("port", appCfg::port);
    set.setValue("hexsend", appCfg::hexsend);
    set.setValue("hexrecv", appCfg::hexrecv);
    set.setValue("autosendtime", appCfg::autosendtime);
    set.setValue("autoconnecttime", appCfg::autoconnecttime);
    set.setValue("senddata",appCfg::data);
    set.endGroup();

    set.beginGroup("serialconfig");
    set.setValue("BaudRate", appCfg::BaudRate);
    set.setValue("StopBit", appCfg::StopBit);
    set.setValue("DataBit", appCfg::DataBit);
    set.setValue("CheckData", appCfg::CheckData);
    set.setValue("HexSend", appCfg::HexSend);
    set.setValue("HexRecv", appCfg::HexRecv);
    set.setValue("AutoSendTime", appCfg::AutoSendTime);
    set.setValue("SendData",appCfg::Data);
    set.endGroup();
}

void appCfg::newconfig()
{
    writeconfig();
}


bool appCfg::checkconfig()
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

void appCfg::writeerror(QString str)
{
    QString fileName = QString("%1/cfg/Error_%2.txt").arg(AppPath).arg(QDATE);
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Append | QFile::Text);
    QTextStream stream(&file);
    stream << DATETIME << "  " << str << "\n";
}

void appCfg::newdir(QString dirname)
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

