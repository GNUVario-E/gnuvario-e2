
#include "VarioBeeper.h"
#include "VarioDebug/VarioDebug.h"

VarioBeeper::VarioBeeper()
{
}

void VarioBeeper::setGlidingAlarmState(bool state)
{
    _nearClimbingAlarm = state;
}

void VarioBeeper::setGlidingBeepState(bool state)
{
    _nearClimbingBeep = state;
}

VARIOCYCLE VarioBeeper::getToneFromMs(float_t m)
{
    VARIOCYCLE tone;

    tone.ms = m;
    tone.cycle = getCycle(m);
    tone.dutty = getDutty(m);
    tone.freq = getFrequency(m);

    return tone;
}

uint16_t VarioBeeper::getDutty(float_t climb)
{
    if (isZerotage(climb))
    {
        return _zerotageDutty;
    }

    return getFromArray(climb, _dutty);
}

uint16_t VarioBeeper::getCycle(float_t climb)
{
    if (isZerotage(climb))
    {
        return _zerotageCycleLow - ((-_zerotageLow - climb) * (_zerotageCycleHigh - _zerotageCycleLow) / (_climbToneOnThreshold - _zerotageLow));
    }

    return getFromArray(climb, _cycle);
}

uint16_t VarioBeeper::getFrequency(float_t climb)
{
    return getFromArray(climb, _hertz);
}

bool VarioBeeper::isZerotage(float_t climb)
{
    return (_withZerotage && climb >= _zerotageLow && climb <= _climbToneOnThreshold) ? true : false;
}

uint16_t VarioBeeper::getFromArray(float_t climb, uint16_t *myArray)
{
    uint16_t before = 0, after;
    float nbMs;
    int16_t nbByMs = 1;

    for (int i = 0; i < BEEPER_SIZE; i++)
    {
        if (climb < _msclimb[i])
        {
            before = myArray[i - 1];
            after = myArray[i];
            nbMs = _msclimb[i] - _msclimb[i - 1];
            nbByMs = (after - before) / nbMs;

            return before + (climb - _msclimb[i - 1]) * nbByMs;
        }
    }

    return before + (climb - _msclimb[BEEPER_SIZE - 1]) * nbByMs;
}

void VarioBeeper::startTask()
{
    // task creation
    VARIO_PROG_DEBUG_PRINTLN("TaskVarioBeeper started");
    xTaskCreate(this->startTaskImpl, "TaskVarioBeeper", 1000, this, 10, &_taskVarioBeeperHandle);
}

void VarioBeeper::startTaskImpl(void *parm)
{
    VARIO_SOUND_DEBUG_PRINTLN("START Task VarioBeeper");

    // wrapper for task
    static_cast<VarioBeeper *>(parm)->task();
}

void VarioBeeper::task()
{
    enableAmp();

    while (1)
    {

        // play beep based on climb rate
        if (vario.msIs > vario.msShould)
        {
            vario.msIs -= vario.climbStep;
            vario.msIs = max(vario.msIs, vario.msShould);
        }
        else if (vario.msIs < vario.msShould)
        {
            vario.msIs += vario.climbStep;
            vario.msIs = min(vario.msIs, vario.msShould);
        }

        // check the thresholds
        if (vario.isSilent)
        {
            if ((_withZerotage && isZerotage(vario.msIs)) || vario.msIs >= _climbToneOnThreshold || vario.msIs <= _sinkToneOnThreshold)
            {
                vario.isSilent = false;
            }
        }
        else
        {
            if ((!_withZerotage && vario.msIs >= _sinkToneOffThreshold && vario.msIs <= _climbToneOffThreshold) || (_withZerotage && vario.msIs >= _sinkToneOffThreshold && vario.msIs <= _zerotageLow))
            {
                vario.isSilent = true;
            }
        }

        if (vario.isSilent || isMute())
        {
            stopTone();
        }
        else
        {
            if (_withZerotage && _isPreviousToneIsZerotage && !isZerotage(vario.msIs))
            {
                //on sort de la zone de zerotage, ce doit etre immédiat
                enableAmp();
                vario.timeToneOn = 0;
            }
            VARIOCYCLE tCycle = getToneFromMs(vario.msIs);
            startTone(tCycle.freq, tCycle.cycle, tCycle.dutty);
            _isPreviousToneIsZerotage = isZerotage(vario.msIs);
        }

        // give time to other tasks
        vTaskDelay(delayT10);
    }
}

void VarioBeeper::startTone(float_t frequency, float_t cycle, float_t duty)
{
    if (frequency == 0 || duty == 0 || isMute())
        return;

    if (vario.frequency != frequency)
    {
        vario.frequency = frequency;
    }

    uint32_t now = millis();

    if (vario.timeToneOn == 0 && vario.duty > 0)
    {
        //demarrage du cycle
        enableAmp();
        vario.cycleIsOn = true;
        vario.duttyIsOn = false;
        vario.timeToneOn = now; //date de demarrage du cycle
        toneAC(vario.frequency, getVolume());
    }
    else if (vario.cycleIsOn)
    {
        //si le cycle est terminé
        if ((now - vario.timeToneOn) >= vario.cycle)
        {
            vario.timeToneOn = 0;
            vario.cycleIsOn = false;
            vario.duttyIsOn = false;
        }
        else
        {
            //si la partie jouée du cycle est terminée
            if ((now - vario.timeToneOn) >= (vario.cycle * vario.duty / 100.00))
            {
                if (vario.duttyIsOn)
                {
                    //redemarrage de l'ampli pour le prochain cycle
                    if ((vario.cycle - (now - vario.timeToneOn)) < 80)
                    {
                        enableAmp();
                    }
                }
                else
                {
                    vario.duttyIsOn = true;
                    toneAC(30000, getVolume());
                    noToneAC();
                    disableAmp();
                }
            }
            else
            {
                // on est en cours de cycle, on change la frequence
                toneAC(vario.frequency, getVolume());
            }
        }
    }

    // on met a jour avec les valeurs du cycle en cours
    vario.duty = duty;
    vario.cycle = cycle;
}

void VarioBeeper::stopTone()
{
    vario.frequency = 0;
    vario.timeToneOn = 0;
    // pour supprimer le "tac"
    toneAC(30000, getVolume());
    
    noToneAC();
    disableAmp();
}

void VarioBeeper::setVelocity(float_t climb)
{
    if (climb < -10)
    {
        climb = -10;
    }
    else if (climb > 10)
    {
        climb = 10;
    }

    vario.msShould = climb;
}

void VarioBeeper::toggleZerotage()
{
    _withZerotage = !_withZerotage;
}

bool VarioBeeper::isWithZerotage()
{
    return _withZerotage;
}