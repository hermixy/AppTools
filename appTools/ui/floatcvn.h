#ifndef FLOATCVN_H
#define FLOATCVN_H

#include <QWidget>

namespace Ui {
class FloatCvn;
}

class FloatCvn : public QWidget
{
    Q_OBJECT

public:
    explicit FloatCvn(QWidget *parent = nullptr);
    ~FloatCvn();

private slots:
    void on_convHex_clicked();

    void on_convFlaot_clicked();

private:
    Ui::FloatCvn *ui;
};

#endif // FLOATCVN_H
