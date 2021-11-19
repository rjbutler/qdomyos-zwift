#ifndef TRAINPROGRAM_H
#define TRAINPROGRAM_H
#include "bluetooth.h"
#include <QGeoCoordinate>
#include <QObject>
#include <QTime>
#include <QTimer>

class trainrow {
  public:
    QTime duration = QTime(0, 0, 0, 0);
    double speed = -1;
    double fanspeed = -1;
    double inclination = -200;
    int8_t resistance = -1;
    int8_t lower_resistance = -1;
    int8_t upper_resistance = -1;
    int8_t requested_peloton_resistance = -1;
    int8_t lower_requested_peloton_resistance = -1;
    int8_t upper_requested_peloton_resistance = -1;
    int16_t cadence = -1;
    int16_t lower_cadence = -1;
    int16_t upper_cadence = -1;
    bool forcespeed = false;
    int8_t loopTimeHR = 10;
    int8_t zoneHR = -1;
    int8_t maxSpeed = -1;
    int32_t power = -1;
    int32_t mets = -1;
    double latitude = NAN;
    double longitude = NAN;
};

class trainprogram : public QObject {
    Q_OBJECT

  public:
    trainprogram(const QList<trainrow> &, bluetooth *b);
    void save(const QString &filename);
    static trainprogram *load(const QString &filename, bluetooth *b);
    static QList<trainrow> loadXML(const QString &filename);
    static bool saveXML(const QString &filename, const QList<trainrow> &rows);
    QTime totalElapsedTime();
    QTime currentRowElapsedTime();
    QTime currentRowRemainingTime();
    double currentTargetMets();
    QTime duration();
    double totalDistance();
    trainrow currentRow();
    void increaseElapsedTime(uint32_t i);
    void decreaseElapsedTime(uint32_t i);
    int32_t offsetElapsedTime() { return offset; }

    QList<trainrow> rows;
    QList<trainrow> loadedRows; // rows as loaded
    bool enabled = true;

    void restart();
    void scheduler(int tick);

  public slots:
    void onTapeStarted();
    void scheduler();

  signals:
    void start();
    void stop();
    void changeSpeed(double speed);
    bool changeFanSpeed(uint8_t speed);
    void changeInclination(double grade, double inclination);
    void changeResistance(int8_t resistance);
    void changeRequestedPelotonResistance(int8_t resistance);
    void changeCadence(int16_t cadence);
    void changePower(int32_t power);
    void changeSpeedAndInclination(double speed, double inclination);
    void changeGeoPosition(QGeoCoordinate p);

  private:
    uint32_t calculateTimeForRow(int32_t row);
    bluetooth *bluetoothManager;
    bool started = false;
    int32_t ticks = 0;
    uint16_t currentStep = 0;
    int32_t offset = 0;
    QTimer timer;
};

#endif // TRAINPROGRAM_H
