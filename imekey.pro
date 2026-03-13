QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += "-fno-sized-deallocation"
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    imekey_impl_lnx.cpp \
    imekey_impl_win32.cpp \
    main.cpp \
    imekey.cpp

HEADERS += \
    common.h \
    imekey.h \
    imekey_impl_lnx.h \
    imekey_impl_win32.h

FORMS += \
    imekey.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
linux: LIBS+= -lX11 -lXss
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    imekey.qrc

DISTFILES += \
    README.md
