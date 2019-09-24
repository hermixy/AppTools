#ifndef SERIAL_H
#define SERIAL_H

#include <QMainWindow>
#include <QSerialPort>

#pragma execution_character_set("utf-8")

namespace Ui {
class SerialWidget;
}

class serial : public QWidget
{
    Q_OBJECT

public:
    explicit serial(QWidget *parent = nullptr);
    ~serial();

private slots:
    void initdata();
    void initform();
    void builtconnect();
    void openserial();
    void readData();                //读取串口数据
    void sendData();                //发送串口数据
    void sendData(QString data);    //发送串口数据带参数
    void changeEnable(bool b);      //改变状态

    void restart();
    void autosendrestart();
    void append(quint8 type, QString msg);//显示
    void on_autosend_stateChanged(int arg1);
    void on_sendcountbtn_clicked();
    void on_recvcountbtn_clicked();
    //void on_savedatabtn_clicked();
    void on_cleardatabtn_clicked();
    void on_checkport_clicked();

    void saveconfig();

private:
    Ui::SerialWidget *ui;

    QSerialPort *myserial;   //串口通信对象
    QTimer *timesend;        //定时发送串口数据
    bool ok;

    int waittime;           //接收延时时间
    int recvcount;          //接收数据计数
    int sendcount;          //发送计数
};
#endif // SERIAL_H
