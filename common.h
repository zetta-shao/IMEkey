#ifndef COMMON_H
#define COMMON_H

#include <QDialog>
#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QCoreApplication>
#include <QApplication>
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
#include <QDebug>
#include <vector>

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

enum def_flags {
    //os: low 3bit
    flag_os_unknow = 0x0,
    flag_os_win32 = 0x1,
    flag_os_linux_tty = 0x2,
    flag_os_linux_x11 = 0x3,
    flag_os_linux_wayland = 0x4,
};

enum def_flag_mask {
    flag_os_mask = 0x7,
};

#endif // COMMON_H
