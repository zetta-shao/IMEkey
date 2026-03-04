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


#ifdef WIN32
#define WINVER _WIN32_WINNT_VISTA
#include <windows.h>
#include <winuser.h>
#include <winnls.h>
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
    void createIconGroupBox();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void showMessage();
    void messageClicked();

private:
    void createSysLangGroupBox();
    void createTrayIcon();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    Ui::IMEkey *ui;
    uint32_t p_curHKL;
    uint32_t *p_aryHKL;
    uint32_t p_numHKL;

    QGroupBox *qgbICON;
    QLabel *qlICON;
    QComboBox *qcbxICON;

    QGroupBox *messageGroupBox;
    QLabel *qlTitle;
    QLabel *qlBody;
    QLabel *qlHotkey;
    QLabel *qlLang;
    QLabel *qlTimeout;
    QGroupBox *qgbSysLang;
    QComboBox *qcbSysLang;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

};
#endif // IMEKEY_H
