#include <mainwindow.h>
#include <QCoreApplication>
#include <fstream>
#include <string>
//字符输入编辑数据结构然后更新屏幕字符串,x是行，y是列
void MainWindow::edit(char ch)
{
/** 1、定位光标在数据结构中的位置 **/
    //row pointer postion
    list currow=header;
    while(currow->num!=y)
        currow=currow->next;
    isSaved=false;
/** 在相应的光标位置处操作 **/
    switch(ch)
    {
        case '\b':
        /** 分两种情况 x=0，删除当前行，x!=0删除行中的字符 **/{
            if(x==0)
            {
                if(y==0)
                {                   
                    if(header->next)
                    {
                        list tmp=header;
                        header=header->next;
                        total-=tmp->size;
                        delete tmp->a;
                        delete tmp;
                    }
                    else
                    {
                        currow->a=(char*)realloc(currow->a,101*sizeof(char));
                        currow->a[0]='\0';
                        header->size=0;
                        header->total=100;
                        total=0;
                    }

                }
                else
                {
                    total-=currow->size;
                    list pre=header;
                    while(pre->next!=currow)
                        pre=pre->next;
                    pre->next=currow->next;
                    delete currow->a;
                    delete currow;
                }
                list tmp=header;
                int i;
                for(i=0;tmp;tmp=tmp->next,i++)
                    tmp->num=i;
                if (y == i && y != 0)
                    y--;
            }
            else
            {
                x--;
                for(int i=x;i<currow->size;i++)
                    currow->a[i]=currow->a[i+1];
                int block_size=(x/100+1)*100;
                if(block_size!=currow->total)
                {
                    currow->total=block_size;
                    currow->a=(char*)realloc(currow->a,(currow->total+1)*sizeof(char));
                }
                total--;
                if(currow->size>0)
                    currow->size--;
            }
            print(x,y);                                                }
        break;
        case '\r':
            qDebug() <<"enter:" <<"x: " <<x <<"y: " <<y;
        /** 分三种情况 行首回车，行中回车，行末回车 **/{
            list next=currow->next,pre=currow;
            currow->next=new row;
            currow->next->num=currow->num+1;
            currow=currow->next;
            currow->next=next;

            while(next)
            {
                next->num++;
                next=next->next;
            }
            if(x==pre->size)
            {
                currow->a=new char[101];
                currow->a[0]='\0';
                currow->size=0;
                currow->total=100;
                x=0;
                y++;

            }
            else if(x==0)
            {
                currow->a=pre->a;
                currow->size=pre->size;
                currow->total=pre->total;
                pre->a=new char[101];
                pre->a[0]='\0';
                pre->size=0;
                pre->total=100;
                y++;
            }
            else
            {
                char *tmp=pre->a;
                currow->size=pre->total-pre->size;
                currow->total=(currow->size/100+1)*100;
                currow->a=new char[currow->total+1];
                currow->a[0]='\0';

                pre->total=(x/100+1)*100;
                pre->a=new char[pre->total+1];
                pre->a[0]='\0';
                pre->size=x;

                memcpy(pre->a,tmp,x);
                memcpy(currow->a,tmp+x,strlen(tmp)-x);
                delete tmp;
                x=0;y++;

            }
            print(x,y);
                                                    }
        break;
        default:
        /** 当输入字母和数字的时候 **/{
            if(currow->size==currow->total)
            {
                currow->total+=100;
                currow->a=(char*)realloc(currow->a,(currow->total+1)*sizeof(char));
            }
            for(int i=currow->size+1;i>x;i--)
                currow->a[i]=currow->a[i-1];
            currow->a[x]=ch;
            currow->size++;
            x++;
            total++;
            print(x,y);
            /** 输入字母和数字的情况结束 **/}
    }

}
void MainWindow::exe(char order)
{
    switch(order)
    {
        case 'd':
        {
            isSaved=false;
            clipboard.copy(header, x, y, ox, oy, true, total);
            print(x, y, ox, oy);
            break;
        }
        case 'p':
        {
            isSaved=false;
            total += clipboard.paste(header, x, y, ox, oy);
            print(x, y, ox, oy);
            break;
        }
        case 'c':
        {
            clipboard.copy(header, x, y, ox, oy, false, total);
            print(x, y, ox, oy);
            break;
        }
    }
}

