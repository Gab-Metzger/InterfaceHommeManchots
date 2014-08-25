#include "readregister.h"

ReadRegister::ReadRegister()
{
}

QVariant ReadRegister::readInRegister(QString path) {
    QSettings settings("METZGER","IHManchots");
    return settings.value(path);
}
