#ifndef _imekey_impl_win32_
#define _imekey_impl_win32_
#include <imekey.h>
#if defined(Q_OS_WIN32)||defined(Q_OS_WIN64)

#define WINVER _WIN32_WINNT_VISTA
#undef _WIN32_WINNT //mingw define is 0x502
#define _WIN32_WINNT _WIN32_WINNT_VISTA
#include <windows.h>
#include <winuser.h>
#include <winnls.h>
#include <sysinfoapi.h>

using namespace std;
class clsHKL {
public:
    explicit clsHKL(void);
    int numHKL;
    vector<uint32_t> aryHKL;
    uint32_t    curHKL;
    LASTINPUTINFO p_TIMERlpi;
};


#endif

#endif //_imekey_impl_win32_
