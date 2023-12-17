QT = httpserver sql testlib

CONFIG += c++17 cmdline
CONFIG += testcase
CONFIG += qtestcase_loop
CONFIG += qtestlib_coverage


QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
QMAKE_LFLAGS += -lgcov --coverage

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        backendtest.cpp \
        main.cpp \
        server.cpp \
        sql.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    backendtest.h \
    libs.h \
    server.h \
    sql.h

RESOURCES += \
    src.qrc

TARGET = my_program
