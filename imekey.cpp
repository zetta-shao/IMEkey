#include "imekey.h"
#include "./ui_imekey.h"

IMEkey::IMEkey(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::IMEkey)
{
    p_curHKL = 0x04090409;
    p_aryHKL = nullptr;
    p_numHKL = 0;
    qlTitle = nullptr;
    qlBody = nullptr;
    qlHotkey = nullptr;
    qlLang = nullptr;
    qlTimeout = nullptr;
    qgbSysLang = nullptr;
    qcbxSysLang = nullptr;
    minimizeAction = nullptr;
    restoreAction = nullptr;
    quitAction = nullptr;
    trayIcon = nullptr;
    trayIconMenu = nullptr;
    //ui->setupUi(this);
    qiconICON = QIcon(":/key.png");
    p_TIMERlpi.cbSize = sizeof(LASTINPUTINFO);
    createTimerGroupBox();
    createSysLangGroupBox();
    createTrayIcon();
    trayIcon->setIcon(qiconICON);
    setWindowIcon(qiconICON);
    connect(trayIcon, &QSystemTrayIcon::messageClicked, this, &IMEkey::messageClicked);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &IMEkey::iconActivated);
    connect(qcbxTIMER, QOverload<int>::of(&QComboBox::currentIndexChanged),this,&IMEkey::slt_selIdleTimer);
    connect(qcbxSysLang, QOverload<int>::of(&QComboBox::currentIndexChanged),this,&IMEkey::slt_selTgtLang);
    connect(this, &IMEkey::sig_SystemIdleTimeout, this, &IMEkey::slt_ChangeKeyboardLayout);
    connect(&qtTIMER, &QTimer::timeout, this, &IMEkey::slt_CheckSystemIdle);
    qtTIMER.setInterval(1000);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(qgbTIMER);
    mainLayout->addWidget(qgbSysLang);
    setLayout(mainLayout);
    qcbxSysLang->setCurrentIndex( qcbxSysLang->findText("en-US") );
    qcbxTIMER->setCurrentIndex( qcbxTIMER->findText("3000") );
    trayIcon->show();
    setWindowTitle(tr("IMEkey"));
    resize(300,150);
    qtTIMER.start();
}

IMEkey::~IMEkey() { delete ui; }


int IMEkey::getSystemKeyboardLayouts(int count, void *hkl) { (void)count; (void)hkl;
#ifdef WIN32
    if(count==0) return GetKeyboardLayoutList(0, nullptr);
    return GetKeyboardLayoutList(count, (HKL*)hkl);
#else
    //XkbGetNames
#endif
}

void IMEkey::hklToLocalName(int val, wchar_t *outwSTR, int strsize) { (void)val; (void)outwSTR; (void)strsize;
#ifdef WIN32
    val &= 0x7fff;
    LCIDToLocaleName((LCID)val, (LPWSTR)outwSTR, strsize, 0);
#else
#endif
}

