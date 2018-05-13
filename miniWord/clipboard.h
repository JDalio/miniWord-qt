#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <string.h>

//数据结构中每一行的定义
typedef struct row
{
    int num;//行号
    char *a;//每一行数组的首地址
    int total;
    int size;//每行的实际字符数，是光标移动的右界
    struct row *next;
} row, *list;

typedef struct clipdata
{
    char *a;
    int size;
    struct clipdata *next;
} clipdata, *clipheader;

class board
{
public:
    //剪切时，单独行的操作
    int clip(list row, int minx, int maxx);

    //粘贴时，单独行的操作
    void append(char *a);

    //清空剪切板
    void refresh();

private:
    //往剪切板压入一行
    void push(char *src, int start = -1, int end = -1);

    clipheader header, tail;
};

#endif // CLIPBOARD_H