void MainWindow::open()
{
    QFileDialog *fileDialog = new QFileDialog();
    fileDialog->setWindowTitle(tr("打开文件"));
    fileDialog->setDirectory(".");
    fileDialog->setViewMode(QFileDialog::Detail);

    QString path = fileDialog->getOpenFileName();
    filename=std::string((const char *)path.toLocal8Bit());

    //将文件内容读入结构体
    std::ifstream fin;
    fin.open(filename, std::ios::in);
    if (!fin.is_open())
        QMessageBox::information(this, tr("打开文件"), tr("打开失败"));
    else
    {
        std::string linetmp;
        list currow = header;
        int rownum = 0;
        while (std::getline(fin, linetmp))
        {
            if (rownum != 0)
            {
                currow->next = new row;
                currow = currow->next;
                currow->num = rownum++;
                currow->next = NULL;
            }
            if (currow == header)
                rownum++;
            total += linetmp.length();
            currow->size = linetmp.length();
            currow->total = (currow->size / 100 + 1) * 100;
            currow->a = new char[currow->total];
            strcpy(currow->a, linetmp.data());
        }
    }
    print(0, 0, -1, -1);
}

void MainWindow::save()
{
    if (filename=="")
    {
        QString path=QDir::currentPath();
        QString name = QFileDialog::getSaveFileName(this, tr("保存文件"),path,tr("Text Files (*.txt)"));
        if(!name.isNull())
            filename=name.toStdString();
        else
        {
            isSaved=false;
            return;
        }
    }
    isSaved=true;
    std::ofstream fout;
    fout.open(filename);
    list currow = header;
    while (currow)
    {
        fout << currow->a;
        currow = currow->next;
        if (currow)
            fout << "\n";
    }
    fout.close();

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(total !=0 && !isSaved)
    {
          switch( QMessageBox::information(this,tr("提示"),tr("是否保存文件?"),tr("确定"), tr("取消"),0,1))
          {
              case 0:
                  save();
                  break;
              case 1:
              default:
                if(filename!="")
                  event->ignore();
          }
    }

}

void MainWindow::quit()
{
    this->close();
}

void MainWindow::getnext(char *p,int next[],int len)
{
    next[0] = -1;
    int k = -1;
    int j = 0;
    while (j < len - 1)
    {
        if (k == -1 || p[j] == p[k])
        {
            ++k;
            ++j;
            if (p[j] != p[k])
                next[j] = k;
            else
                next[j] = next[k];
        }
        else k = next[k];
    }
}
void MainWindow::showpre()
{
    if(findpos>0)
    {
        findpos--;
    }
    showPos();
}
void MainWindow::shownext()
{
    findpos++;
    showPos();
}

