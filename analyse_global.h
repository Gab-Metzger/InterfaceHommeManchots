#ifndef ANALYSE_GLOBAL_H
#define ANALYSE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ANALYSE_LIBRARY)
#  define ANALYSESHARED_EXPORT Q_DECL_EXPORT
#else
#  define ANALYSESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ANALYSE_GLOBAL_H
