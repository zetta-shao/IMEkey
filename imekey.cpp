#include "imekey.h"
#include "./ui_imekey.h"

IMEkey::IMEkey(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::IMEkey)
{
    p_HKL = nullptr;
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
    p_flags = 0;
    //ui->setupUi(this);
    qiconICON = QIcon(":/key.png");

    initIMEkey(); //init. by platfrom
    createTimerGroupBox();
    createSysLangGroupBox();
    createTrayIcon();
    trayIcon->setIcon(qiconICON);
    setWindowIcon(qiconICON);
    trayIcon->setToolTip("IMEkey");
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
    qcbxTIMER->setCurrentIndex( qcbxTIMER->findText("3000") );
    trayIcon->show();
    setWindowTitle(tr("IMEkey"));
    resize(300,150);    
    qtTIMER.start();
}

IMEkey::~IMEkey() { writeSetting(); delete ui; }

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

void IMEkey::readSetting(void) {
    QSettings pSet(QSettings::IniFormat, QSettings::UserScope, "IDEA software", "IMEkey");
    pSet.beginGroup("main");
    if(pSet.value("timeout").isValid()) {
        setIdleOut( pSet.value("timeout").toInt() );
    }
    if(pSet.value("tgtLang").isValid()) {
        setTgtLang( pSet.value("tgtLang").toString() );
    }
    pSet.endGroup();
}

void IMEkey::writeSetting(void) {
    QSettings pSet(QSettings::IniFormat, QSettings::UserScope, "IDEA software", "IMEkey");
    pSet.beginGroup("main");
    pSet.setValue("timeout", p_nTIMER );
    pSet.setValue("tgtLang", qcbxSysLang->currentText() );
    pSet.endGroup();
}

void IMEkey::messageClicked() {
    QMessageBox::information(nullptr, tr("Systray"), tr("just a switch")); }

void IMEkey::closeEvent(QCloseEvent *event) {
    if (trayIcon->isVisible()) {
        //QMessageBox::information(this, tr("Systray"),
        //                         tr("The program will keep running in the "
        //                            "system tray. To terminate the program, "
        //                            "choose <b>Quit</b> in the context menu "
        //                            "of the system tray entry."));
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
    trayIcon->showMessage("IMEkey", "IMEkey", qiconICON, 1000);
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
    case QSystemTrayIcon::MiddleClick:
        { //showMessage();
        trayIcon->contextMenu()->popup(QCursor::pos());
        break; }
    default:
        ;
    }
}

void IMEkey::createTimerGroupBox() {
    qgbTIMER = new QGroupBox(tr("system idle time"));
    qlTIMER = new QLabel("idle tile(mS):");
    qcbxTIMER = new QComboBox;

    qcbxTIMER->addItem(tr("1500"));
    qcbxTIMER->addItem(tr("3000"));
    qcbxTIMER->addItem(tr("5000"));
    qcbxTIMER->addItem(tr("7000"));
    qcbxTIMER->addItem(tr("10000"));
    qcbxTIMER->addItem(tr("15000"));

    QHBoxLayout *timerLayout = new QHBoxLayout;
    timerLayout->addWidget(qlTIMER);
    timerLayout->addWidget(qcbxTIMER);
    qgbTIMER->setLayout(timerLayout);
}

void IMEkey::slt_selIdleTimer(int idx) { p_nTIMER=qcbxTIMER->itemText(idx).toUInt(); }

void IMEkey::setIdleOut(int32_t val) {
    QString str = QString::number(val, 10);
    int idx = qcbxTIMER->findText( str );
    if(idx != -1) {
        p_nTIMER = val; qcbxTIMER->setCurrentIndex(idx); return; }
    qcbxTIMER->addItem( str );
    qcbxTIMER->setCurrentIndex( qcbxTIMER->findText( str ) );
}

