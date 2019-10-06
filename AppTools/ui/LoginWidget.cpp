#include "LoginWidget.h"
#include "ui_LoginWidget.h"
#include "../help/UiSet.h"
#include "mainwindow.h"

#pragma execution_character_set("utf-8")

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);   //去掉边框
    setAttribute(Qt::WA_StyledBackground);
    UiSet::windowCenter(this);

    ui->picture->setText("");

    ui->jusername->setPlaceholderText("用户名：admin");
    ui->kpassword->setPlaceholderText("密码：admin");
    ui->kpassword->setEchoMode(QLineEdit::Password);

    UiSet::setWidgetPaddingAndSpacing(this,5,20);
    UiSet::setWidgetPaddingAndSpacing(ui->titlewidget,0,10);

    connect(ui->closebtn,&QPushButton::clicked,[]
    {
        QApplication::quit();
    });
    connect(ui->hidebtn,&QPushButton::clicked,[this]
    {
        this->showMinimized();
    });

    ui->loginbtn->setShortcut(Qt::Key_Enter);   //登录快捷键回车

    // 输入密码后按下回车进行登陆
    connect(ui->kpassword, &QLineEdit::returnPressed, [this] {
        ui->loginbtn->click();
    });
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::on_loginbtn_clicked()
{
    QString username=ui->jusername->text().trimmed();
    QString password=ui->kpassword->text().trimmed();
    if(username.isEmpty()||password.isEmpty())
    {
        QMessageBox::warning(this, "警告", "用户名不存在或者密码为空！",QMessageBox::Ok);
        return;
    }
    if("admin"==username&&"admin"==password)
    {
        MainWindow *mainw=new MainWindow();
        this->hide();
        mainw->show();
        this->close();
    }
}

//鼠标点击事件
void LoginWidget::mousePressEvent(QMouseEvent *event)
{
    //读取坐鼠标点击坐标点
    lastPoint = event->globalPos();
}

//鼠标移动事件
void LoginWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (this->isMaximized()) { return; }
    if(!lastPoint.isNull())
    {
        //把移动的点记录下来
        //int dx = event->globalX() - lastpoint.x();//这种也可以
        //int dy = event->globalY() - lastpoint.y();
        //move(x() + dx, y() + dy); //窗口移动到此处
        movePoint=event->globalPos()-lastPoint;
        lastPoint = event->globalPos(); //更新记录点
        move(pos()+movePoint);
    }
}
//鼠标释放事件
void LoginWidget::mouseReleaseEvent(QMouseEvent *)
{
    lastPoint=QPoint();
}
