#include "AppCfg.h"
#include "MyHelper.h"

#pragma execution_character_set("utf-8")

//TCP
QString AppCfg::tcpModel = "TCPServer";
QString AppCfg::tcpIp = "127.0.0.1";
int AppCfg::tcpPort = 8000;
bool AppCfg::tcpHexSend = true;
bool AppCfg::tcpHexRecv = true;
int AppCfg::tcpAutoSendTime = 1000;
int AppCfg::tcpAutoConnectTime = 1000;
QString AppCfg::tcpData="";

//Serial
int AppCfg::serialBaudRate=9600;
int AppCfg::serialStopBit=1;
int AppCfg::serialDataBit=8;
QString AppCfg::serialCheckData="无";
bool AppCfg::serialHexSend=true;                //16进制发送
bool AppCfg::serialHexRecv=true;                //16进制接收
int AppCfg::serialAutoSendTime=1000;            //自动发送间隔
QString AppCfg::serialData="";

void AppCfg::readConfig()
{
    if (!checkConfig()) {
        qDebug()<<"check error";
        return;
    }

    QString fileName = QString("%1/cfg/config.ini").arg(AppPath);
    QSettings set(fileName, QSettings::IniFormat);

    set.beginGroup("tcpconfig");//向当前组追加前缀
    AppCfg::tcpModel = set.value("model").toString();
    AppCfg::tcpIp = set.value("ip").toString();
    AppCfg::tcpPort = set.value("port").toInt();
    AppCfg::tcpHexSend = set.value("hexsend").toBool();
    AppCfg::tcpHexRecv = set.value("hexrecv").toBool();
    AppCfg::tcpAutoSendTime = set.value("autosendtime").toInt();
    AppCfg::tcpAutoConnectTime = set.value("autoconnecttime").toInt();
    AppCfg::tcpData=set.value("senddata").toString();
    set.endGroup();

    set.beginGroup("serialconfig");//向当前组追加前缀
    AppCfg::serialBaudRate = set.value("BaudRate").toInt();
    AppCfg::serialStopBit = set.value("StopBit").toInt();
    AppCfg::serialDataBit = set.value("DataBit").toInt();
    AppCfg::serialCheckData = set.value("CheckData").toString();
    AppCfg::serialHexSend = set.value("HexSend").toBool();
    AppCfg::serialHexRecv = set.value("HexRecv").toBool();
    AppCfg::serialAutoSendTime = set.value("AutoSendTime").toInt();
    AppCfg::serialData=set.value("SendData").toString();
    set.endGroup();
}

void AppCfg::writeConfig()
{
    QString fileName = QString("%1/cfg/config.ini").arg(AppPath);
    QSettings set(fileName, QSettings::IniFormat);

    set.beginGroup("tcpconfig");
    set.setValue("model", AppCfg::tcpModel);
    set.setValue("ip", AppCfg::tcpIp);
    set.setValue("port", AppCfg::tcpPort);
    set.setValue("hexsend", AppCfg::tcpHexSend);
    set.setValue("hexrecv", AppCfg::tcpHexRecv);
    set.setValue("autosendtime", AppCfg::tcpAutoSendTime);
    set.setValue("autoconnecttime", AppCfg::tcpAutoConnectTime);
    set.setValue("senddata",AppCfg::tcpData);
    set.endGroup();

    set.beginGroup("serialconfig");
    set.setValue("BaudRate", AppCfg::serialBaudRate);
    set.setValue("StopBit", AppCfg::serialStopBit);
    set.setValue("DataBit", AppCfg::serialDataBit);
    set.setValue("CheckData", AppCfg::serialCheckData);
    set.setValue("HexSend", AppCfg::serialHexSend);
    set.setValue("HexRecv", AppCfg::serialHexRecv);
    set.setValue("AutoSendTime", AppCfg::serialAutoSendTime);
    set.setValue("SendData",AppCfg::serialData);
    set.endGroup();
}

void AppCfg::newConfig()
{
    writeConfig();
}


bool AppCfg::checkConfig()
{
    QString fileName = QString("%1/cfg/config.ini").arg(AppPath);

    //如果配置文件大小为0,则以初始值继续运行,并生成配置文件
    QFile file(fileName);

    if (file.size() == 0) {
        qDebug()<<"filesize=0";
        newConfig();
        return false;
    }
    return true;
}

void AppCfg::writeError(QString str)
{
    QString fileName = QString("%1/cfg/Error_%2.txt").arg(AppPath).arg(QDATE);
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Append | QFile::Text);
    QTextStream stream(&file);
    stream << DATETIME << "  " << str << "\n";
}

void AppCfg::newDir(QString dirname)
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

