#ifndef GPX_H
#define GPX_H

#include "bluetoothdevice.h"
#include "sessionline.h"
#include <QFile>
#include <QGeoCoordinate>
#include <QObject>
#include <QTime>

class gpx_altitude_point_for_treadmill {
  public:
    uint32_t seconds = 0;
    float inclination = 0;
    float elevation = 0;
    float speed = 0;
    double distance = 0;
    double latitude = 0;
    double longitude = 0;
};

class gpx_point {
  public:
    QDateTime time;
    QGeoCoordinate p;
};

class gpx : public QObject {
    Q_OBJECT
  public:
    explicit gpx(QObject *parent = nullptr);
    QList<gpx_altitude_point_for_treadmill> open(const QString &gpx);
    static void save(const QString &filename, QList<SessionLine> session, bluetoothdevice::BLUETOOTH_TYPE type);

  private:
    QList<gpx_point> points;

  signals:
};

#endif // GPX_H
