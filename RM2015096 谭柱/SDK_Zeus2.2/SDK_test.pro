#-------------------------------------------------
#
# Project created by QtCreator 2015-07-25T13:07:02
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = SDK_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

#INCLUDEPATH += /usr/include/libusb-1.0

SOURCES += main.cpp \
    sdk_lib/DJI_Pro_App.cpp \
    sdk_lib/DJI_Pro_Codec.cpp \
    sdk_lib/DJI_Pro_Hw.cpp \
    sdk_lib/DJI_Pro_Link.cpp \
    sdk_lib/DJI_Pro_Test.cpp \
    sdk_lib/DJI_Control.cpp \
    sdk_lib/Tan_Display.cpp \
    sdk_lib/ComputerVision.cpp \
    sdk_lib/Logic_CV_Thread.cpp \
    sdk_lib/serial.cpp

HEADERS += \
    sdk_lib/DJI_Pro_App.h \
    sdk_lib/DJI_Pro_Codec.h \
    sdk_lib/DJI_Pro_Hw.h \
    sdk_lib/DJI_Pro_Link.h \
    sdk_lib/DJI_Pro_Test.h \
    sdk_lib/DJI_Control.h \
    sdk_lib/Tan_Display.h \
    sdk_lib/CV_header.h \
    sdk_lib/Logic_CV_Thread.h \
    sdk_lib/serial.h

INCLUDEPATH   += "/opt/halcon/include"
INCLUDEPATH   += "/opt/halcon/include/halconcpp"
INCLUDEPATH   += "/opt/halcon/include/cpp"
INCLUDEPATH += /usr/local/include \
            /usr/local/include/opencv \
            /usr/local/include/opencv2

#LIBS += /usr/local/lib/libopencv_highgui.so \
 #   /usr/local/lib/libopencv_core.so    \
  #  /usr/local/lib/libopencv_imgproc.so
#libs
QMAKE_LIBDIR  += "/opt/halcon/lib/x64-linux"
#unix:
LIBS          = -L/opt/halcon/lib/x64-linux\
     -L/usr/X11R6/lib64 -lhalconcpp -lhalcon -lXext -lX11 -ldl -lQt5Widgets \
    -L/usr/lib/x86_64-linux-gnu -lQt5Gui -lQt5Core -lGL -lpthread -lopencv_core -lopencv_highgui -lv4l2rds -lv4lconvert -lv4l2 -lv4l1
