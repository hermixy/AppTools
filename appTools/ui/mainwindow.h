#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
#include <QMainWindow>
#include <QSizeGrip>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setPaddingAndSpacing();
    void initWindow();
    void setWidgetPaddingAndSpacing(QWidget *widget, int padding, int spacing);
    void titlebtn();
    void createbtnmenu();
    void registerClass();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);

    void on_listWidget_clicked(const QModelIndex &);

private:
    Ui::MainWindow *ui;

    QListWidgetItem *tcpTool;
    QListWidgetItem *serialTool;
    QListWidgetItem *floatTool;
    QListWidgetItem *qssTool;

    QWidget *Frm;
    QSizeGrip *sizeGrip;

    //记录鼠标位置
    QPoint lastpoint;
    QPoint movepoint;
};
#endif // MAINWINDOW_H
