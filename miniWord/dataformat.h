#ifndef DATAFORMAT_H
#define DATAFORMAT_H
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
typedef struct pos_target//存储查找目标位置的结构体
{
    int n_row;//行地址
    int n_word;//字符编号
    pos_target *next;
} pos_target, *htar;
#endif // DATAFORMAT_H
