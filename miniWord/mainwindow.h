#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);
private:
    //前端文本编辑的显示区域
    QLabel *pLabel;

    //后端将数据结构中的数据处理成一个完整的html传给前端，需要加粗的位置用span标签包裹
    QString str1,str2;

    //光标闪烁的计数器
    qint8 cursorTimer=0;
    //光标的坐标
    int x=0,y=0;

    //菜单栏
    QAction *newAction,*openAction;
    QAction *saveAction,*quitAction;
    QAction *findAction,*replaceAction;

    void create();
    void open();
    void save();
    void quit();

    void find();
    void replace();
private slots:
    void blink();
};

#endif // MAINWINDOW_H
