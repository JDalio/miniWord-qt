#include <mainwindow.h>
//打印数据结构
void MainWindow::print(int x, int y, int x1, int y1)
{
    qDebug() <<"坐标" <<x  <<" " <<y;
    if(x1==-1&&y1==-1)
    {
        //delete str1;delete str2;
        char *s1=new char[total+200];strcpy(s1,"<pre style='font-weight:500;font-size:16px;'>");
        char *s2=new char[total+200];strcpy(s2,"<pre style='font-weight:500;font-size:16px;'>");

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
            currow->size++;
            //char position
            int index=x%100;
            if(currow->size!=0&&currow->size%100==0)
            {
                while(cursen->next)
                    cursen=cursen->next;
                cursen->a[100]='\0';
                cursen->next=new sen;
                cursen->next->num=cursen->num+1;
                cursen=cursen->next;
                cursen->a=new char[101];
                cursen->next=NULL;
                //currow->size+=100;
                //index=0;
            }
            else
            {
                int sennum=x/100;

                while(cursen->num!=sennum)
                    cursen=cursen->next;
            }
            //rightBdry=(rightBdry+1)%100;
            qDebug()<<"Input:" <<ch;
            if(strlen(cursen->a)<100)
                for(int i=strlen(cursen->a);i>=index;i--)
                {
                    //qDebug()<<i <<" " <<cursen->a[i] <<"->" <<cursen->a[i] <<endl;
                    cursen->a[i+1]=cursen->a[i];
                }
            else//对整个行块链进行移动
            {
                //tmp is tail and cursen is "head"
                hsen tmp=cursen;
                while(tmp->next)
                    tmp=tmp->next;
                //if(currow->size%100)
                char last_letters[tmp->num-cursen->num+1];
                tmp=cursen;
                for(int i=0;tmp->next;i++,tmp=tmp->next)
                {
                    last_letters[i]=tmp->a[99];
                    if(tmp==cursen)
                        for(int i=strlen(cursen->a)-1;i>index;i--)
                        {
                            qDebug()<<i <<" " <<cursen->a[i-1] <<"->" <<cursen->a[i] <<endl;
                            cursen->a[i]=cursen->a[i-1];
                        }
                    else
                    {
                        for(int i=99;i>0;i--)
                            tmp->a[i]=tmp->a[i-1];
                    }
                }
                tmp=cursen->next;
                for(int i=0;tmp->next;tmp=tmp->next,i++)
                    tmp->a[0]=last_letters[i];

            }
            cursen->a[index]=ch;
            //total++;
            /*int len=strlen(cursen->a);
            if(len<100)
                cursen->a[len+1]='\0';
            else
                cursen->a[100]='\0';*/
            print(x,y);
            x++;
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
