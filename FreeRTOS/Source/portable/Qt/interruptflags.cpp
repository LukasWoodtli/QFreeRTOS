
#include <QMutexLocker>
#include <QMetaEnum>
#include <QDebug>

#include "interruptflags.h"

InterruptFlags::InterruptFlags(QObject *parent) : QObject(parent)
{

}


void InterruptFlags::setFlag(Interrupt interrupt)
{
    QMutexLocker locker(&m_lock);
    m_interruptFlags |= interrupt;
    qDebug() << m_interruptFlags;
}

bool InterruptFlags::isZero()
{
    QMutexLocker locker(&m_lock);
    qDebug() << m_interruptFlags;
    return int(m_interruptFlags) == 0;

}
bool InterruptFlags::isFlagSet(Interrupt interrupt)
{
    QMutexLocker locker(&m_lock);
    return m_interruptFlags.testFlag(interrupt);

}

void InterruptFlags::clear()
{
    QMutexLocker locker(&m_lock);
    m_interruptFlags = 0;
}

int InterruptFlags::numFlags() const {
// from http://blubbqt.blogspot.ch/2013/06/qt-enum-to-string-string-to-enum-flag.html
    return 2; // InterruptFlags::staticMetaObject.enumerator(InterruptFlags::staticMetaObject.indexOfEnumerator("Interrupt")).keyCount());
}
