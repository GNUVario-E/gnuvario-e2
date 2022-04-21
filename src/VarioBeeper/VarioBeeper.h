#pragma once

#include "Arduino.h"
#include "toneAC.h"
#include "VarioTone.h"


#define BEEPER_SIZE 12
struct VARIOCYCLE
{
    uint16_t freq;
    uint16_t cycle;
    uint16_t dutty;
    float_t ms;
};

struct VARIOBEEP
{
    float_t msIs = 0.00;
    float_t msShould = 0.00;
    uint16_t cycle = 0;     // full cycle time in ms
    uint16_t duty = 0;      // duty cycle in %
    uint16_t frequency = 0; // frequency
    float_t climbStep = 0.1;
    uint32_t timeToneOn = 0; // time when cycle started
    bool isSilent = true;
    bool cycleIsOn = false; // currently in cycle
    bool duttyIsOn = false; // currently in dutty part of cycle
};

class VarioBeeper : public VarioTone
{
private:
    VARIOBEEP vario;
    VARIOCYCLE getToneFromMs(float_t m);

    float _msclimb[BEEPER_SIZE] = {-10.00, -3.00, -2.00, -1.00, -0.30, 0.10, 0.50, 1.00, 2.00, 3.00, 5.00, 10.00};
    uint16_t _hertz[BEEPER_SIZE] = {200, 293, 369, 440, 475, 493, 550, 595, 675, 745, 880, 1108};
    uint16_t _cycle[BEEPER_SIZE] = {200, 200, 200, 200, 600, 600, 550, 500, 400, 310, 250, 200};
    uint16_t _dutty[BEEPER_SIZE] = {100, 100, 100, 100, 100, 50, 50, 50, 50, 50, 50, 50};
    uint16_t _zerotageCycleLow = 1000;
    uint16_t _zerotageCycleHigh = 450;
    uint16_t _zerotageDutty = 5;

    // parametrage du zerotage
    // si activé, surcharge le fonctionnement normal
    bool _withZerotage = true;

    bool _isPreviousToneIsZerotage = false;

    // seuil de déclenchement
    float_t _climbToneOnThreshold = 0.30;
    float_t _climbToneOffThreshold = 0.20;
    float_t _zerotageLow = -0.50;
    float_t _sinkToneOnThreshold = -1.0;
    float_t _sinkToneOffThreshold = -0.9;

    // a implementer pour compatibilité
    bool _nearClimbingAlarm = false;
    bool _nearClimbingBeep = false;

    uint16_t getDutty(float_t climb);
    uint16_t getCycle(float_t climb);
    uint16_t getFrequency(float_t climb);
    uint16_t getFromArray(float_t climb, uint16_t *myArray);

    bool isZerotage(float_t climb);

    static void startTaskImpl(void *);
    void task();

    void startTone(float_t frequency, float_t cycle, float_t duty);
    void stopTone();

public:
    VarioBeeper();
    void startTask();
    void setVelocity(float_t climb);
    void toggleZerotage();
    bool isWithZerotage();

    void setGlidingAlarmState(bool state);
    void setGlidingBeepState(bool state);
};

extern VarioBeeper beeper;
