#ifndef CONFIG_H
#define CONFIG_H

#include <QtGlobal>

#if defined(NTPLIBCLI_LIBRARY)
#   define NTPLIBCLI_EXPORT Q_DECL_EXPORT
#else
#   define NTPLIBCLI_EXPORT Q_DECL_IMPORT
#endif

#endif // CONFIG_H
