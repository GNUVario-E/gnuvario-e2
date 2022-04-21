#include "VarioTone.h"

VarioTone::VarioTone()
{
}

void VarioTone::init(uint8_t baseVolume)
{
    _volume = baseVolume;
    fc.sound.volume = _volume;
}

void VarioTone::generateTone(uint32_t fHz, int ms)
{
    if (_taskVarioBeeperHandle != NULL)
    {
        vTaskSuspend(_taskVarioBeeperHandle);
#ifdef SOUND_DEBUG
        SerialPort.println("suspend task");
#endif // SOUND_DEBUG
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
#ifdef SOUND_DEBUG
        SerialPort.println("suspend task");
#endif // SOUND_DEBUG
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
#ifdef SOUND_DEBUG
        SerialPort.println("suspend task");
#endif // SOUND_DEBUG
    }
    generateTone(440, 100);
    delay(100);
    generateTone(880, 100);
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
#ifdef SOUND_DEBUG
        SerialPort.println("suspend task");
#endif // SOUND_DEBUG
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
    _volume = newVolume;
    fc.sound.volume = _volume;
}

uint8_t VarioTone::getVolume()
{
    return _volume;
}

void VarioTone::mute()
{
    noToneAC();
    _muted = true;
    fc.sound.isMute = _muted;
}

void VarioTone::unMute()
{
    _muted = false;
    fc.sound.isMute = _muted;
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
