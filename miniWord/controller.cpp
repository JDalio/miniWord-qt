#include <mainwindow.h>
//打印数据结构
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
            for(hsen cursen=currow->h;cursen&&cursen->num<=x/100;cursen=cursen->next)
                strcat(s1,cursen->a);
            strcat(s1,"\n");
            currow=currow->next;

        }
//        qDebug()<<"1";
        //打印位置行的复制,位置之前的复制
        hsen cursen=currow->h;
        int sennum=x/100;
        int index=x%100;
        while(cursen->num<sennum)
        {
            strcat(s1,cursen->a);
            //s1[(cursen->num+1)*100]='\0';
            //strcat(s1,"\0");
            cursen=cursen->next;
        }
//        qDebug()<<"2";
        //剩余小于100段的附加
        strncat(s1,cursen->a,index+1);
        //copy the former part && append the cursor
        strcpy(s2,s1);

        strcat(s1,"<span style='font-size:20px;font-weight:900;background-color:white;color:black;margin:0;'>|</sapn><span style='font-weight:500;font-size:16px;'>");
        strcat(s2,"<sapn style='font-size:20px;font-weight:900;background-color:white;color:white;margin:0;'>|</span><span style='color:black;font-weight:500;font-size:16px;'>");

        //append the latter part which may less than 100
        char *latter=&cursen->a[index+1];
        strcat(s1,latter);
        strcat(s2,latter);

        //后半部分大小为100整块的复制，复制完整个数据结构
        cursen=cursen->next;
        while(cursen)
        {
            strcat(s1,cursen->a);
            strcat(s2,cursen->a);
            cursen=cursen->next;
        }
