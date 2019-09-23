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

//#define AppName "TCP"
#define AppPath qApp->applicationDirPath()//返回包含应用程序可执行文件的目录
#define AppDeskWidth qApp->desktop()->availableGeometry().width()
#define AppDeskHeight qApp->desktop()->availableGeometry().height()

#ifndef MYHELPER_H
#define MYHELPER_H

class myHelper: public QObject
{
public:
    static void setUTF8Code()
    {
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    }

    //设置指定样式
    static void qssopen(QString qssfile)
    {
        QFile file(qssfile);
        if (file.open(QFile::ReadOnly))
        {
            QString qss = QLatin1String(file.readAll());
            if(qss.isEmpty())
                qDebug()<<qss;
            qApp->setStyleSheet(qss);
            file.close();
        }
        else
            qDebug()<<"qss error";
    }

    //延时
    static void sleep(int sec)
    {
        QTime dieTime = QTime::currentTime().addMSecs(sec);

        while (QTime::currentTime() < dieTime) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }

    //窗体居中显示
    static void InCenter(QWidget *frm)
    {
        int frmX = frm->width();
        int frmY = frm->height();
        QDesktopWidget w;
        int deskWidth = w.geometry().width();
        int deskHeight = w.geometry().height();
        QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
        frm->move(movePoint);
    }

    static void savedata(QTextBrowser *txt,QWidget *t)
    {
        QString tempData = txt->toPlainText();//以纯文本的形式返回文本编辑的文本。
        if (tempData == "")
        {
            append("没有数据可以保存！",txt);
            return;
        }
        QString path = QFileDialog::getSaveFileName(t,
                                                    tr("Open File"),
                                                    ".",
                                                    tr("Text Files(*.txt)"));
        if(!path.isEmpty())
        {
            QFile file(path);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox::warning(t,
                                     tr("Write File"),
                                     tr("Can't open file:\n%1").arg(path));
                return;
            }
            QTextStream out(&file);
            out << tempData;
            file.close();
            append("保存文件成功！",txt);
        }
        else
            append("你没有保存任何文件！",txt);
    }

    static void append(QString msg,QTextBrowser *txt)
    {
        QString str;
        str = ">> 提示信息 :";
        txt->setTextColor(QColor("red"));
        txt->append(QString("时间[%1] %2 %3").arg(DATETIME).arg(str).arg(msg));
    }
};

#endif // MYHELPER_H
