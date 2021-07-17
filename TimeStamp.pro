QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    driver.cpp \
    ftdsocket.cpp \
    main.cpp \
    mainwindow.cpp \
    graphwindow.cpp

HEADERS += \
    driver.h \
    ftdsocket.h \
    mainwindow.h \
    graphwindow.h \
    ftd3xx.h


FORMS += \
    mainwindow.ui \
    graphwindow.ui

LIBS += /home/yusuf/Documents/Workspace/qt-workspace/TimeStamp/lib/libftd3xx.so

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
