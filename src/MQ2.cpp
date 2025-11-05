#include "MQ2.h"
#include <math.h>

MQ2::MQ2(uint8_t pin, float rl_value, float ro_clean_air_factor)
    : _pin(pin), _RL_VALUE(rl_value), _RO_CLEAN_AIR_FACTOR(ro_clean_air_factor), _Ro(10.0) {}

void MQ2::calibrate() {
    Serial.println("Calibrating MQ2...");
    float val = 0;
    for (int i = 0; i < 50; i++) {
        val += sensorResistance(analogRead(_pin));
        delay(100);
    }
    _Ro = (val / 50.0) / _RO_CLEAN_AIR_FACTOR;
    Serial.print("Calibration done. Ro = ");
    Serial.print(_Ro);
    Serial.println(" kΩ");
}

float MQ2::sensorResistance(int raw) {
    // ESP32 ADC is 12-bit (0-4095)
    return _RL_VALUE * ((4095.0 - raw) / (float)raw);
}

float MQ2::readSensor() {
    float val = 0;
    for (int i = 0; i < 5; i++) {
        val += sensorResistance(analogRead(_pin));
        delay(50);
    }
    return val / 5.0;
}

float MQ2::readRatio() {
    return readSensor() / _Ro;
}

int MQ2::getGas(float ratio, float *curve) {
    return pow(10, ((log(ratio) - curve[1]) / curve[2]) + curve[0]);
}

int MQ2::getLPG()   { return getGas(readRatio(), LPGCurve); }
int MQ2::getCO()    { return getGas(readRatio(), COCurve); }
int MQ2::getSmoke() { return getGas(readRatio(), SmokeCurve); }
