#pragma once

#include <Arduino.h>
#include <FunctionFSM.h>
#include "VarioDisplay/VarioDisplay.h"
#include "Observer/Observer.h"
#include "Observer/Subject.h"
#include "event.h"

// template <typename Self, typename Finished>
class VarioFSM : public Observer, public Subject
{
private:
    FunctionState _state_boot;
    FunctionState _state_statistic_init;
    FunctionState _state_calibration;
    FunctionState _state_wifi;
    FunctionState _state_vario1;
    FunctionState _state_vario2;
    FunctionState _state_vario3;
    FunctionState _state_sound;
    FunctionState _state_statistic;
    FunctionState _state_sound_edit;

    FunctionFsm fsm;

    void boot_on_enter();
    void boot_on();
    void boot_on_exit();

    void calibration_on_enter();
    void calibration_on();
    void calibration_on_exit();

    void wifi_on_enter();
    void wifi_on();
    void wifi_on_exit();

    void vario1_on_enter();
    void vario1_on();
    void vario1_on_exit();

    void vario2_on_enter();
    void vario2_on();
    void vario2_on_exit();

    void vario3_on_enter();
    void vario3_on();
    void vario3_on_exit();

    void sound_on_enter();
    void sound_on();
    void sound_on_exit();

    void statistic_on_enter();
    void statistic_on();
    void statistic_on_exit();

    void soundedit_on_enter();
    void soundedit_on();
    void soundedit_on_exit();

    VarioDisplay *varioDisplay;

public:
    VarioFSM()
        : _state_boot([this]()
                      { boot_on_enter(); },
                      [this]()
                      { boot_on(); },
                      [this]()
                      { boot_on_exit(); }),

          _state_statistic_init([this]()
                                { statistic_on_enter(); },
                                [this]()
                                { statistic_on(); },
                                [this]()
                                { statistic_on_exit(); }),

          _state_calibration([this]()
                             { calibration_on_enter(); },
                             [this]()
                             { calibration_on(); },
                             [this]()
                             { calibration_on_exit(); }),

          _state_wifi([this]()
                      { wifi_on_enter(); },
                      [this]()
                      { wifi_on(); },
                      [this]()
                      { wifi_on_exit(); }),

          _state_vario1([this]()
                        { vario1_on_enter(); },
                        [this]()
                        { vario1_on(); },
                        [this]()
                        { vario1_on_exit(); }),

          _state_vario2([this]()
                        { vario2_on_enter(); },
                        [this]()
                        { vario2_on(); },
                        [this]()
                        { vario2_on_exit(); }),

          _state_vario3([this]()
                        { vario3_on_enter(); },
                        [this]()
                        { vario3_on(); },
                        [this]()
                        { vario3_on_exit(); }),

          _state_sound([this]()
                       { sound_on_enter(); },
                       [this]()
                       { sound_on(); },
                       [this]()
                       { sound_on_exit(); }),

          _state_statistic([this]()
                           { statistic_on_enter(); },
                           [this]()
                           { statistic_on(); },
                           [this]()
                           { statistic_on_exit(); }),

          _state_sound_edit([this]()
                            { soundedit_on_enter(); },
                            [this]()
                            { soundedit_on(); },
                            [this]()
                            { soundedit_on_exit(); }),

          fsm(&_state_boot)
    {
    }

    void initfsm(VarioDisplay *_varioDisplay);
    void update(uint8_t _val);
    void run();
};