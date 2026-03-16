#include "imekey.h"

#if defined(Q_OS_LINUX)
#include "imekey_impl_lnx.h"
#include <QDebug>

clsHKL::clsHKL(void) { }

int getFcitx5IMEs(QStringList *o_pqStringList) { (void)o_pqStringList;
    char *env = nullptr;
    int count = 0;
    QFile qF;
    env = getenv("HOME");
    QString iniF(QString(env)+QString("/.config/fcitx5/profile"));
    QSettings qSet(iniF, QSettings::IniFormat);
    QStringList qGrpLst = qSet.childGroups();

    qF.setFileName(iniF);
    if(qF.open(QIODevice::ReadOnly|QIODevice::Text)==false) return 0;
    qF.close();;

    for (const QString &group : qGrpLst) {
        if(! group.contains("Groups")) { continue; }
        qSet.beginGroup(group);

        QStringList lstSubIdx = qSet.childGroups();
        for(const QString &strSubIdx : lstSubIdx) {
            qSet.beginGroup(strSubIdx + "/Items");
            QStringList lstTmIdx = qSet.childGroups();
            //qDebug() << "ItemIndex:" << strSubIdx;
            for(const QString &strTmIdx : lstTmIdx) {
                qSet.beginGroup( strTmIdx );
                QString name = qSet.value("Name").toString();
                QString layout = qSet.value("Layout").toString();
                qSet.endGroup();
                if(name.length() <= 0) { continue; }
                if(o_pqStringList) { o_pqStringList->append(name); }
                count++;
            }
        }
    }
    return count;
}

int32_t getScreenSaverActiveTime_wayland() {
    QDBusInterface iface("org.gnome.ScreenSaver", "/org/gnome/ScreenSaver",
                         "org.gnome.ScreenSaver", QDBusConnection::sessionBus());
    if (iface.isValid()) {
        QDBusReply<uint> reply = iface.call("GetActiveTime");
        qDebug() << "select gnome screensaver timer" << QString::number(reply, 10);
        return reply.value(); // 回傳已閒置秒數
    }
    return -1;
}

int32_t getScreenSaverActiveTime_x11() {
    Display *display = XOpenDisplay(NULL);
    if (!display) return -1;

    XScreenSaverInfo *info = XScreenSaverAllocInfo();
    XScreenSaverQueryInfo(display, DefaultRootWindow(display), info);

    long idle_ms = info->idle;

    XFree(info);
    XCloseDisplay(display);
    //qDebug() << "select gnome screensaver timer" << QString::number(idle_ms, 10);
    return idle_ms;
}

void IMEkey::initIMEkey(void) {
    char *env = getenv("XDG_SESSION_TYPE");
    p_HKL = new clsHKL();
    if(! strcmp("x11", env)) { p_flags |= flag_os_linux_x11; }
    if(! strcmp("wayland", env)) { p_flags |= flag_os_linux_wayland; }
    if(! strcmp("tty", env)) { p_flags |= flag_os_linux_tty; }
    if(!(p_flags & flag_os_mask)) p_flags |= flag_os_linux_tty;
    //p_HKL->curHKL = "keyboard-us";
    p_HKL->pvIME = &getFcitx5IMEs; //define is using fcitx5
    if(getScreenSaverActiveTime_x11() > 0) {
        p_HKL->pvTimeOut = &getScreenSaverActiveTime_x11; }
    else if(getScreenSaverActiveTime_wayland() > 0) {
        p_HKL->pvTimeOut = &getScreenSaverActiveTime_wayland; }
    else p_HKL->pvTimeOut = nullptr;
}

void IMEkey::createSysLangGroupBox() {
    int res, i;
    QStringList aryHKLs;
    if(qgbSysLang!=nullptr) return;
    this->qgbSysLang = new QGroupBox(tr("System_Languages") );
    this->qlLang = new QLabel("target_lang:");
    this->qcbxSysLang = new QComboBox;

    p_HKL->numHKL = 0;
    p_HKL->curHKL = "";
    //res = getFcitx5IMEs(&aryHKLs);
    res = p_HKL->pvIME(&aryHKLs);
    if(!res) return;
    p_HKL->numHKL = res;
    for(i=0; i<p_HKL->numHKL; i++) {
        p_HKL->aryHKL.push_back(aryHKLs[i]);
        qcbxSysLang->addItem(aryHKLs[i]);
    }
    i = qcbxSysLang->findText("keyboard-us");
    if(i >= 0) {
        p_HKL->curHKL = p_HKL->aryHKL[i];
        qcbxSysLang->setCurrentIndex(i);
    }
    {
        QHBoxLayout *langLayout = new QHBoxLayout;
        langLayout->addWidget(this->qlLang);
        langLayout->addWidget(this->qcbxSysLang);
        this->qgbSysLang->setLayout(langLayout);
    }
}

void IMEkey::slt_selTgtLang(int idx) { if(idx<0) return; p_HKL->curHKL = p_HKL->aryHKL[idx]; }

void IMEkey::slt_CheckSystemIdle(void) {
    if(!p_HKL->pvTimeOut) return;
    if(p_HKL->pvTimeOut() > p_nTIMER ) { emit sig_SystemIdleTimeout(); }
}

void IMEkey::slt_ChangeKeyboardLayout(void) {
    QDBusInterface iface("org.fcitx.Fcitx5",
                         "/controller",
                         "org.fcitx.Fcitx.Controller1",
                         QDBusConnection::sessionBus());
    if (iface.isValid()) {
        iface.call("SetCurrentIM", p_HKL->curHKL);
    }
}

void IMEkey::setTgtLang(QString lang) {
    //int idx = p_HKL->numHKL;
    //QString qSTR(pSTR);
    //for(idx--; idx>0; idx--) {
    //    if(qSTR.compare(p_HKL->aryHKL[idx]) != 0) continue;
    //    p_HKL->curHKL=p_HKL->aryHKL[idx];
    //    qcbxSysLang->setCurrentIndex(idx+1);
    //    break;
    //}
    int idx = qcbxSysLang->findText(lang);
    if(idx < 0 ) return;
    p_HKL->curHKL = p_HKL->aryHKL[idx];
    qcbxSysLang->setCurrentIndex(idx); return;

}

#endif
