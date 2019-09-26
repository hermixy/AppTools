#include "CustomWidget.h"
#include "ui_CustomWidget.h"

CustomWidget::CustomWidget(QString s,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomWidget)
{
    ui->setupUi(this);
    QApplication::setFont(QFont ("微软雅黑", 10, QFont::Normal, false));
    ui->label->setFont(QFont("微软雅黑", 28, QFont::Bold, false));
    //ui->label->setStyleSheet("color:blue;");
    ui->label->setAlignment(Qt::AlignCenter);
    //ui->label->setWordWrap(true);
    ui->label->setText(s);

}

CustomWidget::~CustomWidget()
{
    delete ui;
}
