QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console
QMAKE_CXXFLAGS_WARN_ON += /std:c++latest

SOURCES += \
    agepage.cpp \
#    conclusionpage.cpp \
    intentofcomingpage.cpp \
    main.cpp \
    saveevent.cpp \
    statisticswizard.cpp

HEADERS += \
    agepage.h \
#    conclusionpage.h \
    intentofcomingpage.h \
    saveevent.h \
    statisticswizard.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    agepage.ui \
    intentofcomingpage.ui
