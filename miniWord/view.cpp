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
    header->a=new char[101];
    header->a[0]='\0';
    header->size=0;
    header->total=100;
    header->next=NULL;

    str1=new char[100];str2=new char[100];
    strcpy(str1,"<pre><span style='font-size:20px;font-weight:900;background-color:white;color:black;margin:0;'>|</sapn></pre>");
    strcpy(str2,"<pre><span style='font-size:20px;font-weight:900;background-color:white;color:white;margin:0;'>|</sapn></pre>");

}
//打印数据结构,修改str1,str2
void MainWindow::print(int x, int y, int x1, int y1)
{
    qDebug() <<"Print:" <<"x: " <<x <<"y: " <<y;

    if(x1==-1&&y1==-1)
    {
        //delete str1;delete str2;
        char *s1=new char[total+300];strcpy(s1,"<pre style='font-weight:500;font-size:16px;'>\n");
        char *s2=new char[total+300];strcpy(s2,"<pre style='font-weight:500;font-size:16px;'>\n");

        list currow=header;
        //打印位置那行之前的复制，每行末无换行符，遍历过程中append
        while(currow->num<y)
        {
            strcat(s1,currow->a);
            strcat(s1,"\n");
            currow=currow->next;
        }
        //打印位置行的复制,位置之前的复制
        strncat(s1,currow->a,x);

        strcpy(s2,s1);
        //加入光标
        strcat(s1,"<span style='font-size:20px;font-weight:900;background-color:white;color:black;margin:0;'>|</sapn><span style='font-weight:500;font-size:16px;'>");
        strcat(s2,"<sapn style='font-size:20px;font-weight:900;background-color:white;color:white;margin:0;'>|</span><span style='color:black;font-weight:500;font-size:16px;'>");

        //append the latter part which may less than 100
        char *latter=&currow->a[x];
        strcat(s1,latter);
        strcat(s2,latter);
        //光标行之后的复制
        if(currow->next)
        {
            strcat(s1,"\n");
            strcat(s2,"\n");
        }
        currow=currow->next;
        while(currow)
        {
            strcat(s1,currow->a);
            strcat(s1,"\n");
            strcat(s2,currow->a);
            strcat(s2,"\n");
            currow=currow->next;
        }
        //格式化str1和str2，会自动在blink()中显示
        strcat(s1,"</span></pre>");
        strcat(s2,"</span></pre>");
        char *tmp1=str1;
        char *tmp2=str2;
        str1=s1;
        str2=s2;
//        qDebug()<<str1 <<endl <<str2;
        if(str1&&str2)
        {
            delete tmp1;
            delete tmp2;
        }
    }

}
//获取键盘捕获函数
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    list tmp=header;
    switch(event->key())
    {
        case Qt::Key_Left:
            if(x>0)
            {
                x--;
                print(x,y);
            }
            break;
        case Qt::Key_Right:
            while(tmp->num!=y)
                tmp=tmp->next;
            if(x<tmp->size)
            {
                x++;
                print(x,y);
            }
            break;
        case Qt::Key_Up:
            if(y>0)
                y--;
            while(tmp->num!=y)
                tmp=tmp->next;
            if(x>tmp->size)
                x=tmp->size;
            print(x,y);
            break;
        case Qt::Key_Down:
            y++;
            while(tmp&&tmp->num!=y)
                tmp=tmp->next;
            if(tmp&&x>tmp->size)
                x=tmp->size;
            if(!tmp)
               y--;
            print(x,y);
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
         //qDebug()<<"blink1";
     }
     else
     {
         QString s2=QString(QLatin1String(str2));
         delete s->widget();
         QLabel *label=new QLabel();
         label->setText(s2);
         s->setWidget(label);
         //qDebug()<<"blink2";
     }
}
