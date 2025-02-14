#ifndef TRAINPROGRAM_H
#define TRAINPROGRAM_H
#include "bluetooth.h"
#include <QGeoCoordinate>
#include <QMutex>
#include <QObject>
#include <QSet>
#include <QTime>
#include <QTimer>

class trainrow {
  public:
    QTime duration = QTime(0, 0, 0, 0);
    double distance = -1;
    double speed = -1;
    double lower_speed = -1;   // used for peloton
    double average_speed = -1; // used for peloton
    double upper_speed = -1;   // used for peloton
    double fanspeed = -1;
    double inclination = -200;
    double lower_inclination = -200;   // used for peloton
    double average_inclination = -200; // used for peloton
    double upper_inclination = -200;   // used for peloton
    int8_t resistance = -1;
    int8_t lower_resistance = -1;
    int8_t average_resistance = -1; // used for peloton
    int8_t upper_resistance = -1;
    int8_t requested_peloton_resistance = -1;
    int8_t lower_requested_peloton_resistance = -1;
    int8_t average_requested_peloton_resistance = -1; // used for peloton
    int8_t upper_requested_peloton_resistance = -1;
    int16_t cadence = -1;
    int16_t lower_cadence = -1;
    int16_t average_cadence = -1; // used for peloton
    int16_t upper_cadence = -1;
    bool forcespeed = false;
    int8_t loopTimeHR = 10;
    int8_t zoneHR = -1;
    int8_t maxSpeed = -1;
    int32_t power = -1;
    int32_t mets = -1;
    QTime rampDuration = QTime(0, 0, 0, 0); // QZ split the ramp in 1 second segments. This field will tell you how long
                                            // is the ramp from this very moment
    QTime rampElapsed = QTime(0, 0, 0, 0);
    double latitude = NAN;
    double longitude = NAN;
    double altitude = NAN;
    double azimuth = NAN;
    QString toString() const;
};

class trainprogram : public QObject {
    Q_OBJECT

  public:
    trainprogram(const QList<trainrow> &, bluetooth *b, QString *description = nullptr, QString *tags = nullptr);
    void save(const QString &filename);
    static trainprogram *load(const QString &filename, bluetooth *b);
    static QList<trainrow> loadXML(const QString &filename);
    static bool saveXML(const QString &filename, const QList<trainrow> &rows);
    QTime totalElapsedTime();
    QTime currentRowElapsedTime();
    QTime currentRowRemainingTime();
    QTime remainingTime();
    double currentTargetMets();
    QTime duration();
    double totalDistance();
    trainrow currentRow();
    trainrow getRowFromCurrent(uint32_t offset);
    void increaseElapsedTime(uint32_t i);
    void decreaseElapsedTime(uint32_t i);
    int32_t offsetElapsedTime() { return offset; }
    void clearRows();

    QList<trainrow> rows;
    QList<trainrow> loadedRows; // rows as loaded
    QString description = "";
    QString tags = "";
    bool enabled = true;

    void restart();
    bool isStarted() { return started; }
    void scheduler(int tick);

  public slots:
    void onTapeStarted();
    void scheduler();

  signals:
    void start();
    void stop();
    void lap();
    void changeSpeed(double speed);
    bool changeFanSpeed(uint8_t speed);
    void changeInclination(double grade, double inclination);
    void changeNextInclination300Meters(QList<MetersByInclination>);
    void changeResistance(int8_t resistance);
    void changeRequestedPelotonResistance(int8_t resistance);
    void changeCadence(int16_t cadence);
    void changePower(int32_t power);
    void changeSpeedAndInclination(double speed, double inclination);
    void changeGeoPosition(QGeoCoordinate p, double azimuth, double avgAzimuthNext300Meters);

  private:
    mutable QRecursiveMutex schedulerMutex;
    double avgAzimuthNext300Meters();
    QList<MetersByInclination> inclinationNext300Meters();
    double avgInclinationNext100Meters();
    uint32_t calculateTimeForRow(int32_t row);
    uint32_t calculateTimeForRowMergingRamps(int32_t row);
    double calculateDistanceForRow(int32_t row);
    bluetooth *bluetoothManager;
    bool started = false;
    int32_t ticks = 0;
    uint16_t currentStep = 0;
    int32_t offset = 0;
    double lastOdometer = 0;
    double currentStepDistance = 0;
    QTimer timer;
};

#endif // TRAINPROGRAM_H
