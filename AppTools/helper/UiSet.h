#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#pragma execution_character_set("utf-8")

#define TIMEMS qPrintable (QTime::currentTime().toString("HH:mm:ss zzz"))
#define TIME qPrintable (QTime::currentTime().toString("HH:mm:ss"))
#define QDATE qPrintable (QDate::currentDate().toString("yyyy-MM-dd"))
#define QTIME qPrintable (QTime::currentTime().toString("HH-mm-ss"))
#define DATETIME qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
#define STRDATETIME qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss"))
#define STRDATETIMEMS qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss-zzz"))

#define AppPath qApp->applicationDirPath()//返回包含应用程序可执行文件的目录

#ifndef UISET_H
#define UISET_H

class UiSet: public QObject
{
public:
    //设置utf8编码
    static void setUTF8Code();

    //设置QSS
    static void setQSS(QString qssfile);

    // 设置 widget 的 padding 和 spacing
    static void setWidgetPaddingAndSpacing(QWidget *widget, int padding, int spacing);

    //延时
    static void sleep(int sec);

    //窗体居中显示
    static void windowCenter(QWidget *window);

    //保存txt到本地
    static void saveToFile(QTextBrowser *txt,QWidget *t);

    //显示消息
    static void append(QString msg,QTextBrowser *txt);
};

#endif // UISET_H
