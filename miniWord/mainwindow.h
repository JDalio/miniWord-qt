#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

typedef struct row{
    int num;//行号
    char *col;//列数组的首地址
    row *next;
}row,*list;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    //数据结构改变后刷新窗口编辑区
    void update(int row,int col,char ch);
    void refresh(char *str1,char *str2);
    //键盘事件的捕获
    void keyPressEvent(QKeyEvent *event);
    //数据结构的基本操作函数
private:
    //核心的数据结构
    list header;

    //后端将数据结构中的数据处理成一个完整的html传给前端的字符串，需要加粗的位置用span标签包裹
    char *str1,*str2;

    //前端文本编辑的显示区域
    QLabel *pLabel;

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
