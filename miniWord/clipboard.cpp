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
}

void board::push(char *src, int start, int end)
{
    clipheader newnode = new clipdata;
    if (start != -1 && end != -1)
        newnode->size = end - start;
    else
        newnode->size = strlen(src);
    newnode->a = new char[newnode->size + 1];
    newnode->a[0] = '\0';
    if (start != -1 && end != -1)
        strncat(newnode->a, &src[start], newnode->size);
    else
        strcat(newnode->a, src);
    newnode->next = NULL;
    if (!header)
    {
        header = newnode;
        tail = newnode;
    } else
        tail->next = newnode;
}

void board::append(char *a)
{
    if (header == tail)
    {
        strcat(a, header->a);
        delete header->a;
        delete header;
        header = NULL;
    } else
    {
        clipheader tmp = header;
        while (tmp->next != tail)
            tmp = tmp->next;
        strcat(a, tail->a);
        delete tail->a;
        delete tail;
        tmp->next = NULL;
        tail = tmp;
    }
}

int board::clip(list currow, int minx, int maxx)
{
    push(currow->a, minx, maxx);
    currow->size -= (maxx - minx);
    currow->total = (currow->size / 100 + 1) * 100;
    char *tmp = new char[currow->total + 1];
    tmp[0] = '\0';

    strncat(tmp, currow->a, minx);
    strcat(tmp, &currow->a[maxx]);

    delete currow->a;
    currow->a = tmp;
    return maxx - minx;
}
