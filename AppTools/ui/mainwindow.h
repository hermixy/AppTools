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
    void titleBtn();
    void createListWidgetBtnMenu();  

    void on_listWidget_clicked(const QModelIndex &);
    void createMenuMap(QString className);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);

private:
    Ui::MainWindow *ui;
    QSizeGrip *sizeGrip;
    //记录鼠标位置
    QPoint lastPoint;
    QPoint movePoint;

    QMap<QString,QWidget*> menuMap;

};
#endif // MAINWINDOW_H
