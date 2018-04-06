#ifndef NEWSPAPER_H
#define NEWSPAPER_H
#include <QObject>
class Newspaper:public QObject
{
    Q_OBJECT
public:
    Newspaper(const QString &name):
        m_name(name)
    {

    }

    void send()
    {
        emit newPaper(m_name);
    }

signals:
//signals 块所列出的，就是该类的信号。
//信号就是一个个的函数名，返回值是 void，参数是该类需要让外界知道的数据。

    void newPaper(const QString &name);

private:
    QString m_name;
};
#endif // NEWSPAPER_H
