#ifndef MQ2_H
#define MQ2_H

#include <Arduino.h>

class MQ2 {
public:
    MQ2(uint8_t pin, float rl_value = 2.0, float ro_clean_air_factor = 9.83);

    void calibrate();
    float readRatio();
    int getLPG();
    int getCO();
    int getSmoke();

private:
    uint8_t _pin;
    float _RL_VALUE;
    float _RO_CLEAN_AIR_FACTOR;
    float _Ro;

    float sensorResistance(int raw);
    float readSensor();
    int getGas(float ratio, float *curve);

    float LPGCurve[3]   = {2.3, 0.21, -0.47};
    float COCurve[3]    = {2.3, 0.72, -0.34};
    float SmokeCurve[3] = {2.3, 0.53, -0.44};
};

#endif
