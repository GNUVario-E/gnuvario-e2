#pragma once

#include "Arduino.h"
#include <soc/rtc_io_reg.h>
#include <soc/rtc_cntl_reg.h>
#include <soc/sens_reg.h>
#include <soc/rtc.h>
#include <driver/dac.h>

class AudioCos
{
private:
    uint8_t volume = 1;
    void dac_cosine_enable(dac_channel_t channel);
    void dac_frequency_set(int clk_8m_div, int frequency_step);
    void dac_scale_set(dac_channel_t channel, int scale);
    void dac_offset_set(dac_channel_t channel, int offset);
    void dac_invert_set(dac_channel_t channel, int invert);

public:
    void audio_config(int pnDacChan, uint8_t _volume);
    void audio_generate_tone(int freqHz, int milliseconds);
    void audio_set_frequency(int freqHz);
    void audio_set_volume(uint8_t _volume);
};