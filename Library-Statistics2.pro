QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


win32 {
    CONFIG += console
    QMAKE_CXXFLAGS_WARN_ON += /std:c++latest
}
unix {
    QMAKE_CXXFLAGS += -std=c++2a
}
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

FORMS += \
    agepage.ui \
    intentofcomingpage.ui
