#ifndef READREGISTER_H
#define READREGISTER_H

#include <QVariant>
#include <QSettings>

class ReadRegister
{
public:
    ReadRegister();
    static QVariant readInRegister(QString path);
};

#endif // READREGISTER_H
