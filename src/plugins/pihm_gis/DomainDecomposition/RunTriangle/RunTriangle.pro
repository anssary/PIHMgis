######################################################################
# Automatically generated by qmake (2.00a) Tue Feb 20 02:01:40 2007
######################################################################

TEMPLATE = app
TARGET += 
DEPENDPATH += . pihmLIBS pihmLIBS/helpDialog
INCLUDEPATH += . pihmLIBS pihmLIBS/helpDialog
QT += qt3support
# Input
HEADERS += runtriangle.h \
           triangle.h \
           pihmLIBS/createTinShapeFile.h \
           pihmLIBS/generatePolyFile.h \
           pihmLIBS/homeDir.h \
           pihmLIBS/line.h \
           pihmLIBS/mergeFeatures.h \
           pihmLIBS/pickGridValue.h \
           pihmLIBS/polygonToPolyline.h \
           pihmLIBS/shapefil.h \
           pihmLIBS/simplifyPolyline.h \
           pihmLIBS/simplifyPolySHP.h \
           pihmLIBS/splitLineAtVertices.h \
           pihmLIBS/helpDialog/helpdialog.h
FORMS += runtriangle.ui pihmLIBS/helpDialog/helpdialog.ui
SOURCES += main.cpp \
           triangle.cpp \
           pihmLIBS/dbfopen.c \
           pihmLIBS/shpopen.c \
           runtriangle.cpp \
           pihmLIBS/helpDialog/helpdialog.cpp