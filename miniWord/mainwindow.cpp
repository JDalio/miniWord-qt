#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QLayout>
#include <QScrollArea>
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

/** init the main window  **/

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

    QScrollArea *s = new QScrollArea;
    s->setGeometry(0, 0, this->width(), this->height());
    s->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    pLabel=new QLabel;
    pLabel->setStyleSheet("qproperty-alignment: 'AlignTop | AlignLeft'; font-size:20px;");
    str1="";
    str2="";
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
/** init the main datastructure **/

}


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
    update(x,y,*ch);

    qDebug() <<ch <<int(*ch);

}
void MainWindow::update(int row, int col, char ch)
{
    //1、处理数据结构

    //2、将数据结构中的内容整合成一个满足要求的字符串:
    //坐标位置处的字符用两种不同的标签包裹

    //3、调用mainWindow的接口，在屏幕上打印新的字符串
    //refresh(str1,str2);
}

void MainWindow::refresh(char *str1, char *str2)
{
    //刷新
}


void MainWindow::blink()
{
     cursorTimer ++;
     if(cursorTimer%2==0)
     {
         if(cursorTimer==100)
             cursorTimer=0;
         QString s1=QString(QLatin1String(str1));
         pLabel->setText(s1);
     }
     else
     {
         QString s2=QString(QLatin1String(str2));
         pLabel->setText(s2);
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
