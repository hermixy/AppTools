#include "CrcWidget.h"
#include "ui_CrcWidget.h"

CrcWidget::CrcWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CrcWidget)
{
    ui->setupUi(this);
}

CrcWidget::~CrcWidget()
{
    delete ui;
}
