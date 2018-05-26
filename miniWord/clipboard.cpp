#include "clipboard.h"
void board::refresh()
{
    while (header)
    {
        delete header->a;
        clipheader tmp = header;
        header = header->next;
        delete tmp;
    }
    header = NULL;
    tail = NULL;
}

void board::push(char *src, int start, int end)
{
    clipheader newnode = new clipdata;
    if (start != -1 && end != -1)
    {
        newnode->size = end - start;
    }
    else
    {
        newnode->size = strlen(src);
    }

    newnode->a = new char[newnode->size + 1];
    newnode->a[0] = '\0';
    if (start != -1 && end != -1)
    {
        strncat(newnode->a, &src[start], newnode->size);
    }
    else
    {
        strcpy(newnode->a, src);
    }

    newnode->next = NULL;
    if (!header)
    {
        header = newnode;
        tail = newnode;
    } else
    {
        tail->next = newnode;
        tail = tail->next;
    }
}

int board::paste(list mainHeader, int &x, int &y, int &ox, int &oy)
{
    int total = 0;
    clipheader h = header;
    list currow = mainHeader;
    while (currow->num < y)
        currow = currow->next;
    //第一行的粘贴
    char *tmpa = new char[currow->size + 1];
    strcpy(tmpa, currow->a);
    int ypos = currow->num, xpos = x;

    total += h->size;
    currow->size += (h->size - (currow->size - x));
    currow->total = (1 + currow->size / 100) * 100;
    char *tmp = (char *) realloc(currow->a, sizeof(char) * (1 + currow->total));
    tmp[0] = '\0';
    strncat(tmp, currow->a, x);
    strcat(tmp, h->a);
    delete currow->a;
    currow->a = tmp;
    h = h->next;
    //中间行的粘贴
    while (h)
    {
        ypos++;
        total += h->size;
        list newrow = new row;
        newrow->size = h->size;
        newrow->total = (1 + newrow->size / 100) * 100;
        newrow->a = new char[newrow->total + 1];
        strcpy(newrow->a, h->a);
        newrow->next = currow->next;
        currow->next = newrow;
        currow = currow->next;
        h = h->next;
    }
    //最末行的粘贴
    x = currow->size;
    ox = x;
    currow->size += (strlen(tmpa) - xpos);
    currow->total = (1 + currow->size / 100) * 100;
    tmp = new char[currow->total + 1];
    strcpy(tmp, currow->a);
    strcat(tmp, &tmpa[xpos]);

    delete currow->a;
    delete tmpa;
    currow->a = tmp;
    y = ypos;
    oy = y;

    //修改行号
    currow = mainHeader;
    for (int i = 0; currow; currow = currow->next)
    {
        currow->num = i;
        i++;
    }
    return total;
}

int board::clip(list currow, int minx, int maxx)
{
    int dellen = (minx == -1 && maxx == -1 ? strlen(currow->a) : maxx - minx);
    push(currow->a, minx, maxx);
    if (maxx == -1 && minx == -1)
    {
        currow->size = 0;
        currow->total = 100;

    } else
    {
        currow->size -= (maxx - minx);
        currow->total = (currow->size / 100 + 1) * 100;
    }

    char *tmp = new char[currow->total + 1];
    tmp[0] = '\0';

    if (maxx != -1 && minx != -1)
    {
        strncat(tmp, currow->a, minx);
        strcat(tmp, &currow->a[maxx]);
    }

    delete currow->a;
    currow->a = tmp;
    return dellen;
}

void board::copy(list mainHeader, int &x, int &y, int &ox, int &oy, bool clip, int &total)
{
    refresh();
    list currow = mainHeader;
    int minx = (x < ox || ox == -1 ? x : ox), maxx = (x > ox || ox == -1 ? x : ox);
    int miny = (y < oy || oy == -1 ? y : oy), maxy = (y > oy || oy == -1 ? y : oy);

    while (currow->num < miny)
        currow = currow->next;
    if (miny == maxy)
    {
        if (clip)
            total -= this->clip(currow, minx, maxx);
        else
            push(currow->a, minx, maxx);
        x = minx;
        ox = x;
    } else
    {
        int startx = (y == miny ? x : ox), endx = (y == maxy ? x : ox);
        //第一行的处理
        if (clip)
            total -= this->clip(currow, startx, currow->size);
        else
            push(currow->a, startx, currow->size);
        currow = currow->next;
        //剪切中间行的处理
        while (currow->num < maxy)
        {
            if (clip)
                total -= this->clip(currow, -1, -1);
            else
                push(currow->a, -1, -1);
            currow = currow->next;
        }
        //剪切最末行的处理
        if (clip)
            total -= this->clip(currow, 0, endx);
        else
            push(currow->a, 0, endx);
        x = startx;
        ox = x;
        y = miny;
        oy = y;
    }
}
