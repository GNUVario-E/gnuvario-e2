#include "VarioTone.h"

VarioTone::VarioTone()
{
}

void VarioTone::init(uint8_t baseVolume)
{
    _volume = baseVolume;
    fc.setSoundVolume(_volume);
}

void VarioTone::generateTone(uint32_t fHz, int ms)
{
    if (_taskVarioBeeperHandle != NULL)
    {
        vTaskSuspend(_taskVarioBeeperHandle);

        VARIO_SOUND_DEBUG_PRINTLN("suspend task");
    }
    enableAmp();
    vTaskDelay(delayT10 * 5); // time for amp to get on
    toneAC(fHz, _volume, ms);
    disableAmp();
    if (_taskVarioBeeperHandle != NULL)
    {
        vTaskResume(_taskVarioBeeperHandle);
    }
}

void VarioTone::generateTone(uint32_t fHz, int ms, uint8_t volume)
{
    if (_taskVarioBeeperHandle != NULL)
    {
        vTaskSuspend(_taskVarioBeeperHandle);

        VARIO_SOUND_DEBUG_PRINTLN("suspend task");
    }
    enableAmp();
    vTaskDelay(delayT10 * 5); // time for amp to get on
    toneAC(fHz, volume, ms);
    disableAmp();
    if (_taskVarioBeeperHandle != NULL)
    {
        vTaskResume(_taskVarioBeeperHandle);
    }
}

void VarioTone::generateToneSuccess()
{
    if (_taskVarioBeeperHandle != NULL)
    {
        vTaskSuspend(_taskVarioBeeperHandle);

        VARIO_SOUND_DEBUG_PRINTLN("suspend task");
    }
    generateTone(220, 100);
    delay(50);
    generateTone(440, 100);
    delay(50);
    generateTone(880, 100);

    if (_taskVarioBeeperHandle != NULL)
    {
        vTaskResume(_taskVarioBeeperHandle);
    }
}

void VarioTone::generateToneSoftFailure()
{
    if (_taskVarioBeeperHandle != NULL)
    {
        vTaskSuspend(_taskVarioBeeperHandle);

        VARIO_SOUND_DEBUG_PRINTLN("suspend task");
    }

    uint16_t fHz = 300;
    enableAmp();
    delay(80);

    while (fHz > 150)
    {
        toneAC(fHz, _volume);
        fHz -= 5;
        vTaskDelay(delayT10); // time for amp to get on
    }

    disableAmp();

    if (_taskVarioBeeperHandle != NULL)
    {
        vTaskResume(_taskVarioBeeperHandle);
    }
}

void VarioTone::generateToneFailure()
{
    if (_taskVarioBeeperHandle != NULL)
    {
        vTaskSuspend(_taskVarioBeeperHandle);

        VARIO_SOUND_DEBUG_PRINTLN("suspend task");
    }

    uint16_t fHz = 300;
    enableAmp();
    delay(80);

    while (fHz > 150)
    {
        toneAC(fHz, _volume);
        fHz--;
        vTaskDelay(delayT10); // time for amp to get on
    }

    disableAmp();

    if (_taskVarioBeeperHandle != NULL)
    {
        vTaskResume(_taskVarioBeeperHandle);
    }
}

void VarioTone::setVolume(uint8_t newVolume)
{
    if (_volume < 0)
    {
        _volume = 0;
    }
    else if (_volume > 10)
    {
        _volume = 10;
    }
    else
    {
        _volume = newVolume;
    }

    fc.setSoundVolume(_volume);
}

void VarioTone::increaseVolume()
{
    setVolume(_volume + 1);
}

void VarioTone::decreaseVolume()
{
    setVolume(_volume - 1);
}

uint8_t VarioTone::getVolume()
{
    return _volume;
}

void VarioTone::mute()
{
    noToneAC();
    _muted = true;
    disableAmp();
    fc.setSoundIsMute(_muted);
}

void VarioTone::unMute()
{
    _muted = false;
    enableAmp();
    fc.setSoundIsMute(_muted);
}

bool VarioTone::isMute(void)
{
    return _muted;
}

void VarioTone::enableAmp()
{
    if (!_isAmpOn)
    {
        pinMode(PIN_AUDIO_AMP_ENA, OUTPUT);
        digitalWrite(PIN_AUDIO_AMP_ENA, HIGH);
        _isAmpOn = true;
    }
};

void VarioTone::disableAmp()
{
    if (_isAmpOn)
    {
        pinMode(PIN_AUDIO_AMP_ENA, OUTPUT);
        digitalWrite(PIN_AUDIO_AMP_ENA, LOW);
        _isAmpOn = false;
    }
}
