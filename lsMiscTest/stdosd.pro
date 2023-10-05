TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG += qt
QT += core gui
QT += widgets

greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

INCLUDEPATH += \
		googletest/googletest \
		googletest/googletest/include \


SOURCES += \
    ../stdQt/stdQt.cpp \
    ../stdosd/DetectVM.cpp \
    gtestStdQt.cpp \
    gtestStdosd_literal.cpp \
        main.cpp \
    googletest/googletest/src/gtest-all.cc \
    gtestStdosd.cpp \
    gtestOpParser.cpp \
    ../stdosd/stdosd.cpp \

HEADERS += \
    ../stdQt/stdQt.h \
    ../stdosd/DetectVM.h \
    ../stdosd/OpParser.h \
    ../stdosd/PathUtil.h \
    ../stdosd/stdosd.h \
    ../stdosd/stdosd_literal.h

win32 {
    message( "win32 sources and libs" )

    DEFINES += NOMINMAX
    DEFINES += _WIN32_WINNT=0x0601

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
        LIBS += -lKernel32 -lPsapi -lUser32 -lOle32 -lShlwapi -luuid
    }
    win32-msvc* {
        message( "win32-msvc* libs" )
        LIBS += User32.lib Ole32.lib Shlwapi.lib uuid.lib shell32.lib
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
