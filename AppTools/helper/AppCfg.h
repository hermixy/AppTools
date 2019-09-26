#ifndef APPCFG_H
#define APPCFG_H

class QString;

class AppCfg
{
public:
    //TCP
    static QString tcpModel;               //TCP模式
    static QString tcpIp;                  //服务器IP
    static int tcpPort;                    //监听端口
    static bool tcpHexSend;                //16进制发送
    static bool tcpHexRecv;                //16进制接收
    static int tcpAutoSendTime;            //自动发送间隔
    static int tcpAutoConnectTime;         //自动重连
    static QString tcpData;                //发送数据

    //Serial
    static int serialBaudRate;
    static int serialStopBit;
    static int serialDataBit;
    static QString serialCheckData;
    static bool serialHexSend;                //16进制发送
    static bool serialHexRecv;                //16进制接收
    static int serialAutoSendTime;            //自动发送间隔
    static QString serialData;                //发送数据

    static void readConfig();           //读取配置文件,在main函数最开始加载程序载入
    static void writeConfig();          //写入配置文件,在更改配置文件程序关闭时调用
    static void newConfig();            //以初始值新建配置文件
    static bool checkConfig();          //校验配置文件
    static void writeError(QString str);//写入错误信息
    static void newDir(QString dirname);//新建目录

};

#endif // APPCFG_H
