#include "imekey.h"

#if defined(Q_OS_LINUX)
#inclide "imekey_impl_lnx.h"

void IMEkey::initIMEkey(void) {
    {   char *env = getenv("XDG_SESSION_TYPE");
        if(! strcmp("x11", env)) { p_flags |= flag_os_linux_x11; }
        if(! strcmp("wayland", env)) { p_flags |= flag_os_linux_wayland; }
        if(! strcmp("tty", env)) { p_flags |= flag_os_linux_tty; } }
    p_curHKL = 0;
    p_flags |= flag_os_linux;
}

void IMEkey::createSysLangGroupBox() {
    int res;
    if(qgbSysLang!=nullptr) return;
    this->qgbSysLang = new QGroupBox(tr("System_Languages") );
    this->qlLang = new QLabel("target_lang:");
    this->qcbxSysLang = new QComboBox;

    res = GetKeyboardLayoutList(0, nullptr);
    if(res > array_hkl_size) { res = array_hkl_size; }
    p_numHKL = res;
    if(res > 0) {
        int32_t i;
        uint64_t *pqwHKL;
        wchar_t wstr[64];
        pqwHKL = (uint64_t*)malloc(sizeof(uint64_t) * p_numHKL);
        p_aryHKL = new uint32_t(p_numHKL);
        //res = getSystemKeyboardLayouts(p_numHKL, pqwHKL);
        res = GetKeyboardLayoutList(p_numHKL, (HKL*)pqwHKL);
        for(i=0; i<res; i++) {
            //p_aryHKL[i] = (uint32_t)(pqwHKL[i]);
            LCIDToLocaleName((LCID)(p_aryHKL[i] & 0x7fff), wstr, 64, 0);
            this->qcbxSysLang->addItem(QString::fromWCharArray((const wchar_t*)wstr)); }
        if(res == 0) { this->qcbxSysLang->addItem(QString("not sound") ); }
        free(pqwHKL);
    }
    {
        QHBoxLayout *langLayout = new QHBoxLayout;
        langLayout->addWidget(this->qlLang);
        langLayout->addWidget(this->qcbxSysLang);
        this->qgbSysLang->setLayout(langLayout);
    }
}

void IMEkey::slt_selTgtLang(int idx) { p_HKL->curHKL = p_HKL->aryHKL[idx]; }

void IMEkey::slt_CheckSystemIdle(void) {
    uint64_t currentTime;
    int32_t idleTime;
    if(!GetLastInputInfo(&p_TIMERlpi)) return;
    currentTime = GetTickCount64(); // 系統啟動至今的毫秒數
    idleTime = (int32_t)(currentTime - p_TIMERlpi.dwTime); // 計算閒置了多久
    if (idleTime < p_nTIMER) return;
    else { emit sig_SystemIdleTimeout(); }
}

void IMEkey::slt_ChangeKeyboardLayout(void) {
    HWND hwnd = GetForegroundWindow();
    HKL chkl = (HKL)(uint64_t)p_curHKL;
    if(!hwnd) return;
    PostMessage(hwnd, WM_INPUTLANGCHANGEREQUEST, 0, (LPARAM)chkl);
}

#endif
