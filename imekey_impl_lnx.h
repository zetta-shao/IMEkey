#ifndef _imekey_impl_lnx_
#define _imekey_impl_lnx_
#include <imekey.h>
#include <QtDBus/QtDBus>

#if defined(Q_OS_LINUX)
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/extensions/scrnsaver.h>
#include <iostream>

using namespace std;

//int getFcitx5IMEs(QStringList *o_pqStringList)
typedef int (*getSystemIMEs)(QStringList*);
typedef int32_t (*getScreenSaverActiveTime)(void);

int getFcitx5IMEs(QStringList *o_pqStringList);

class clsHKL {
public:
    explicit clsHKL(void);
    int numHKL;
    vector<QString> aryHKL;
    QString curHKL;
    getSystemIMEs pvIME;
    getScreenSaverActiveTime pvTimeOut;
};

#endif
#endif //_imekey_impl_lnx_
