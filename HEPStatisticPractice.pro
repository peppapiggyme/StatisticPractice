QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ExDef.cpp \
        ExUtils.cxx \
        Excercise_1.cpp \
        Excercise_2.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ExDef.h \
    ExUtils.h \
    ExcerciseBase.h \
    Excercise_1.h \
    Excercise_2.h \
    Utils.h

# install gsl
LIBS += -L/usr/local/Cellar/gsl/2.6/lib/ -lgsl -lgslcblas

INCLUDEPATH += /usr/local/Cellar/gsl/2.6/include
DEPENDPATH += /usr/local/Cellar/gsl/2.6/include
PRE_TARGETDEPS += /usr/local/Cellar/gsl/2.6/lib/libgsl.a
PRE_TARGETDEPS += /usr/local/Cellar/gsl/2.6/lib/libgslcblas.a

# install ROOT
LIBS += -L/usr/local/Cellar/root/6.22.00_1/lib/root -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lROOTDataFrame -stdlib=libc++ -lpthread -lm -ldl
INCLUDEPATH += /usr/local/Cellar/root/6.22.00_1/include/root/
