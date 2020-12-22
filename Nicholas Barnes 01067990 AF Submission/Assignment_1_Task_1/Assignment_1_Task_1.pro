QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opencvtest
TEMPLATE = app

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += C:\opencv\build\include

LIBS += C:\opencv\build\bin\libopencv_core411.dll
LIBS += C:\opencv\build\bin\libopencv_highgui411.dll
LIBS += C:\opencv\build\bin\libopencv_imgcodecs411.dll
LIBS += C:\opencv\build\bin\libopencv_imgproc411.dll
LIBS += C:\opencv\build\bin\libopencv_features2d411.dll
LIBS += C:\opencv\build\bin\libopencv_calib3d411.dll

#INCLUDEPATH += $$(OPENCV_SDK_DIR)/include

#LIBS += -L$$(OPENCV_SDK_DIR)/x86/mingw/lib \
#        -lopencv_core411        \
#        -lopencv_highgui411     \
#        -lopencv_imgcodecs411   \
#        -lopencv_imgproc411     \
#        -lopencv_features2d411  \
#        -lopencv_calib3d411

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
