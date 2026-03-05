#include "imekey.h"

void printhelp(char* argv0) { (void)argv0;
    qDebug() << "usage: \n";
    qDebug() << "\t-time=<value> time of switch IME to en-US when system idle, mS. ex: 3000mS at -time=3000\n";
    qDebug() << "\t-lang=<value> IME code what wait to reset. ex: 0x0409(en-US) at -lang=0x04090409 \n";
}


int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(imekey);
    int i;
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
    printhelp(*argv);
    for(i=0; i<argc; i++) {
        char* ptr = argv[i];
        char* brk = NULL;
        if(!ptr) break;
        brk = strchr(ptr, '=');
        if(brk && strncmp("-time=", ptr, 6) == 0) { w.setIdleOut(strtol(brk + 1, NULL, 10)); }
        if(brk && strncmp("-lang=", ptr, 6) == 0) { w.setTgtLang(brk+1); }
    }

    w.show();
    return a.exec();
}
