#ifndef GMWIDGET_H
#define GMWIDGET_H

#include <QWidget>
#include "SiteWidget.h"
#include "RuptureWidget.h"
#include "GMPEWidget.h"
#include "IntensityMeasureWidget.h"

namespace Ui {
class GMWidget;
}

class GMWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GMWidget(QWidget *parent = 0);
    ~GMWidget();

private:
    Ui::GMWidget *ui;
    SiteWidget* m_siteWidget;
    Site* m_site;
    RuptureWidget* m_ruptureWidget;
    PointSourceRupture* m_eqRupture;
    GMPE* m_gmpe;
    GMPEWidget* m_gmpeWidget;
    IntensityMeasure* m_intensityMeasure;
    IntensityMeasureWidget* m_intensityMeasureWidget;
};

#endif // GMWIDGET_H
