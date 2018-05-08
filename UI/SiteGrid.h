#ifndef SITEGRID_H
#define SITEGRID_H

#include <QObject>
#include "GridDivision.h"
#include "Location.h"
#include "JsonSerializable.h"

class SiteGrid : public QObject, public JsonSerializable
{
    Q_OBJECT
    Q_PROPERTY(GridDivision* latitude READ getLatitudePtr CONSTANT)
    Q_PROPERTY(GridDivision* longitude READ getLongitudePtr CONSTANT)

public:
    explicit SiteGrid(QObject *parent = nullptr);

    GridDivision& latitude();
    GridDivision& longitude();

signals:

public slots:

private:
    GridDivision m_latitude;
    GridDivision m_longitude;
    GridDivision* getLatitudePtr();
    GridDivision* getLongitudePtr();

    // JsonSerializable interface
public:
    QJsonObject getJson();
};

#endif // SITEGRID_H
