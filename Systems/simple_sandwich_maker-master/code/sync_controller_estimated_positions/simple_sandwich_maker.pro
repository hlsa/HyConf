QT -= core
QT -= gui

%MAKEFILE = Makefile.gen

VREP_PATH = $$(VREP_PATH)

isEmpty(VREP_PATH) {
	message("The environment variable VREP_PATH does not define the path to VREP. Assuming default");
	VREP_PATH = "/usr/share/vrep/programming"
}

BIN		= bin
SRC		= src
INCLUDE	= include
LIB		= lib

message("VREP_PATH: $$VREP_PATH")

TARGET = $$BIN/simple_sandwich_maker_remoteApi
TEMPLATE = app

DEFINES -= UNICODE
CONFIG   += console
CONFIG   -= app_bundle

DEFINES += NON_MATLAB_PARSING
DEFINES += MAX_EXT_API_CONNECTIONS=255

*-msvc* {
    QMAKE_CXXFLAGS += -O2
    QMAKE_CXXFLAGS += -W3
}
*-g++* {
    QMAKE_CXXFLAGS += -O3
    QMAKE_CXXFLAGS += -Wall
    QMAKE_CXXFLAGS += -Wno-unused-parameter
    QMAKE_CXXFLAGS += -Wno-strict-aliasing
    QMAKE_CXXFLAGS += -Wno-empty-body
    QMAKE_CXXFLAGS += -Wno-write-strings

    QMAKE_CXXFLAGS += -Wno-unused-but-set-variable
    QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
    QMAKE_CXXFLAGS += -Wno-narrowing

    QMAKE_CFLAGS += -O3
    QMAKE_CFLAGS += -Wall
    QMAKE_CFLAGS += -Wno-strict-aliasing
    QMAKE_CFLAGS += -Wno-unused-parameter
    QMAKE_CFLAGS += -Wno-unused-but-set-variable
    QMAKE_CFLAGS += -Wno-unused-local-typedefs
}

win32 {
    LIBS += -lwinmm
    LIBS += -lWs2_32
}

macx {
}

unix:!macx {
    LIBS += -lrt
}


INCLUDEPATH += "$$VREP_PATH/include"
INCLUDEPATH += "$$VREP_PATH/remoteApi"
INCLUDEPATH += "$$INCLUDE"
INCLUDEPATH += "$$INCLUDE/framework"

SOURCES += \
    $$SRC/*.cpp \
    $$SRC/framework/*.cpp \
    $$VREP_PATH/remoteApi/extApi.c \
    $$VREP_PATH/remoteApi/extApiPlatform.c \
    $$VREP_PATH/common/shared_memory.c

HEADERS +=\
    $$VREP_PATH/remoteApi/extApi.h \
    $$VREP_PATH/remoteApi/extApiPlatform.h \
    $$VREP_PATH/common/shared_memory.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

run.commands = clear && ./$$TARGET 8888 19 17 18
#run.depends = clean
run.depends += all
QMAKE_EXTRA_TARGETS += run

doc.commands = cd doc && pdflatex dynamics.tex
doc.depends = FORCE
QMAKE_EXTRA_TARGETS += doc

QMAKE_CLEAN += doc/*.log
QMAKE_CLEAN += doc/*.aux

OBJECTS_DIR = objects


