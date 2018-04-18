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
    //qDebug()<<;
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

    header=new row;
    header->num=0;
    header->h=new sen;
    header->next=NULL;

    header->h->num=0;
    header->h->a=new char[100];
    header->h->a[0]='\0';
    header->size=100;
    header->h->next=NULL;

}
//打印数据结构
void MainWindow::print(int x, int y, int x1=-1, int y1=-1)
{

    if(x1=-1&&y1=-1)
    {
        //delete str1;delete str2;
        s1=new char[total+50];str1[0]='\0';
        s2=new char[total+50];str2[0]='\0';

        list currow=header;
        //打印位置那行之前的复制
        while(currow<y)
        {
            for(hsen cursen=currow->a;cursen->num<=x/100;cursen=cursen->next)
            {
                strcat(s1,cursen->a);
                s1[(cursen->num+1)*100]='\0';
            }
            currow=currow->next;
        }
        //打印位置行的复制

    }


}
//字符输入编辑数据结构然后更新屏幕字符串
void MainWindow::edit(int x, int y, char ch)
{
//qDebug() <<"123" <<ch <<"456";

/** (1)定位光标在数据结构中的位置 **/
    //row pointer postion
    list currow=header;
    while(currow->num!=x)
        currow=currow->next;

    //col pointer postion
    hsen cursen=currow->h;
    //char position
    int index;
    if(y!=0&&y%100==0)
    {
        while(cursen->next)
            cursen=cursen->next;
        cursen->a=new char[100];
        currow->size+=100;
        index=0;
    }
    else
    {
        int sennum=y/100;
        index=y%100;
        while(cursen->num!=sennum)
            cursen=cursen->next;
    }
/** end of 1 **/

/** 在相应的光标位置处操作 **/
    switch(ch)
    {
        case '\b':
            qDebug() <<"backspace";
        break;

        case '\r':
            qDebug() <<"enter";
        break;

        default:
            total++;
            qDebug()<<ch;
            if(index!=99)
                for(int i=strlen(cursen);i>index;i--)
                    cursen[i]=cursen[i-1];

            cursen[index]=ch;
            cursen[index+1]='\0';
    }

    x++;y++;
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
