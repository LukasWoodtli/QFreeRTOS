#ifndef INTERRUPTFLAGS_H
#define INTERRUPTFLAGS_H

#include <QObject>
#include <QFlags>
#include <QMutex>

class InterruptFlags : public QObject
{
    Q_OBJECT
public:
    enum Interrupt {
        TaskTimer = (1 << 0),
        Yield     = (1 << 1)
    };

    Q_DECLARE_FLAGS(Interrupts, Interrupt)
    Q_FLAGS(Interrupts)

    explicit InterruptFlags(QObject *parent = 0);
    void setFlag(Interrupt interrupt);
    bool isZero();
    bool isFlagSet(Interrupt interrupt);
    void clear();
    int numFlags() const;

signals:

public slots:
protected:
    Interrupts m_interruptFlags;
    QMutex     m_lock;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(InterruptFlags::Interrupts);

#endif // INTERRUPTFLAGS_H
