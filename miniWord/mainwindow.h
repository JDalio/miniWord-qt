#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QLayout>
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>
#include <QStatusBar>
#include <QFileDialog>
#include <QString>
#include <QByteArray>
#include <QCloseEvent>
#include <QLineEdit>//仅用于查找输入框
#include <QDialog>
#include <QPushButton>

#include "clipboard.h"


//窗口的定义，其中核心数据结构放在了窗口的私有成员中
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
/**
  整个程序采用一种"面对事件编程"的思路
  用户产生的鼠标键盘输入是改变输入状态的唯一因素
  当捕获到用户的输入后，会改变后端的数据结构，处理数据结构，将数据结构中的新内容通过一定格式，赋值给str1和str2
  前端的显示函数会定时(400ms)的在屏幕上显示，用两个str主要是为了做出光标效果
 **/
protected:
    //键盘事件的捕获
    void keyPressEvent(QKeyEvent *event);
    /** 数据结构的基本操作函数*****by 晨昊 and 冠华 and Dalio **/
    //根据行号，列号(从零开始编号)来修改内存中的数据结构，具体要求在mainWindow.cpp中
    void edit(char ch);
    //命令模式
    void exe(char ch);
    //点击关闭窗口按钮
    void closeEvent(QCloseEvent *event);
    //KMP算法
    htar find(char *target, htar head);
    void getnext(char *p,int next[],int len);
    void showPos();
    /** 数据结构的基本操作函数完 **/
private:
    /** 编程主要用的后端的，及后端与前端公用的数据 !!!只可增加不可删除**/
    //核心的数据结构，每一行的堆就是malloc/new出来的数组
    list header;
    //整个数据结构的有效字符的数目
    int total=0;
    //当前窗口内的内容是否完整保存
    bool isSaved=true;
    //后端将数据结构中的数据处理成一个完整的html传给前端的字符串，需要加粗的位置用span标签包裹
    char *str1 = NULL, *str2 = NULL;

    //光标的坐标 x对应行号，y对应列号,ox,oy剪切粘贴用
    int x = 0, y = 0,ox=-1,oy=-1;

    //右边界和下边界
    int rightBdry=0,LowerBdry=0;

    //打印数据结构
    void print(int x, int y, int x1 = -1, int y1 = -1);
    //status bar 的提示信息，用于命令模式
    void hint(const char *hint);
    //文件的基本操作 *****by Dalio
    //文件名
    std::string filename = "";

    void open();

    void save();

    void quit();

    //查找与替换    *****by Dalio
    QString str="";
    int findpos=-1;
    htar head=NULL;

    bool prepos=-2;
    QDialog *findDlg=NULL;
    QDialog *replaceDlg=NULL;
    QLineEdit *findLineEdit=NULL;
    QLineEdit *findLineEdit1=NULL;
    QLineEdit *replaceLineEdit=NULL;

    void createFindDlg();

    void createReplaceDlg();
    /** 后端的，及后端与前端公用的数据 完**/
    //前端文本编辑的显示区域
    QLabel *pLabel;
    //滚动栏
    QScrollArea *s;
    //剪贴板
    board clipboard;
    //光标闪烁的计数器
    qint8 cursorTimer = 0;

    //菜单栏
    QAction *newAction, *openAction;
    QAction *saveAction, *quitAction;
    QAction *findAction, *replaceAction;
    //status bar
    QStatusBar *statusbar;
    bool order_mod=false;

private
    slots:
        void showpre();
        void shownext();
        void replace();
        void replaceall();
        void blink();
};

#endif // MAINWINDOW_H
