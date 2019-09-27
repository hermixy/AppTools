#include "UiSet.h"

void UiSet::setUTF8Code()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
}

void UiSet::setQSS(QString qssfile)
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

void UiSet::setWidgetPaddingAndSpacing(QWidget *widget, int padding, int spacing)
{
    // 设置 Widget 的 padding 和 spacing
    QLayout *layout = widget->layout();

    if (layout)
    {
        layout->setContentsMargins(padding, padding, padding, padding);
        layout->setSpacing(spacing);
    }
}

void UiSet::windowCenter(QWidget *window)
{
    // QSize size = qApp->desktop()->availableGeometry().size() - window->size();
    QSize size = qApp->primaryScreen()->availableSize() - window->size();
    int x = qMax(0, size.width() / 2);
    int y = qMax(0, size.height() / 2);
    window->move(x, y);
}

void UiSet::saveToFile(QTextBrowser *txt,QWidget *t)
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

void UiSet::append(QString msg,QTextBrowser *txt)
{
    QString str;
    str = ">> 提示信息 :";
    txt->setTextColor(QColor("red"));
    txt->append(QString("时间[%1] %2 %3").arg(DATETIME).arg(str).arg(msg));
}

void UiSet::sleep(int sec)
{
    QTime dieTime = QTime::currentTime().addMSecs(sec);

    while (QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}
