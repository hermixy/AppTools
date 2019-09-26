#include "CrcWidget.h"
#include "ui_CrcWidget.h"

CrcWidget::CrcWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CrcWidget)
{
    ui->setupUi(this);
    QApplication::setFont(QFont ("微软雅黑", 10, QFont::Normal, false));
}

CrcWidget::~CrcWidget()
{
    delete ui;
}
