#ifndef QTWIZARD_GLOBAL_H
#define QTWIZARD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTWIZARD_LIBRARY)
#  define QTWIZARDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTWIZARDSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTWIZARD_GLOBAL_H
