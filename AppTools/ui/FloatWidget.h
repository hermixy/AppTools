#ifndef FLOATWIDGET_H
#define FLOATWIDGET_H

#include <QWidget>

namespace Ui {
class FloatWidget;
}

class FloatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FloatWidget(QWidget *parent = nullptr);
    ~FloatWidget();

private slots:
    void on_convHex_clicked();

    void on_convFlaot_clicked();

private:
    Ui::FloatWidget *ui;
};

#endif // FLOATWIDGET_H
