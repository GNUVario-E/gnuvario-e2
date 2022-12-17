#include "VarioTone.h"

VarioTone::VarioTone()
{
}

void VarioTone::init(uint8_t baseVolume)
{
    pinMode(PIN_AUDIO_AMP_ENA, OUTPUT);
    disableAmp();
    audiocos = new AudioCos();
    _volume = baseVolume;
    fc.setSoundVolume(_volume);
    audiocos->audio_config(1, _volume);
}

void VarioTone::generateTone(uint32_t fHz, int ms)
{
    if (_taskVarioBeeperHandle != NULL)
    {
        vTaskSuspend(_taskVarioBeeperHandle);

        VARIO_SOUND_DEBUG_PRINTLN("suspend task");
    }
    audiocos->audio_generate_tone(fHz, ms);

    // enableAmp();
    // vTaskDelay(delayT10 * 5); // time for amp to get on
    // toneAC(fHz, _volume, ms);
    // disableAmp();
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
    audiocos->audio_generate_tone(fHz, ms);

    // enableAmp();
    // vTaskDelay(delayT10 * 5); // time for amp to get on
    // toneAC(fHz, volume, ms);
    // disableAmp();
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

    if (!_isAmpOn)
    {
        enableAmp();
        delay(80);
    }

    audiocos->audio_generate_tone(220, 100);
    delay(50);
    audiocos->audio_generate_tone(440, 100);
    delay(50);
    audiocos->audio_generate_tone(880, 100);

    // generateTone(220, 100);
    // delay(50);
    // generateTone(440, 100);
    // delay(50);
    // generateTone(880, 100);
    disableAmp();
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

    if (!_isAmpOn)
    {
        enableAmp();
        delay(80);
    }

    while (fHz > 150)
    {
        audiocos->audio_set_frequency(fHz);
        // toneAC(fHz, _volume);
        fHz -= 5;
        vTaskDelay(delayT10); // time for amp to get on
    }
    audiocos->audio_set_frequency(0);
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
        audiocos->audio_set_frequency(fHz);
        // toneAC(fHz, _volume);
        fHz--;
        vTaskDelay(delayT10); // time for amp to get on
    }
    audiocos->audio_set_frequency(0);
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
    else if (_volume > 3)
    {
        _volume = 3;
    }
    else
    {
        _volume = newVolume;
    }
    audiocos->audio_set_volume(_volume);
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
    audiocos->audio_set_frequency(0);
    // noToneAC();
    _muted = true;
    disableAmp();
    fc.setSoundIsMute(_muted);
}

void VarioTone::unMute()
{
    _muted = false;
    // enableAmp();
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

void VarioTone::stopBip()
{
    audiocos->audio_set_frequency(0);
}
void VarioTone::setFreq(uint32_t fHz)
{
    audiocos->audio_set_frequency(fHz);
}
