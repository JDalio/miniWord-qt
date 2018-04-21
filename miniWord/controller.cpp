#include <mainwindow.h>
//打印数据结构
void MainWindow::print(int x, int y, int x1, int y1)
{

    if(x1==-1&&y1==-1)
    {
        //delete str1;delete str2;
        char *s1=new char[total+300];strcpy(s1,"<pre style='font-weight:500;font-size:16px;'>");
        char *s2=new char[total+300];strcpy(s2,"<pre style='font-weight:500;font-size:16px;'>");

        list currow=header;
        //打印位置那行之前的复制，每行末无换行符，遍历过程中append
        while(currow->num<y)
        {
            for(hsen cursen=currow->h;cursen->num<=x/100;cursen=cursen->next)
                strcat(s1,cursen->a);
            //s1[(cursen->num+1)*100+1]='\n';
            strcat(s1,"\n");
            currow=currow->next;

        }
        //qDebug()<<"1";
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
        //qDebug()<<"2";
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
        //qDebug()<<"3";
        if(currow->next)
        {
            strcat(s1,"\n");
            strcat(s2,"\n");
        }


        currow=currow->next;
        while(currow)
        {
            for(hsen cursen=currow->h;cursen->num<=x/100;cursen=cursen->next)
                strcat(s1,cursen->a);
            strcat(s1,"\n");
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

        //qDebug()<<str1 <<endl <<str2;
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
            qDebug() <<"backspace";
        break;

        case '\r':
            qDebug() <<"enter";
        break;

        default:
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
