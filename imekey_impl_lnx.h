#ifndef _imekey_impl_lnx_
#define _imekey_impl_lnx_
#include <imekey.h>
#if defined(Q_OS_LINUX)

#include <X11/XKBlib.h>

using namespace std;
class clsHKL {
public:
    explicit clsHKL(void);
    int numHKL;
    vector<QString> aryHKL;
    QString curHKL;
};

#endif
#endif //_imekey_impl_lnx_
