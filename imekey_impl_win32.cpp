#include "imekey.h"

#if defined(Q_OS_WIN32)||defined(Q_OS_WIN64)
#include "imekey_impl_win32.h"

clsHKL::clsHKL(void) {
}

void IMEkey::initIMEkey(void) {
    p_HKL = new clsHKL();
    p_HKL->p_TIMERlpi.cbSize = sizeof(LASTINPUTINFO);
    p_flags |= flag_os_win32;
}

void IMEkey::createSysLangGroupBox() {
    int res, num;
    if(qgbSysLang!=nullptr) return;
    this->qgbSysLang = new QGroupBox(tr("System_Languages") );
    this->qlLang = new QLabel("target_lang:");
    this->qcbxSysLang = new QComboBox;

    num = GetKeyboardLayoutList(0, nullptr);
    if(num > array_hkl_size) { num = array_hkl_size; }
    if(num > 0) {
        int32_t i;
        uint64_t *pqwHKL;
        wchar_t wstr[64];
        pqwHKL = (uint64_t*)malloc(sizeof(uint64_t) * num);
        res = GetKeyboardLayoutList(num, (HKL*)pqwHKL);
        for(i=0; i<res; i++) {
            p_HKL->aryHKL.push_back((uint32_t)pqwHKL[i]);
            LCIDToLocaleName((LCID)(pqwHKL[i] & 0x7fff), wstr, 64, 0);
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
    p_HKL->curHKL = p_HKL->aryHKL[0];
    p_HKL->numHKL = num;
}

void IMEkey::slt_selTgtLang(int idx) { p_HKL->curHKL = p_HKL->aryHKL[idx]; }

void IMEkey::slt_CheckSystemIdle(void) {
    uint64_t currentTime;
    int32_t idleTime;
    if(!GetLastInputInfo(&p_HKL->p_TIMERlpi)) return;
    currentTime = GetTickCount64(); // 系統啟動至今的毫秒數
    idleTime = (int32_t)(currentTime - p_HKL->p_TIMERlpi.dwTime); // 計算閒置了多久
    if (idleTime < p_nTIMER) return;
    else { emit sig_SystemIdleTimeout(); }
}

void IMEkey::slt_ChangeKeyboardLayout(void) {
    HWND hwnd = GetForegroundWindow();
    HKL chkl = (HKL)(uint64_t)p_HKL->curHKL;
    if(!hwnd) return;
    PostMessage(hwnd, WM_INPUTLANGCHANGEREQUEST, 0, (LPARAM)chkl);
}

void IMEkey::setTgtLang(char *pSTR) {
    int idx = p_HKL->numHKL;
    uint32_t kbid = strtol(pSTR, NULL, 16);
    for(idx--; idx>0; idx--) {
        if(kbid != p_HKL->aryHKL[idx] ) continue;
        p_HKL->curHKL=p_HKL->aryHKL[idx];
        qcbxSysLang->setCurrentIndex(idx+1);
        break;
    }
}

#endif
