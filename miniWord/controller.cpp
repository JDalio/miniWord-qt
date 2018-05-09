#include <mainwindow.h>
//字符输入编辑数据结构然后更新屏幕字符串,x是行，y是列
void MainWindow::edit(char ch)
{
/** 1、定位光标在数据结构中的位置 **/
    //row pointer postion
    list currow=header;
    while(currow->num!=y)
        currow=currow->next;

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
                    currow->a=(char*)realloc(currow->a,101*sizeof(char));
                    currow->a[0]='\0';
                    header->size=0;
                    header->total=100;
                    total=0;
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
                if(y==i)
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
