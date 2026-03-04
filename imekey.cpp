#include "imekey.h"
#include "./ui_imekey.h"

IMEkey::IMEkey(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::IMEkey)
{
    p_curHKL = 0x04090409;
    p_aryHKL = nullptr;
    p_numHKL = 0;
    p_Timer = 3000;
    qlTitle = nullptr;
    qlBody = nullptr;
    qlHotkey = nullptr;
    qlLang = nullptr;
    qlTimeout = nullptr;
    qgbSysLang = nullptr;
    qcbSysLang = nullptr;
    minimizeAction = nullptr;
    restoreAction = nullptr;
    quitAction = nullptr;
    trayIcon = nullptr;
    trayIconMenu = nullptr;
    p_qtiTimer = new QTimer(this);
    qiconICON = QIcon(":/key.png");
    //ui->setupUi(this);
    createTimerGroupBox();
    createSysLangGroupBox();
    createTrayIcon();

    p_qtiTimer->setInterval(1000);
    p_Timer_lpi.cbSize = sizeof(LASTINPUTINFO);
    connect(trayIcon, &QSystemTrayIcon::messageClicked, this, &IMEkey::messageClicked);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &IMEkey::iconActivated);
    connect(qcbxTIMER, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &IMEkey::setTIMER);
    connect(qcbSysLang, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &IMEkey::setIME);
    connect(p_qtiTimer, &QTimer::timeout, this, &IMEkey::checkSystemIdle);
    connect(this, &IMEkey::SystemIdleTimeOut, this, &IMEkey::activeIME);
    {
        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addWidget(qgbTIMER);
        mainLayout->addWidget(qgbSysLang);
        setLayout(mainLayout);
    }
    qcbSysLang->setCurrentIndex(qcbSysLang->findText("en-US"));
    qcbxTIMER->setCurrentIndex(qcbxTIMER->findText("3000"));
    trayIcon->setIcon(qiconICON);
    setWindowIcon(qiconICON);
    trayIcon->show();
    setWindowTitle(tr("IMEkey"));
    resize(300,150);
    p_qtiTimer->start();
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
    this->qcbSysLang = new QComboBox;

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
            this->qcbSysLang->addItem(QString::fromWCharArray((const wchar_t*)wstr)); }
        if(res == 0) { this->qcbSysLang->addItem(QString("not sound") ); }
        free(pqwHKL);
    }
    {
        QHBoxLayout *langLayout = new QHBoxLayout;
        langLayout->addWidget(this->qlLang);
        langLayout->addWidget(this->qcbSysLang);
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
    //trayIconMenu->addAction(maximizeAction);
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

void IMEkey::setIME(int index) { if(index<0) return;
    p_curHKL = p_aryHKL[index]; }

void IMEkey::setTIMER(int index) { if(index<0) return;
    p_Timer = qcbxTIMER->itemText(index).toUInt(); }

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
    trayIcon->showMessage(qlTitle->text(), "bodyEditText", qiconICON, 1000);
}

void IMEkey::setVisible(bool visible) {
    minimizeAction->setEnabled(visible);
    //maximizeAction->setEnabled(!isMaximized());
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
    qgbTIMER = new QGroupBox(tr("idle timer"));
    qlTIMER = new QLabel("idle time(mS):");
    qcbxTIMER = new QComboBox;
    //QIcon icoKey(":/key.png");

    qcbxTIMER->addItem(tr("1000"));
    qcbxTIMER->addItem(tr("2000"));
    qcbxTIMER->addItem(tr("3000"));
    qcbxTIMER->addItem(tr("4000"));
    qcbxTIMER->addItem(tr("5000"));

    QHBoxLayout *iconLayout = new QHBoxLayout;
    iconLayout->addWidget(qlTIMER);
    iconLayout->addWidget(qcbxTIMER);
    qgbTIMER->setLayout(iconLayout);
}

void IMEkey::activeIME() {
#ifdef WIN32
    HWND hwndForeground = GetForegroundWindow();
    HKL chkl = (HKL)(uint64_t)p_curHKL;
    if(!hwndForeground) return;
    PostMessage(hwndForeground, WM_INPUTLANGCHANGEREQUEST, 0, (LPARAM)chkl);
#endif
}

void IMEkey::idleTimeOut() {
}

void IMEkey::checkSystemIdle() {
#ifdef WIN32
    uint64_t currentTime;
    int32_t idleTime;
    if (GetLastInputInfo(& p_Timer_lpi)) {
        currentTime = GetTickCount64(); // 系統啟動至今的毫秒數
        idleTime = (int32_t)(currentTime - p_Timer_lpi.dwTime); // 計算閒置了多久
        if((int32_t)idleTime < p_Timer) return;
        emit SystemIdleTimeOut();
    }
#endif
}

void IMEkey::setIdleTimerVal(int32_t val) { this->p_Timer = val; }
void IMEkey::setTargetFCID(uint32_t FCID) {
    int count = qcbSysLang->count();
    for(count--; count>0; count--) {
        if(p_aryHKL[count] != FCID) continue;
        p_curHKL = FCID; break;
    }
}
