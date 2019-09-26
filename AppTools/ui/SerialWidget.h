#ifndef SERIALWIDGET_H
#define SERIALWIDGET_H

#include <QMainWindow>
#include <QSerialPort>

#pragma execution_character_set("utf-8")

namespace Ui {
class SerialWidget;
}

class SerialWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SerialWidget(QWidget *parent = nullptr);
    ~SerialWidget();

private slots:
    void initData();
    void initWindow();
    void builtConnect();
    void openSerial();
    void readData();                //读取串口数据
    void sendData();                //发送串口数据
    void sendData(QString data);    //发送串口数据带参数
    void changeEnable(bool b);      //改变状态

    void restart();
    void autoSendRestart();
    void append(quint8 type, QString msg);//显示
    void on_autosend_stateChanged(int arg1);
    void on_sendcountbtn_clicked();
    void on_recvcountbtn_clicked();
    //void on_savedatabtn_clicked();
    void on_cleardatabtn_clicked();
    void on_checkport_clicked();

    void saveConfig();

private:
    Ui::SerialWidget *ui;

    QSerialPort *mySerial;   //串口通信对象
    QTimer *sendTime;        //定时发送串口数据
    bool ok;

    int waitTime;           //接收延时时间
    int recvCount;          //接收数据计数
    int sendCount;          //发送计数
};
#endif // SERIALWIDGET_H
