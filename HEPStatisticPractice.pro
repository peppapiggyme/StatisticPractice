QT -= gui

CONFIG += c++11

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
        AtlasOpenData/AnaHyy.cpp \
        ExDef.cpp \
        ExUtils.cpp\
        Exercise_1.cpp \
        Exercise_2.cpp \
        Exercise_3.cpp \
        Exercise_4.cpp \
        Exercise_5.cpp \
        Exercise_6.cpp \
        Exercise_7.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    AtlasOpenData/AnaHyy.h \
    AtlasOpenData/AnaHyyBase.h \
    ExDef.h \
    ExUtils.h \
    ExerciseBase.h \
    Exercise_1.h \
    Exercise_2.h \
    Exercise_3.h \
    Exercise_4.h \
    Exercise_5.h \
    Exercise_6.h \
    Exercise_7.h \
    Utils.h

# For NJU HEP virtual machine
# ---------------------------

# install gsl
LIBS += -L/usr/local/lib -lgsl -lgslcblas
INCLUDEPATH += /usr/local/include/gsl

# install ROOT
LIBS += -L/usr/lib64/root -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lROOTDataFrame -lHistFactory -lRooFit -lRooFitCore -lRooFitMore -lRooStats -lpthread -lm -ldl
INCLUDEPATH += /usr/include/root

# For my MacBook
# --------------

## install gsl
#LIBS += -L/usr/local/Cellar/gsl/2.6/lib -lgsl -lgslcblas
#INCLUDEPATH += /usr/local/Cellar/gsl/2.6/include

## install ROOT
#LIBS += -L/usr/local/lib/root -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lROOTDataFrame -lHistFactory -lRooFit -lRooFitCore -lRooFitMore -lRooStats -stdlib=libc++ -lpthread -lm -ldl
#INCLUDEPATH += /usr/local/Cellar/root/6.22.00_1/include/root
