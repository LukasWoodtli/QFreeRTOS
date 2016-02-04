#ifndef MICROCONTROLLERENVIRONMENT_H
#define MICROCONTROLLERENVIRONMENT_H

#include <QObject>

class MicroControllerEnvironment : public QObject
{
    Q_OBJECT
public:
    explicit MicroControllerEnvironment(QObject *parent = 0);

signals:

public slots:
};

#endif // MICROCONTROLLERENVIRONMENT_H
