#ifndef CRCWIDGET_H
#define CRCWIDGET_H

#include <QWidget>

namespace Ui {
class CrcWidget;
}

class CrcWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CrcWidget(QWidget *parent = nullptr);
    ~CrcWidget();

private slots:
    void on_calculate_clicked();

private:
    Ui::CrcWidget *ui;
};

#endif // CRCWIDGET_H
