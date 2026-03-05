#ifndef IMEKEY_H
#define IMEKEY_H

#include "common.h"

class clsHKL;

enum def_flags {
    //os: low 3bit
    flag_os_unknow = 0x0,
    flag_os_win32 = 0x1,
    flag_os_linux_tty = 0x2,
    flag_os_linux_x11 = 0x3,
    flag_os_linux_wayland = 0x4,
};

class IMEkey : public QDialog
{
    Q_OBJECT

public:
    IMEkey(QWidget *parent = nullptr);
    ~IMEkey();
    void initIMEkey(void); //in _impl_ file
    void setVisible(bool visible) override;
    void createTimerGroupBox();
    void createSysLangGroupBox();
    void createTrayIcon();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void setIdleOut(int32_t val);
    void setTgtLang(char *pSTR);

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
    clsHKL *p_HKL;
    uint32_t p_flags;
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
