#include "ui/mainwindow.h"
#include "helper/myHelper.h"
//#include "ui/floatcvn.h"

#include <QApplication>

void set()
{
    //QApplication::setWindowIcon(QIcon(QString("%1/image/setting-icon-dark.png")\
    //                                  .arg(qApp->applicationDirPath())));

    QApplication::setFont(QFont ("微软雅黑", 10, QFont::Normal, false));

    myHelper::qssopen(QString("%1/qss/mac.css").arg(qApp->applicationDirPath()));
}

int main(int argc, char *argv[])
{
    // 启用 Retina 高分辨率
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    set();

    MainWindow w;
    //FloatCvn w;
    w.show();
    return a.exec();
}
