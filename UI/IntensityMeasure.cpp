#include <QtAlgorithms>
#include <QJsonArray>
#include "IntensityMeasure.h"

IntensityMeasure::IntensityMeasure(QObject *parent) : QObject(parent)
{
    this->setType("SA");
    m_periods << 0.05 << 0.1 << 0.2 << 0.5 << 1.0 << 2.0 << 5.0 << 10.0;
}

QString IntensityMeasure::type() const
{
    return m_type;
}

bool IntensityMeasure::setType(const QString &type)
{
    if(m_type != type && validTypes().contains(type, Qt::CaseInsensitive))
    {
        m_type = type;
        emit typeChanged(m_type);
        return true;
    }
    return false;
}

QJsonObject IntensityMeasure::getJson()
{
    QJsonObject im;
    im.insert("Type", m_type);
    if(m_type == "SA")
    {
        QJsonArray periods;
        foreach(double period, m_periods)
            periods.append(period);
        im.insert("Periods", periods);
    }
    im.insert("EnableJsonOutput", true);
    im.insert("EnableGeoJsonOutput", true);

    return im;
}

const QStringList &IntensityMeasure::validTypes()
{
    static QStringList validTypes = QStringList()
            << "PGA"
            << "SA";

    return validTypes;
}

QList<double> IntensityMeasure::periods() const
{
    return m_periods;
}

void IntensityMeasure::setPeriods(const QList<double> &periods)
{
    m_periods = periods;
}

void IntensityMeasure::addPeriod(double period)
{
    this->m_periods.append(period);
    qSort(this->m_periods);
}
