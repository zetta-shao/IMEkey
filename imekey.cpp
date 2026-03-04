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
    qcbSysLang = nullptr;
    minimizeAction = nullptr;
    maximizeAction = nullptr;
    restoreAction = nullptr;
    quitAction = nullptr;
    trayIcon = nullptr;
    trayIconMenu = nullptr;
    //ui->setupUi(this);
    createIconGroupBox();
    createSysLangGroupBox();
    createTrayIcon();

    connect(trayIcon, &QSystemTrayIcon::messageClicked, this, &IMEkey::messageClicked);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &IMEkey::iconActivated);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(qgbICON);
    mainLayout->addWidget(qgbSysLang);
    setLayout(mainLayout);
    qcbSysLang->setCurrentIndex(0);
    qcbxICON->setCurrentIndex(0);
    trayIcon->show();
    setWindowTitle(tr("IMEkey"));
    resize(300,150);
}

IMEkey::~IMEkey()
{
    delete ui;
}


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

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
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
}

void IMEkey::setVisible(bool visible) {
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
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

void IMEkey::createIconGroupBox() {
    qgbICON = new QGroupBox(tr("Tray Icon"));
    qlICON = new QLabel("Icon:");
    qcbxICON = new QComboBox;
    QIcon icoKey(":/key.png");

    qcbxICON->addItem(icoKey, icoKey.isNull()?tr("none"):tr("key"));

    QHBoxLayout *iconLayout = new QHBoxLayout;
    iconLayout->addWidget(qlICON);
    iconLayout->addWidget(qcbxICON);
    qgbICON->setLayout(iconLayout);
}
