#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <QObject>

typedef void (*mainFunctionVoidVoid_t)( void );
//typedef int (*mainFunctionIntVoid_t)( void );
//typedef int (*mainFunctionVoidIntChar_t)( int, char** );


class MainClass : public QObject
{
    Q_OBJECT
public:
    explicit MainClass(QObject *parent = 0);
    MainClass(mainFunctionVoidVoid_t fn) :
        QObject(NULL),
        m_fn(fn)
    {}

signals:

public slots:
    void run() {
        m_fn();
    }


private:
    mainFunctionVoidVoid_t m_fn;
};



#endif // MAINCLASS_H