void MainWindow::showPos()
{
    if(str=="")
    {
        if(findLineEdit1)
            str=findLineEdit1->text();
        else
            str=findLineEdit->text();
        if(str.length()==0)
        {
            QMessageBox::information(this, tr("查找文件"), tr("请输入待查字符串!"));
        }
        QByteArray ba = str.toLatin1();
        char *target=ba.data();

        int len = strlen(target);
        int next[len];
        getnext(target,next,len);
        htar curtar = head;
        list currow = header;
        //进入遍历寻找字符串
        int sen_num = 0;
        int i = 0;
        int j = 0;
        for(; currow != NULL; currow = currow->next,sen_num++)
        {
            //在一个块中，没有匹配完成/没有遇到空字符时
            i=0;
            while(i<currow->size)
            {
                j = 0;
                while(currow->a[i] != '\0' && j < len)
                {
                    if(j == -1 || target[j] == currow->a[i])
                    {
                        i++;
                        j++;
                    }
                    else
                        j = next[j];

                    //如果完成一次匹配，则重置j
                    if(j == len)
                    {
                        if(curtar==NULL)
                        {
                            curtar=new pos_target;
                            head=curtar;
                        }
                        else
                        {
                            curtar->next = new pos_target;
                            curtar = curtar->next;
                        }
                        curtar->next=NULL;
                        curtar->n_row = sen_num;//记录本次查找到的字符串所在行地址
                        curtar->n_word = i - len;//记录字符串开头在本行中
                    }
                }
            }
        }
    }

    //打印位置
    if(head)
    {
        htar tmp=head;
        for(int i=0;i<findpos;i++)
        {
            if(tmp->next)
            {
                tmp=tmp->next;
            }
            else
            {
                if(findDlg)
                {
                    QMessageBox::information(this, tr("查找文件"), tr("查找完毕!"));
                    findDlg->close();
                }
                str="";
                findpos=-1;
                tmp=head;
                while(head)
                {
                    head=head->next;
                    delete tmp;
                    tmp=head;
                }
                tmp=NULL;
                break;
            }
        }
        if(tmp)
        {
            x=tmp->n_word;
            y=tmp->n_row;
            print(tmp->n_word,tmp->n_row,-1,-1);
        }

    }
    else
    {
        if(!replaceDlg)
        {
            QMessageBox::information(this, tr("查找文件"), tr("文件中没有待查字符串!"));
        }
        else
        {
            QMessageBox::information(this, tr("替换文件"), tr("替换完毕!"));
            findpos=-1;
            prepos=-2;
            str="";
            htar tmp=head;
            while(head)
            {
                head=head->next;
                delete tmp;
                tmp=head;
            }
            replaceDlg->close();
            replaceDlg=NULL;
            findLineEdit1=NULL;
            replaceLineEdit=NULL;

        }
        if(findDlg)
            findDlg->close();
        findpos=-1;
        str="";
    }

}
void MainWindow::createFindDlg()
{
//    创建查找窗口
    findDlg = new QDialog(this);
    findDlg->setWindowTitle(tr("查找"));
    findLineEdit = new QLineEdit(findDlg);

    QPushButton *nextbtn= new QPushButton(tr("下一个"), findDlg);
    QPushButton *prebtn=new QPushButton(tr("上一个"), findDlg);

    QVBoxLayout *layout= new QVBoxLayout(findDlg);
    layout->addWidget(findLineEdit);
    layout->addWidget(prebtn);
    layout->addWidget(nextbtn);
    findDlg->show();

    connect(prebtn, SIGNAL(clicked()), this, SLOT(showpre()));
    connect(nextbtn, SIGNAL(clicked()), this, SLOT(shownext()));
}