void IMEkey::createSysLangGroupBox() {
    int res;
    if(qgbSysLang!=nullptr) return;
    this->qgbSysLang = new QGroupBox(tr("System_Languages") );
    this->qlLang = new QLabel("target_lang:");
    this->qcbxSysLang = new QComboBox;

    res = getSystemKeyboardLayouts(0, nullptr);
    if(res > array_hkl_size) { res = array_hkl_size; }
    p_numHKL = res;
    if(res > 0) {
        int32_t i;
        uint64_t *pqwHKL;
        wchar_t wstr[64];        
        pqwHKL = (uint64_t*)malloc(sizeof(uint64_t) * p_numHKL);
        p_aryHKL = new uint32_t(p_numHKL);
        res = getSystemKeyboardLayouts(p_numHKL, pqwHKL);
        for(i=0; i<res; i++) {
            p_aryHKL[i] = (uint32_t)(pqwHKL[i]);
            hklToLocalName(p_aryHKL[i], wstr, 64);
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

void IMEkey::createTrayIcon() {
    if(trayIconMenu!=nullptr) return;

    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

void IMEkey::messageClicked() {
    QMessageBox::information(nullptr, tr("Systray"), tr("just a switch")); }

void IMEkey::closeEvent(QCloseEvent *event) {
    if (trayIcon->isVisible()) {
        QMessageBox::information(this, tr("Systray"),
                                 tr("The program will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}

void IMEkey::showMessage() {
    //showIconCheckBox->setChecked(true);
    //int selectedIcon = typeComboBox->itemData(typeComboBox->currentIndex()).toInt();
    //QSystemTrayIcon::MessageIcon msgIcon = QSystemTrayIcon::MessageIcon(selectedIcon);

    //if (selectedIcon == -1) { // custom icon
    //    QIcon icon(iconComboBox->itemIcon(iconComboBox->currentIndex()));
    //    trayIcon->showMessage(titleEdit->text(), bodyEdit->toPlainText(), icon,
    //                      durationSpinBox->value() * 1000);
    //} else {
    //    trayIcon->showMessage(titleEdit->text(), bodyEdit->toPlainText(), msgIcon,
    //                      durationSpinBox->value() * 1000);
    //}
    trayIcon->showMessage(qlTitle->text(), qlBody->text(), qiconICON, 1000);
}

void IMEkey::setVisible(bool visible) {
    minimizeAction->setEnabled(visible);
    restoreAction->setEnabled(isMaximized() || !visible);
    QDialog::setVisible(visible);
}

void IMEkey::iconActivated(QSystemTrayIcon::ActivationReason reason) { //action at click systray icon
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        //iconComboBox->setCurrentIndex((iconComboBox->currentIndex() + 1) % iconComboBox->count());
        break;
    case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;
    default:
        ;
    }
}

void IMEkey::createTimerGroupBox() {
    qgbTIMER = new QGroupBox(tr("system idle time"));
    qlTIMER = new QLabel("idle tile(mS):");
    qcbxTIMER = new QComboBox;

    qcbxTIMER->addItem(tr("1000"));
    qcbxTIMER->addItem(tr("2000"));
    qcbxTIMER->addItem(tr("3000"));
    qcbxTIMER->addItem(tr("4000"));
    qcbxTIMER->addItem(tr("5000"));

    QHBoxLayout *timerLayout = new QHBoxLayout;
    timerLayout->addWidget(qlTIMER);
    timerLayout->addWidget(qcbxTIMER);
    qgbTIMER->setLayout(timerLayout);
}

void IMEkey::slt_selIdleTimer(int idx) { p_nTIMER=qcbxTIMER->itemText(idx).toUInt(); }
void IMEkey::slt_selTgtLang(int idx) { p_curHKL=p_aryHKL[idx]; }

void IMEkey::setIdleOut(int32_t val) {
    int idx = qcbxTIMER->findData(val);
    if(idx != -1) {
        p_nTIMER = val; qcbxTIMER->setCurrentIndex(idx); return; }
    qcbxTIMER->addItem(QString::number(val, 10));
    qcbxTIMER->setCurrentIndex( qcbxTIMER->findData(val) );
}

void IMEkey::setTgtLang(uint32_t FCID) {
    int idx = qcbxSysLang->count();
    for(idx--; idx>0; idx--) {
        if(FCID!=this->p_aryHKL[idx]) continue;
        p_curHKL=p_aryHKL[idx];
        qcbxSysLang->setCurrentIndex(idx+1);
        break;
    }
}

void IMEkey::slt_CheckSystemIdle(void) {
#ifdef WIN32
    uint64_t currentTime;
    int32_t idleTime;
    if(!GetLastInputInfo(&p_TIMERlpi)) return;
    currentTime = GetTickCount64(); // 系統啟動至今的毫秒數
    idleTime = (int32_t)(currentTime - p_TIMERlpi.dwTime); // 計算閒置了多久
    if (idleTime < p_nTIMER) return;
    else { emit sig_SystemIdleTimeout(); }
#endif
}

void IMEkey::slt_ChangeKeyboardLayout(void) {
#ifdef WIN32
    HWND hwnd = GetForegroundWindow();
    HKL chkl = (HKL)(uint64_t)p_curHKL;
    if(!hwnd) return;
    PostMessage(hwnd, WM_INPUTLANGCHANGEREQUEST, 0, (LPARAM)chkl);
#endif
}
