
INCLUDEPATH += /usr/local/include \
                 /usr/include \
                /usr/local/include/opencv \
                /usr/local/include/opencv2


LIBS += /usr/lib/i386-linux-gnu/libv4l2rds.so    \
        /usr/lib/i386-linux-gnu/libv4lconvert.so    \
        /usr/lib/i386-linux-gnu/libv4l2.so    \
        /usr/lib/i386-linux-gnu/libv4l1.so    \
 /usr/local/lib/libopencv_highgui.so\
 /usr/local/lib/libopencv_core.so    \
 /usr/local/lib/libopencv_imgproc.so\
 /usr/local/lib/libopencv_calib3d.so

SOURCES += \
    main.cpp \
    sdk_lib/DJI_Control.cpp \
    sdk_lib/DJI_Imag_Dect.cpp \
    sdk_lib/DJI_Pro_App.cpp \
    sdk_lib/DJI_Pro_Codec.cpp \
    sdk_lib/DJI_Pro_Hw.cpp \
    sdk_lib/DJI_Pro_Link.cpp \
    sdk_lib/DJI_Pro_Test.cpp \
    sdk_lib/my_serial_lib.cpp

HEADERS += \
    sdk_lib/DJI_Control.h \
    sdk_lib/DJI_Imag_Dect.h \
    sdk_lib/DJI_Pro_App.h \
    sdk_lib/DJI_Pro_Codec.h \
    sdk_lib/DJI_Pro_Hw.h \
    sdk_lib/DJI_Pro_Link.h \
    sdk_lib/DJI_Pro_Test.h \
    sdk_lib/my_serial_lib.h
