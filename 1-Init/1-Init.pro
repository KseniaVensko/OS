TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

configPr1 {
TARGET = pr1
SOURCES += pr1.c
}

configMain {
TARGET = main
SOURCES += main.c
}

SOURCES += log.c \
    read_files.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    log.h \
    read_files.h

