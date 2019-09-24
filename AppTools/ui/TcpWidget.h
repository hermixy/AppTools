#ifndef TCPWIDGET_H
#define TCPWIDGET_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>

namespace Ui {
class TcpWidget;
}

class TcpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TcpWidget(QWidget *parent = nullptr);
    ~TcpWidget();

private slots:
    void initform();
    void choose();
    void initdata();
    void builtconnect();
    void change(bool b);
    void tcpopen();
    void append(quint8 type, QString msg);
    void senddata();
    void on_autosend_stateChanged(int arg1);
    void autosendrestart();

    void NewConnectionSlot();       //sever
    void disconnectedSlot();
    void server_read();

    void ReadError(QAbstractSocket::SocketError);//client
    void client_read();
    void autoconnectrestart();
    void connectrestart();
    void on_autoconnect_stateChanged(int arg1);
    void on_sendcount_clicked();
    void on_recvcount_clicked();
    //void on_savedata_clicked();
    void on_cleardata_clicked();

    void saveconfig();

private:
    Ui::TcpWidget *ui;

    QTcpServer *tcpserver;          //server
    QList<QTcpSocket*> client;
    QTcpSocket *currentclient;

    QTcpSocket *tcpclient;          //client
    QTimer *timeconnect;

    bool ok;
    int recvcount;                  //接收数据计数
    int sendcount;                  //发送计数
    QTimer *timesend;               //定时发送串口数据
};

#endif // TCPWIDGET_H
