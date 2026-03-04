#include "imekey.h"

#include <QMessageBox>
#include <QApplication>

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(imekey);
    int idx;

    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication a(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, QObject::tr("Systray"),
                              QObject::tr("I couldn't detect any system tray "
                                          "on this system."));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);
    IMEkey w;

    for(idx=0; idx<argc; idx++) {
        char *ptr=argv[idx];
        char *brk=strchr(ptr, '=');
        if(brk && strncmp("-time=", ptr, 6)==0) { w.setIdleTimerVal(strtol(brk+1,NULL,10)); }
        if(brk && strncmp("-lang=", ptr, 6)==0) { w.setIdleTimerVal(strtol(brk+1,NULL,16)); }
    }

    w.show();
    return a.exec();
}