//        qDebug()<<"3";
        if(currow->next)
        {
            strcat(s1,"\n");
            strcat(s2,"\n");
        }


        currow=currow->next;
        while(currow)
        {
            for(hsen cursen=currow->h;cursen&&cursen->num<=x/100;cursen=cursen->next)
            {
                strcat(s1,cursen->a);
                strcat(s2,cursen->a);
            }
            strcat(s1,"\n");
            strcat(s2,"\n");
            currow=currow->next;
        }
        //qDebug()<<"4";
        //格式化str1和str2，会自动在blink()中显示
        strcat(s1,"</span></pre>");
        strcat(s2,"</span></pre>");
        char *tmp1=str1;
        char *tmp2=str2;
        str1=s1;
        str2=s2;

        qDebug()<<str1 <<endl <<str2;
        if(str1&&str2)
        {
            delete tmp1;
            delete tmp2;
        }
        //qDebug()<<"5";

    }

}
//字符输入编辑数据结构然后更新屏幕字符串,x是行，y是列
void MainWindow::edit(char ch)
{
  //qDebug() <<"坐标" <<x  <<" " <<y;
/** 1、定位光标在数据结构中的位置 **/
    //row pointer postion
    list currow=header;
    while(currow->num!=y)
        currow=currow->next;

    //col pointer postion
    hsen cursen=currow->h;
    //使cursen始终保持拆入块的位置
    while(cursen->next && cursen->num!=x/100)
        cursen=cursen->next;

    int index=x%100;

/** end of 1 **/

/** 在相应的光标位置处操作 **/
    switch(ch)
    {
        case '\b':
        /** 分两种情况 x=0，删除当前行，x!=0删除行中的字符 **/{
            qDebug() <<"backspace: " <<"x: " <<x <<"y: " <<y;
            if(x==0)
            {
                if(y==0)
                {
                    cursen=cursen->next;
                    hsen presen=cursen;
                    while(presen)
                    {
                        cursen=cursen->next;
                        delete presen;
                        presen=cursen;

                    }
                    cursen=header->h;
                    cursen->next=NULL;
                    header->size=0;
                    for(int i=0;i<101;i++)
                        cursen->a[i]='\0';

                }
                else
                {
                    hsen presen=cursen;
                    while(presen)
                    {
                        cursen=cursen->next;
                        delete presen;
                        presen=cursen;

                    }
                    list pre=header;
                    while(pre->next!=currow)
                        pre=pre->next;
                    pre->next=currow->next;
                    delete currow;
                }
                list tmp=header;
                int i;
                for(i=0;tmp;tmp=tmp->next,i++)
                    tmp->num=i;
                if(y==i)
                    y--;
                print(x-1,y);

            }
            else
            {
//                if(x==currow->size)
//                    x--;
                char board[currow->size+1]={'\0'};
                cursen=currow->h;
                //先将块链内容抄出来
                while(cursen)
                {
                    for(int i=100*cursen->num,j=0;i<currow->size;i++,j++)
                    {
                        board[i]=cursen->a[j];
                        cursen->a[j]='\0';
                    }

                    cursen=cursen->next;
                }
                //qDebug() <<board;
                //修改块链的规模，如最后一块长度为1则
                cursen=currow->h;
                while(cursen->next)
                    cursen=cursen->next;
                if(strlen(cursen->a)==1&&cursen->num!=0)
                {
                    hsen tmp=currow->h;
                    while(tmp->next!=cursen)
                        tmp=tmp->next;
                    delete cursen;
                    tmp->next=NULL;
                }
                if(strlen(cursen->a)==1&&cursen->num==0)
                {
                    for(int i=0;i<100;i++)
                        cursen->a[i]='\0';
                }
                //将抄出来的内容抄回去
                cursen=currow->h;
                while(cursen)
                {
                    for(int i=100*cursen->num,j=0;i<currow->size;i++,j++)
                    {
                        if(i!=x-1)
                            cursen->a[j]=board[i];
                        else
                            j--;
                    }
                    cursen=cursen->next;
                }
                x--;
                if(currow->size>0)
                    currow->size--;
                //qDebug() <<currow->size;
                print(x-1,y);


            }
            break;
            }
        break;

        case '\r':
            qDebug() <<"enter:" <<"x: " <<x <<"y: " <<y;
        /** 分三种情况 行末回车，行中回车，行末回车 **/{
            if(x==currow->size)
            {
                list next=currow->next;
                currow->next=new row;
                currow->next->num=currow->num+1;
                currow=currow->next;
                currow->next=next;

                currow->h=new sen;
                currow->size=0;
                cursen=currow->h;
                cursen->a=new char [101];
                for(int i=0;i<101;i++)
                    cursen->a[i]='\0';
                cursen->num=0;
                cursen->next=NULL;

                while(next)
                {
                    next->num++;
                    next=next->next;
                }

                x=0;
                y++;
                print(x,y);

            }
            else if(x==0)
            {

            }
            else
            {

            }
        }
        break;

        default:
        /** 当输入字母和数字的时候 **/{
            //qDebug()<<"Input:" <<ch;
            //最后一块是判断条件，若最后一块满，则一定要先扩充块链
            //指向当前行最后一块
            hsen tmp=cursen;
            while(tmp->next)
                tmp=tmp->next;
            //判断最后一块的长度。若等于100，要加一块,然后开始挪，
            int len=strlen(tmp->a);
            //qDebug() <<"len: " <<len;
            //qDebug() <<"坐标" <<x  <<" " <<y;
            if(len==100)
            {
                qDebug()<<"length 100";
                tmp->next=new sen;
                tmp->next->num=tmp->num+1;
                tmp=tmp->next;
                tmp->a=new char[101];
                for(int i=0;i<101;i++)
                    tmp->a[i]='\0';
                tmp->next=NULL;
            }

            //如果是单纯的当前行末尾插入
            if(x==currow->size)
                tmp->a[x%100]=ch;
            else
            {

                char last_letters[tmp->num-cursen->num+1];

                tmp=cursen;
                for(int i=0;tmp;i++,tmp=tmp->next)
                {
                    //当块为满块时
                    if(strlen(tmp->a)==100)
                    {
                        last_letters[i]=tmp->a[99];
                        if(tmp==cursen)
                        {
                            for(int i=strlen(cursen->a)-1;i>index;i--)
                            {
                                qDebug()<<"Move: " <<i-1 <<":" <<cursen->a[i-1] <<"->" <<i <<":"<<cursen->a[i] <<endl;
                                tmp->a[i]=tmp->a[i-1];
                            }
                        }
                        else
                        {
                            for(int i=99;i>0;i--)
                                tmp->a[i]=tmp->a[i-1];
                        }

                    }
                    else
                    {
                        if(cursen->num!=tmp->num)//中间插入当前块无位置，最后一个有位置块必定从头移动
                            for(int i=strlen(tmp->a);i>0;i--)
                                tmp->a[i]=tmp->a[i-1];
                        else//当中间插入，指针自动移动到最后一个块时，如果此块有位置
                            for(int i=strlen(tmp->a);i>index;i--)
                                tmp->a[i]=tmp->a[i-1];
                    }

                }

                tmp=cursen->next;
                for(int i=0;tmp;tmp=tmp->next,i++)
                    tmp->a[0]=last_letters[i];
                cursen->a[index]=ch;
            }
            print(x,y);
            x++;
            total++;
            currow->size++;
            /** 输入字母和数字的情况结束 **/}
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
