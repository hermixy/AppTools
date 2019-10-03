#include "ui/mainwindow.h"
#include "help/UiSet.h"

#include <QApplication>

void set()
{
    //QApplication::setWindowIcon(QIcon(QString("%1/image/setting-icon-dark.png")\
    //                                  .arg(qApp->applicationDirPath())));

    QApplication::setFont(QFont ("微软雅黑", 10, QFont::Normal, false));

    //UiSet::setQSS(QString("%1/qss/mac.css").arg(qApp->applicationDirPath()));
    //UiSet::setQSS("qss/mac.css");
    UiSet::setQSS();
}

int main(int argc, char *argv[])
{
    // 启用 Retina 高分辨率
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    set();

    MainWindow w;
    //设置窗口大小
    w.resize(1000,600);
    w.show();
    return a.exec();
}
