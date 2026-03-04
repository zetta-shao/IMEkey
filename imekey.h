#ifndef IMEKEY_H
#define IMEKEY_H

#include <QDialog>
#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QTimer>

#ifdef WIN32
#define WINVER _WIN32_WINNT_VISTA
#undef _WIN32_WINNT //mingw define is 0x502
#define _WIN32_WINNT _WIN32_WINNT_VISTA
#include <windows.h>
#include <winuser.h>
#include <winnls.h>
#include <sysinfoapi.h>
#else
#include <X11/XKBlib.h>
#endif


QT_BEGIN_NAMESPACE
namespace Ui { class IMEkey; }
//class QAction;
//class QCheckBox;
//class QComboBox;
//class QGroupBox;
//class QLabel;
//class QLineEdit;
//class QMenu;
//class QPushButton;
//class QSpinBox;
//class QTextEdit;
QT_END_NAMESPACE

#define array_hkl_size 32

class IMEkey : public QDialog
{
    Q_OBJECT

public:
    IMEkey(QWidget *parent = nullptr);
    ~IMEkey();

    void setVisible(bool visible) override;
    int getSystemKeyboardLayouts(int count, void *hkl);
    void hklToLocalName(int val, wchar_t *outwSTR, int strsize);
    void createTimerGroupBox();
    void createSysLangGroupBox();
    void createTrayIcon();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void setIdleOut(int32_t val);
    void setTgtLang(uint32_t FCID);

public Q_SLOTS:
    void slt_CheckSystemIdle(void);
    void slt_ChangeKeyboardLayout(void);
    void slt_selIdleTimer(int);
    void slt_selTgtLang(int);

Q_SIGNALS:
    void sig_SystemIdleTimeout(void);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void showMessage();
    void messageClicked();

private:
    Ui::IMEkey *ui;
    QIcon qiconICON;
    uint32_t p_curHKL;
    uint32_t *p_aryHKL;
    uint32_t p_numHKL;
    LASTINPUTINFO p_TIMERlpi;

    QGroupBox *qgbTIMER;
    QLabel *qlTIMER;
    QComboBox *qcbxTIMER;
    QTimer qtTIMER;
    int32_t p_nTIMER;

    QGroupBox *messageGroupBox;
    QLabel *qlTitle;
    QLabel *qlBody;
    QLabel *qlHotkey;
    QLabel *qlLang;
    QLabel *qlTimeout;
    QGroupBox *qgbSysLang;
    QComboBox *qcbxSysLang;

    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

};
#endif // IMEKEY_H
