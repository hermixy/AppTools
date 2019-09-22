#ifndef APP_H
#define APP_H

class QString;

class appCfg
{
public:
    //TCP
    static QString model;               //TCP模式
    static QString ip;                  //服务器IP
    static int port;                    //监听端口
    static bool hexsend;                //16进制发送
    static bool hexrecv;                //16进制接收
    static int autosendtime;            //自动发送间隔
    static int autoconnecttime;         //自动重连
    static QString data;                //发送数据

    //Serial
    static int BaudRate;
    static int StopBit;
    static int DataBit;
    static QString CheckData;
    static bool HexSend;                //16进制发送
    static bool HexRecv;                //16进制接收
    static int AutoSendTime;            //自动发送间隔
    static QString Data;                //发送数据

    static void readconfig();           //读取配置文件,在main函数最开始加载程序载入
    static void writeconfig();          //写入配置文件,在更改配置文件程序关闭时调用
    static void newconfig();            //以初始值新建配置文件
    //static bool checkconfig();          //校验配置文件
    static void writeerror(QString str);//写入错误信息
    static void newdir(QString dirname);//新建目录

};

#endif // APP_H
