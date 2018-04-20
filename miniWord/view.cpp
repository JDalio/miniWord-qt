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
    header->h->a=new char[101];
    header->h->a[0]='\0';
    header->size=100;
    header->h->next=NULL;

    str1=new char[100];str2=new char[100];
    strcpy(str1,"<pre><span style='font-size:20px;font-weight:900;background-color:white;color:black;margin:0;'>|</sapn></pre>");
    strcpy(str2,"<pre><span style='font-size:20px;font-weight:900;background-color:white;color:white;margin:0;'>|</sapn></pre>");

}
//获取键盘捕获函数
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Left:
            rightBdry= x > rightBdry ? x : rightBdry;
            if(x>=0)
            {
                x--;
                print(x,y);
            }
            break;
        case Qt::Key_Right:
            if(x<rightBdry)
            {
                x++;
                print(x,y);
            }

            break;
        case Qt::Key_Up:
            y--;
            break;
        case Qt::Key_Down:
            y++;
            break;
        default:
            QString key=event->text();
            std::string str = key.toStdString();
            const char *ch = str.c_str();
            edit(*ch);
    }


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
         //label->re
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
