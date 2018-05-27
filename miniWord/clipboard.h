#ifndef CLIPBOARD_H
#define CLIPBOARD_H
#include <string.h>
#include <QDebug>
#include "dataformat.h"
//数据结构中每一行的定义
class board
{
public:
    //剪切时，单独行的操作
    int clip(list row, int minx, int maxx);

    //全部数据结构的粘贴操作
    int paste(list header, int &x, int &y, int &ox, int &oy);

    //清空剪切板
    void refresh();

    //复制操作
    void copy(list header, int &x, int &y, int &ox, int &oy, bool clip, int &total);

private:
    //往剪切板压入一行
    void push(char *src, int start = -1, int end = -1);

    clipheader header = NULL, tail = NULL;
};

#endif // CLIPBOARD_H
