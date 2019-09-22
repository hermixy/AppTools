#ifndef CRC_H
#define CRC_H

#include <QWidget>

namespace Ui {
class Crc;
}

class Crc : public QWidget
{
    Q_OBJECT

public:
    explicit Crc(QWidget *parent = nullptr);
    ~Crc();

private:
    Ui::Crc *ui;
};

#endif // CRC_H
