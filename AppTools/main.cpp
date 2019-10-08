#include "ui/mainwindow.h"
#include "help/UiSet.h"
#include "ui/LoginWidget.h"

#include <QApplication>

void set()
{
    //QApplication::setWindowIcon(QIcon(QString("%1/image/setting-icon-dark.png")\
    //                                  .arg(qApp->applicationDirPath())));

    QApplication::setFont(QFont ("微软雅黑", 10, QFont::Normal, false));

    //UiSet::setQSS(QString("%1/qss/mac.css").arg(qApp->applicationDirPath()));
    //UiSet::setQSS("qss/mac.css");
    UiSet::setQSS();

    // 今后访问相对路径的文件，则是相对于可执行文件所在目录了，而不是系统的根目录
    QDir::setCurrent(QCoreApplication::applicationDirPath());
}

int main(int argc, char *argv[])
{
    // 启用 Retina 高分辨率
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    set();

    //主窗口
    //MainWindow y;
    LoginWidget w;

    w.show();
    return a.exec();
}
