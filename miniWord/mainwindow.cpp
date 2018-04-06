#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QLayout>
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>

#include <string.h>

#include "mainwindow.h"
//主窗口的构造函数
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
/** init the main window  **/
 {
    this->resize(QSize(550,600));
    setWindowTitle(tr("MiniWord"));


//File item in menu

    newAction = new QAction(tr("New"),this);
    newAction->setShortcuts(QKeySequence::New);
    connect(newAction,&QAction::triggered,this,&MainWindow::create);

    openAction = new QAction(tr("&Open"), this);
    openAction->setShortcuts(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::open);

    saveAction = new QAction(tr("Save"),this);
    saveAction->setShortcuts(QKeySequence::Save);
    connect(saveAction,&QAction::triggered,this,&MainWindow::save);

    quitAction = new QAction(tr("Quit"),this);
    quitAction->setShortcuts(QKeySequence::Quit);
    connect(quitAction,&QAction::triggered,this,&MainWindow::quit);

//Edit item in menu
    findAction = new QAction(tr("Find"),this);
    findAction->setShortcuts(QKeySequence::Find);
    connect(findAction,&QAction::triggered,this,&MainWindow::find);

    replaceAction = new QAction(tr("Replace"),this);
    replaceAction->setShortcuts(QKeySequence::Replace);
    connect(quitAction,&QAction::triggered,this,&MainWindow::replace);

    QMenu *file = menuBar()->addMenu(tr("&File"));
    file->addAction(newAction);
    file->addAction(openAction);
    file->addAction(saveAction);
    file->addAction(quitAction);


    QMenu *edit=menuBar()->addMenu(tr("&Edit"));
    edit->addAction(findAction);
    edit->addAction(replaceAction);

//Text edit area

    QWidget *cenWid = new QWidget;
    this->setCentralWidget(cenWid);
    cenWid->setStyleSheet("background-color: white;color: black;");
    cenWid->setFocusPolicy(Qt::StrongFocus);
    cenWid->grabKeyboard();
    QVBoxLayout *pVlayout = new QVBoxLayout;

    s = new QScrollArea;
    s->setGeometry(0, 0, this->width(), this->height());
    s->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    pLabel=new QLabel;
    pLabel->setStyleSheet("qproperty-alignment: 'AlignTop | AlignLeft'; font-size:20px;");

    /*str1=new char[2];
    str2=new char[2];
    str1[0]=' ';
    str1[1]='\0';
    str2[0]=' ';
    str2[1]='\0';*/
    QString s1=QString(QLatin1String(str1));
    pLabel->setText(s1);


    s->setWidget(pLabel);
    pVlayout->addWidget(s);
    pVlayout->setMargin(0);
    cenWid->setLayout(pVlayout);
//The blink timer
    QTimer *timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(blink()));
    timer->start(400);
 }
/** init the main datastructure **/
    header=NULL;


}

//方向键刷新屏幕区
void MainWindow::refresh(int row, int col)
{

}
//字符输入编辑数据结构然后更新屏幕字符串
void MainWindow::edit(int x, int y, char ch)
{
    //1、处理数据结构
    if(header==NULL)
    {
        header=new row;
        header->num=0;
        header->h=new sen;
        header->next=NULL;

        cursen=header->h;
        cursen->a=new char[100];
        size+=100;
        cursen->next=NULL;
        cursen->num=0;

        /*switch(ch)
        {

        }*/

    }
    cursen->a[x]=ch;
    cursen->a[x+1]='\0';
    //2、将数据结构中的内容整合成一个满足要求的字符串:
    //坐标位置处的字符用两种不同的标签包裹
    //<span style='background-color:black;color:white;margin:0;'></span>
    //<span style='background-color:white;color:black;margin:0;'></span>
    //最后更新str1和str2，blink()函数会自动显示

    char tmp[2]="\0";
    tmp[0]=ch;
    if(str==NULL)
    {

        str1=new char[size+50];
        strcpy(str1,"<span style='background-color:black;color:white;margin:0;'>");
        strcat(str1,tmp);
        strcat(str1,"</span>");

        str2=new char[size+50];
        strcpy(str2,"<span style='background-color:white;color:black;margin:0;'>");
        strcat(str2,tmp);
        strcat(str2,"</span>");

        str=new char[size];
        str[index++]=ch;
        str[index]='\0';
    }
    else
    {
        strcpy(str1,"<pre>");
        strcat(str1,str);
        strcat(str1,"<span style='background-color:black;color:white;margin:0;'>");
        strcat(str1,tmp);
        strcat(str1,"</span>");
        strcat(str1,"</pre>");

        strcpy(str2,"<pre>");
        strcat(str2,str);
        strcat(str2,"<span style='background-color:white;color:black;margin:0;'>");
        strcat(str2,tmp);
        strcat(str2,"</span>");
        strcat(str2,"</pre>");

        str[index++]=ch;
        str[index]='\0';

    }
    //qDebug() <<str1;
    //qDebug() <<str2;



}
//获取键盘捕获函数
void MainWindow::keyPressEvent(QKeyEvent *event)
{

    switch(event->key())
    {
        case Qt::Key_Left:
            x--;
            qDebug() <<x;
            break;
        case Qt::Key_Right:
            x++;
            break;
        case Qt::Key_Up:
            y++;
            break;
        case Qt::Key_Down:
            y--;
            break;
    }

    QString key=event->text();
    std::string str = key.toStdString();
    const char *ch = str.c_str();
    edit(x,y,*ch);
}

void MainWindow::blink()
{
     cursorTimer ++;
     if(cursorTimer%2==0)
     {
         if(cursorTimer==100)
             cursorTimer=0;
         QString s1=QString(QLatin1String(str1));
         delete s->widget();
         QLabel *label=new QLabel();
         label->setText(s1);
         s->setWidget(label);
     }
     else
     {
         QString s2=QString(QLatin1String(str2));
         delete s->widget();
         QLabel *label=new QLabel();
         label->setText(s2);
         s->setWidget(label);

     }
}

void MainWindow::open()
{
    QMessageBox::information(this, tr("Information"), tr("Open"));
}
void MainWindow::create()
{
    QMessageBox::information(this,tr("info"),tr("New"));
}
void MainWindow::save()
{

}
void MainWindow::quit()
{

}
void MainWindow::find()
{

}
void MainWindow::replace()
{

}

MainWindow::~MainWindow()
{

}
