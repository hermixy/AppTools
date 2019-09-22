#include "crc.h"
#include "ui_crc.h"

Crc::Crc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Crc)
{
    ui->setupUi(this);
}

Crc::~Crc()
{
    delete ui;
}
