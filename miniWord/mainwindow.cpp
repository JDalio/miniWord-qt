#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
//#include <QStatusBar>
//#include <QLabel>
#include <QLayout>
#include <QScrollArea>
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->resize(QSize(550,600));
    //this->centralWidget()->resize(QSize(550,600));
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

    QVBoxLayout *pVlayout = new QVBoxLayout;

    QScrollArea *s = new QScrollArea;
    s->setGeometry(0, 0, this->width(), this->height());
    s->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    pLabel=new QLabel;
    pLabel->setStyleSheet("qproperty-alignment: 'AlignTop | AlignLeft'; font-size:20px;");
    str1="aaaahello<span style='background-color:black;color:white;margin:0;'>w</span>aaa1111111111111111111111111aa";
    str2="aaaahello<span style='background-color:white;color:black;margin:0;'>w</span>aaa1111111111111111111111111aa";
    pLabel->setText(str1);


    s->setWidget(pLabel);

    pVlayout->addWidget(s);
    pVlayout->setMargin(0);

    cenWid->setLayout(pVlayout);

    QTimer *timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(blink()));
    timer->start(400);



}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Left:
            x--;
            str1="aaaahell<span style='background-color:black;color:white;margin:0;'>o</span>waaa1111111111111111111111111aa";
            str2="aaaahell<span style='background-color:white;color:black;margin:0;'>o</span>waaa1111111111111111111111111aa";
            this->blink();
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

    //const char *ch = str.c_str();
    //QWidget::keyPressEvent(event);
    //qDebug() <<ch <<int(*ch);
    qDebug()<<x <<" " <<y;

}

void MainWindow::blink()
{
     //qDebug()<<"hello";
     cursorTimer ++;
     if(cursorTimer%2==0)
     {
         if(cursorTimer==100)
             cursorTimer=0;
         pLabel->setText(str1);
     }
     else
     {
         //str="aaaahello<span style='background-color:white;color:black;margin:0;'>w</span>aaa1111111111111111111111111aa";
         pLabel->setText(str2);
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
