#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myHelper.h"
#include "SerialWidget.h"
#include "TcpWidget.h"
#include "CustomWidget.h"
#include "floatcvn.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,Frm(nullptr)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);   //去掉边框
    setAttribute(Qt::WA_StyledBackground);

    initWindow();
    setPaddingAndSpacing();
    titlebtn();

    createbtnmenu();
    registerClass();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initWindow()
{
    myHelper::InCenter(this);
    setWindowTitle("AppTools");

    sizeGrip=new QSizeGrip(nullptr);
    QGridLayout *l = qobject_cast<QGridLayout*>(ui->centralwidget->layout());
    // 添加 size grip 到窗口右下角
    l->addWidget(sizeGrip, 1, 1, Qt::AlignRight | Qt::AlignBottom);

    ui->label->setFont(QFont("微软雅黑", 14, QFont::Normal, false));
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->setText("AppTools");
    ui->labelicon->setPixmap(QPixmap(":/ico/setting-icon-dark.png"));

    Frm=new CustomWidget(QString("你好呀!"));
    if(Frm!=nullptr)
    {
        ui->stackedWidget->addWidget(Frm);
        ui->stackedWidget->setCurrentWidget(Frm);
        Frm->setAttribute(Qt::WA_DeleteOnClose,true);
    }
}

void MainWindow::setPaddingAndSpacing()
{
    setWidgetPaddingAndSpacing(ui->centralwidget,0,0);
    setWidgetPaddingAndSpacing(ui->titlewidget,5,10);
    setWidgetPaddingAndSpacing(ui->listWidget,0,0);
    //setWidgetPaddingAndSpacing(ui->page,0,0);
}

void MainWindow::setWidgetPaddingAndSpacing(QWidget *widget, int padding, int spacing)
{
    // 设置 Widget 的 padding 和 spacing
    QLayout *layout = widget->layout();

    if (layout)
    {
        layout->setContentsMargins(padding, padding, padding, padding);
        layout->setSpacing(spacing);
    }
}

void MainWindow::titlebtn()
{
    ui->minbtn->hide();
    connect(ui->closebtn,&QPushButton::clicked,[]
    {
        QApplication::quit();
    });
    connect(ui->hidebtn,&QPushButton::clicked,[this]
    {
        this->showMinimized();
    });
    connect(ui->maxbtn,&QPushButton::clicked,[this]
    {
        this->showMaximized();
        ui->maxbtn->hide();
        ui->minbtn->show();
    });
    connect(ui->minbtn,&QPushButton::clicked,[this]
    {
        this->showNormal();
        ui->maxbtn->show();
        ui->minbtn->hide();
    });
}

void MainWindow::createbtnmenu()
{
    QFont font("微软雅黑", 12, QFont::Normal, false);
    tcpTool=new QListWidgetItem(QString("TCP助手"),ui->listWidget);
    tcpTool->setTextAlignment(Qt::AlignCenter);
    tcpTool->setFont(font);
    serialTool=new QListWidgetItem(QString("串口助手"),ui->listWidget);
    serialTool->setTextAlignment(Qt::AlignCenter);
    serialTool->setFont(font);
    floatTool=new QListWidgetItem(QString("浮点数转换助手"),ui->listWidget);
    floatTool->setTextAlignment(Qt::AlignCenter);
    floatTool->setFont(font);
    qssTool=new QListWidgetItem(QString("重新加载QSS"),ui->listWidget);
    qssTool->setTextAlignment(Qt::AlignCenter);
    qssTool->setFont(font);
}

void MainWindow::registerClass()
{
    //qRegisterMetaType<serial>("serial");
}

void MainWindow::on_listWidget_clicked(const QModelIndex &)
{
    QListWidgetItem *list=ui->listWidget->currentItem();
    QString className=list->data(Qt::DisplayRole).toString();
    //qDebug()<<className;
    if(className=="重新加载QSS")
    {
        myHelper::qssopen(":/qss/mac.css");
        qDebug()<<"重新加载QSS";
        return;
    }
    if(Frm!=nullptr)
    {
        delete Frm;
        Frm=nullptr;
    }
    if(className=="串口助手")
        Frm = new serial();
    else if(className=="TCP助手")
        Frm=new tcp();
    else if(className=="浮点数转换助手")
        Frm=new FloatCvn();
    if(Frm!=nullptr)
    {
        ui->stackedWidget->addWidget(Frm);
        ui->stackedWidget->setCurrentWidget(Frm);
        Frm->setAttribute(Qt::WA_DeleteOnClose,true);
    }
}

//鼠标点击事件
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //读取坐鼠标点击坐标点
    lastpoint = event->globalPos();
}

//鼠标移动事件
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (this->isMaximized()) { return; }
    if(!lastpoint.isNull())
    {
        //把移动的点记录下来
        //int dx = event->globalX() - lastpoint.x();//这种也可以
        //int dy = event->globalY() - lastpoint.y();
        //move(x() + dx, y() + dy); //窗口移动到此处
        movepoint=event->globalPos()-lastpoint;
        lastpoint = event->globalPos(); //更新记录点
        move(pos()+movepoint);
    }
}
//鼠标释放事件
void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
    lastpoint=QPoint();
}