void MainWindow::replace()
{
    if(findLineEdit1->text()==""||replaceLineEdit->text()=="")
    {
        QMessageBox::information(this, tr("替换文件"), tr("请检查输入!"));
    }
    else
    {
        if(head)
        {
            htar tmp=head;
            for(int i=0;i<findpos;i++)
                if(tmp->next)
                    tmp=tmp->next;
                if(prepos!=findpos)
                {
                    QString rs=replaceLineEdit->text(),fs=findLineEdit1->text();
                    std::string rss = rs.toStdString(),fss=fs.toStdString();
                    const char *rstr = rss.c_str(),*fstr=fss.c_str();
                    list currow=header;
                    while(currow->num!=tmp->n_row)
                        currow=currow->next;

                    currow->size+=(strlen(rstr)-strlen(fstr));
                    currow->total=(currow->size/100+1)*100;
                    char *tmprowa=currow->a;
                    currow->a=new char[currow->total];
                    currow->a[0]='\0';
                    strncat(currow->a,tmprowa,tmp->n_word);
                    strcat(currow->a,rstr);
                    strcat(currow->a,&tmprowa[tmp->n_word+strlen(fstr)]);
                    delete tmprowa;
                    print(tmp->n_word,tmp->n_row);
                    findpos=-1;
                    prepos=-2;
                    str="";
                    htar tmp=head;
                    while(head)
                    {
                        head=head->next;
                        delete tmp;
                        tmp=head;
                    }
                }
        }
        else
        {
            QMessageBox::information(this, tr("替换文件"), tr("文件中或者当前位置没有待替换字符串!"));
            findpos=-1;
            prepos=-2;
            str="";
            htar tmp=head;
            while(head)
            {
                head=head->next;
                delete tmp;
                tmp=head;
            }
            replaceDlg->close();
            replaceDlg=NULL;
            findLineEdit1=NULL;
            replaceLineEdit=NULL;
        }

    }

}
void MainWindow::replaceall()
{
    if(findLineEdit1->text()==""||replaceLineEdit->text()=="")
    {
        QMessageBox::information(this, tr("替换文件"), tr("请检查输入!"));
    }
    else
    {
        showPos();
        if(!head)
        {
            QMessageBox::information(this, tr("替换文件"), tr("没有匹配的字符串!"));
        }
        else
        {
            htar tmp=head;
            QString rs=replaceLineEdit->text(),fs=findLineEdit1->text();
            std::string rss = rs.toStdString(),fss=fs.toStdString();
            const char *rstr = rss.c_str(),*fstr=fss.c_str();
            while(tmp)
            {
                list currow=header;
                while(currow->num!=tmp->n_row)
                {
                    currow=currow->next;
                }
                currow->size+=(strlen(rstr)-strlen(fstr));
                currow->total=(currow->size/100+1)*100;
                char *tmprowa=currow->a;
                currow->a=new char[currow->total];
                currow->a[0]='\0';
                strncat(currow->a,tmprowa,tmp->n_word);
                strcat(currow->a,rstr);
                strcat(currow->a,&tmprowa[tmp->n_word+strlen(fstr)]);
                tmp=tmp->next;
                delete tmprowa;
            }
            x=0;y=0;
            print(0,0,-1,-1);
            findpos=-1;
            str="";
            tmp=head;
            while(head)
            {
                head=head->next;
                delete tmp;
                tmp=head;
            }
            replaceDlg->close();
            replaceDlg=NULL;
            findLineEdit1=NULL;
            replaceLineEdit=NULL;
        }

    }
}

void MainWindow::createReplaceDlg()
{
    //创建替换窗口
    replaceDlg = new QDialog(this);
    replaceDlg->setWindowTitle(tr("替换"));

    findLineEdit1 = new QLineEdit(replaceDlg);
    findLineEdit1->setPlaceholderText("查找");

    replaceLineEdit = new QLineEdit(replaceDlg);
    replaceLineEdit->setPlaceholderText("替换为");
    QPushButton *nextbtn= new QPushButton(tr("下一个"), replaceDlg);
    QPushButton *prebtn=new QPushButton(tr("上一个"), replaceDlg);
    QPushButton *replacebtn=new QPushButton(tr("替换"), replaceDlg);
    QPushButton *replaceallbtn=new QPushButton(tr("全部替换"), replaceDlg);
    QVBoxLayout *layout= new QVBoxLayout(replaceDlg);
    layout->addWidget(findLineEdit1);
    layout->addWidget(replaceLineEdit);
    layout->addWidget(replacebtn);
    layout->addWidget(replaceallbtn);
    layout->addWidget(prebtn);
    layout->addWidget(nextbtn);
    replaceDlg->show();

    connect(prebtn, SIGNAL(clicked()), this, SLOT(showpre()));
    connect(nextbtn, SIGNAL(clicked()), this, SLOT(shownext()));
    connect(replacebtn, SIGNAL(clicked()), this, SLOT(replace()));
    connect(replaceallbtn, SIGNAL(clicked()), this, SLOT(replaceall()));
}

MainWindow::~MainWindow()
{
    list tmp=header;
    while(header)
    {
        header=header->next;
        delete tmp->a;
        delete tmp;
        tmp=header;
    }
}
