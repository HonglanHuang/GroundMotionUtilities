#-------------------------------------------------
#
# Project created by QtCreator 2018-04-13T12:12:24
#
#-------------------------------------------------

QT       += core gui qml quick quickcontrols2 charts positioning

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GMWidget
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        GMWidget.cpp \
    Location.cpp \
    Site.cpp \
    PointSourceRupture.cpp \
    RuptureLocation.cpp \
    GMPE.cpp \
    IntensityMeasure.cpp \
    SiteWidget.cpp \
    RuptureWidget.cpp \
    GMPEWidget.cpp \
    IntensityMeasureWidget.cpp \
    RecordSelectionConfig.cpp \
    RecordSelectionWidget.cpp \
    ScenarioProcessor.cpp \
    SiteResult.cpp \
    HBoxFormLayout.cpp \
    SiteGrid.cpp \
    GridDivision.cpp \
    SiteConfig.cpp \
    SiteConfigWidget.cpp \
    SiteGridWidget.cpp \
    SiteResultsModel.cpp \
    LocationsListModel.cpp \
    ../../SimCenterCommon/Common/FooterWidget.cpp \
    GmAppConfig.cpp \
    GmAppConfigWidget.cpp \
    GmCommon.cpp \
    SAResult.cpp \
    PGAResult.cpp \
    RecordSelection.cpp \
    SiteResultsWidget.cpp \
    NGARecordsDb.cpp

HEADERS += \
        GMWidget.h \
    Location.h \
    Site.h \
    PointSourceRupture.h \
    RuptureLocation.h \
    GMPE.h \
    IntensityMeasure.h \
    SiteWidget.h \
    RuptureWidget.h \
    GMPEWidget.h \
    IntensityMeasureWidget.h \
    JsonSerializable.h \
    RecordSelectionConfig.h \
    RecordSelectionWidget.h \
    ScenarioProcessor.h \
    SiteResult.h \
    HBoxFormLayout.h \
    SiteGrid.h \
    GridDivision.h \
    SiteConfig.h \
    SiteConfigWidget.h \
    SiteGridWidget.h \
    SiteResultsModel.h \
    LocationsListModel.h \
    ../../SimCenterCommon/Common/FooterWidget.h \
    GmAppConfig.h \
    GmAppConfigWidget.h \
    GmCommon.h \
    SAResult.h \
    PGAResult.h \
    RecordSelection.h \
    SiteResultsWidget.h \
    NGARecordsDb.h

FORMS +=

RESOURCES += \
    qml.qrc \
    ui.qrc \
    ../../SimCenterCommon/Common/images.qrc
