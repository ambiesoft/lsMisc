TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG += qt
QT += core gui
QT += widgets

INCLUDEPATH += \
		googletest/googletest \
		googletest/googletest/include \


SOURCES += \
    ../stdQt/stdQt.cpp \
    gtestStdQt.cpp \
    gtestStdosd_literal.cpp \
        main.cpp \
    googletest/googletest/src/gtest-all.cc \
    gtestStdosd.cpp \
    gtestOpParser.cpp \
    ../stdosd/stdosd.cpp \




HEADERS += \
    ../stdQt/stdQt.h \
    ../stdosd/OpParser.h \
    ../stdosd/PathUtil.h \
    ../stdosd/stdosd.h \
    ../stdosd/stdosd_literal.h

win32 {
    message( "win32 sources and libs" )

    DEFINES += NOMINMAX

    HEADERS += \
        ../GetLastErrorString.h

    SOURCES += \
        ../stdosd/PathUtil_win.cpp \
        gtestPathUtil_win.cpp \
        ../stdosd/stdosd_win.cpp \
        ../stdQt/stdQt_win32.cpp \
        ../GetLastErrorString.cpp

    win32-g++ {
		message( "win32-g++ libs" )
        LIBS += -lOle32 -lShlwapi -luuid
    }
    win32-msvc* {
		message( "win32-msvc* libs" )
        LIBS += Ole32.lib Shlwapi.lib uuid.lib shell32.lib
    }
}
linux-g++ {
	message( "linux-g++ sources and libs" )
    SOURCES += \
        ../stdosd/PathUtil_linux.cpp \
        gtestPathUtil_linux.cpp \
        ../stdosd/stdosd_linux.cpp \
        ../stdQt/stdQt_linux.cpp

    LIBS += -lpthread

}
