#ifndef VARIO_IMU_H
#define VARIO_IMU_H

class Kalmanvert;

class VarioImu
{
private:
    Kalmanvert *kalmanvert;
    double alti;
    double temp;
    double accel;
    int compteurAccel = 0;

public:
    VarioImu(Kalmanvert *kalmanvert);
    void init();
    void initKalman(double firstAlti);
    bool havePressure(void);
    bool updateData();
    void updateKalman(double mp, double ma, unsigned long timestamp);
    double getAlti();
    double getTemp();
    double getAccel();
    double firstAlti();
};
#endif //VARIO_IMU_H