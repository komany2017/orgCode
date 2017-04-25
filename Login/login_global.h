#ifndef LOGIN_GLOBAL_H
#define LOGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LOGIN_LIBRARY)
#  define LOGINSHARED_EXPORT  Q_DECL_EXPORT
#else
#  define LOGINSHARED_EXPORT  Q_DECL_IMPORT
#endif

#endif // LOGIN_GLOBAL_H
