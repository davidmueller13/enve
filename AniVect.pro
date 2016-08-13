#-------------------------------------------------
#
# Project created by QtCreator 2016-08-06T14:25:20
#
#-------------------------------------------------

QT       += core gui svg opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AniVect
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vectorpath.cpp \
    pathpoint.cpp \
    valueanimators.cpp \
    canvas.cpp \
    undoredo.cpp \
    connectedtomainwindow.cpp \
    movablepoint.cpp \
    pathpivot.cpp \
    canvasmouseinteractions.cpp \
    ctrlpoint.cpp \
    boundingbox.cpp \
    Colors/ColorWidgets/colorlabel.cpp \
    Colors/ColorWidgets/colorsettingswidget.cpp \
    Colors/ColorWidgets/colorvaluerect.cpp \
    Colors/ColorWidgets/colorvaluespin.cpp \
    Colors/ColorWidgets/colorwidget.cpp \
    Colors/ColorWidgets/h_wheel_sv_triangle.cpp \
    Colors/color.cpp \
    Colors/colorpickingwidget.cpp \
    Colors/savedcolorbutton.cpp \
    Colors/savedcolorswidget.cpp \
    Colors/savedcolorwidgets.cpp \
    Colors/helpers.cpp \
    boxesgroup.cpp \
    fillstrokesettings.cpp \
    Colors/ColorWidgets/gradientwidget.cpp \
    Colors/ColorWidgets/glwidget.cpp \
    gradientpoint.cpp

HEADERS  += mainwindow.h \
    vectorpath.h \
    pathpoint.h \
    valueanimators.h \
    canvas.h \
    undoredo.h \
    updatescheduler.h \
    connectedtomainwindow.h \
    movablepoint.h \
    pathpivot.h \
    ctrlpoint.h \
    boundingbox.h \
    Colors/ColorWidgets/colorlabel.h \
    Colors/ColorWidgets/colorsettingswidget.h \
    Colors/ColorWidgets/colorvaluerect.h \
    Colors/ColorWidgets/colorvaluespin.h \
    Colors/ColorWidgets/colorwidget.h \
    Colors/ColorWidgets/h_wheel_sv_triangle.h \
    Colors/color.h \
    Colors/colorpickingwidget.h \
    Colors/savedcolorbutton.h \
    Colors/savedcolorswidget.h \
    Colors/savedcolorwidgets.h \
    Colors/helpers.h \
    boxesgroup.h \
    fillstrokesettings.h \
    Colors/ColorWidgets/gradientwidget.h \
    Colors/ColorWidgets/glwidget.h \
    gradientpoint.h
