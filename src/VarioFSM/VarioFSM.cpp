#include "VarioFSM/VarioFSM.h"
#include "VarioButton/VarioButton.h"

void VarioFSM::startTask()
{
    // task creation
    VARIO_PROG_DEBUG_PRINTLN("TaskFSM started");
    xTaskCreate(this->startTaskImpl, "TaskFSM", 2000, this, 10, &_taskFSMHandle);
}

void VarioFSM::startTaskImpl(void *parm)
{
    // wrapper for task
    static_cast<VarioFSM *>(parm)->task();
}

void VarioFSM::task()
{

    while (1)
    {
        run();

        // give time to other tasks
        vTaskDelay(delayT50);
    }
}

void VarioFSM::initfsm(VarioDisplay *_varioDisplay)
{
    varioDisplay = _varioDisplay;

    // definition des transitions
    fsm.add_timed_transition(&_state_boot, &_state_statistic_init, 4000, nullptr);   // bascule en mode vario si pas d'appui sur le bouton pendant 5s
    fsm.add_timed_transition(&_state_statistic_init, &_state_vario1, 3000, nullptr); // bascule en mode vario apres les stats initiales

    fsm.add_transition(&_state_boot, &_state_calibration, BTN_SHORT_C, nullptr); // bascule en mode calibration si appui sur bouton C pendant boot
    fsm.add_transition(&_state_boot, &_state_wifi, BTN_SHORT_A, nullptr);        // bascule en mode wifi si appui sur bouton A pendant boot

    fsm.add_transition(&_state_vario1, &_state_vario2, BTN_SHORT_C, nullptr);    // deplacement dans les ecrans vario
    fsm.add_transition(&_state_vario2, &_state_vario3, BTN_SHORT_C, nullptr);    // deplacement dans les ecrans vario
    fsm.add_transition(&_state_vario3, &_state_sound, BTN_SHORT_C, nullptr);     // deplacement dans les ecrans vario
    fsm.add_transition(&_state_sound, &_state_statistic, BTN_SHORT_C, nullptr);  // deplacement dans les ecrans vario
    fsm.add_transition(&_state_statistic, &_state_vario1, BTN_SHORT_C, nullptr); // deplacement dans les ecrans vario

    fsm.add_transition(&_state_statistic, &_state_sound, BTN_SHORT_A, nullptr);  // deplacement dans les ecrans vario
    fsm.add_transition(&_state_sound, &_state_vario3, BTN_SHORT_A, nullptr);     // deplacement dans les ecrans vario
    fsm.add_transition(&_state_vario3, &_state_vario2, BTN_SHORT_A, nullptr);    // deplacement dans les ecrans vario
    fsm.add_transition(&_state_vario2, &_state_vario1, BTN_SHORT_A, nullptr);    // deplacement dans les ecrans vario
    fsm.add_transition(&_state_vario1, &_state_statistic, BTN_SHORT_A, nullptr); // deplacement dans les ecrans vario

    fsm.add_transition(&_state_sound, &_state_sound_edit, BTN_SHORT_B, nullptr); // gestion de l'edition du son
    fsm.add_transition(&_state_sound_edit, &_state_sound, BTN_SHORT_B, nullptr); // gestion de l'edition du son
}

void VarioFSM::boot_on_enter()
{
    VARIO_FSM_DEBUG_PRINTLN("boot_on_enter");
    varioDisplay->displayScreen(varioDisplay->bootScreen);
}

void VarioFSM::boot_on()
{
    VARIO_FSM_DEBUG_PRINTLN("boot_on");
}

void VarioFSM::boot_on_exit()
{
    VARIO_FSM_DEBUG_PRINTLN("boot_on_exit");
}

void VarioFSM::calibration_on_enter()
{
    VARIO_FSM_DEBUG_PRINTLN("calibration_on_enter");
    calibOnEnter = true;
}

void VarioFSM::calibration_on()
{
    VARIO_FSM_DEBUG_PRINTLN("calibration_on");
    if (calibOnEnter)
    {
        varioDisplay->displayScreen(varioDisplay->calibrationScreen);
        // vTaskDelay(delayT50);
        _notifyObserver(CALIBRATION_START_ASKED);
        calibOnEnter = false;
    }
}

void VarioFSM::calibration_on_exit()
{
    VARIO_FSM_DEBUG_PRINTLN("calibration_on_exit");
}

void VarioFSM::wifi_on_enter()
{
    VARIO_FSM_DEBUG_PRINTLN("wifi_on_enter");
    wifiOnEnter = true;
}

void VarioFSM::wifi_on()
{
    VARIO_FSM_DEBUG_PRINTLN("wifi_on");
    if (wifiOnEnter)
    {
        varioDisplay->displayScreen(varioDisplay->wifiScreen);
        _notifyObserver(WIFI_START_ASKED);
        wifiOnEnter = false;
    }
}

void VarioFSM::wifi_on_exit()
{
    VARIO_FSM_DEBUG_PRINTLN("wifi_on_exit");
}

void VarioFSM::vario1_on_enter()
{
    VARIO_FSM_DEBUG_PRINTLN("vario1_on_enter");
    varioDisplay->displayScreen(varioDisplay->vario1Screen);
}

void VarioFSM::vario1_on()
{
    VARIO_FSM_DEBUG_PRINTLN("vario1_on");
}

void VarioFSM::vario1_on_exit()
{
    VARIO_FSM_DEBUG_PRINTLN("vario1_on_exit");
}

void VarioFSM::vario2_on_enter()
{
    VARIO_FSM_DEBUG_PRINTLN("vario2_on_enter");
    varioDisplay->displayScreen(varioDisplay->vario2Screen);
}

void VarioFSM::vario2_on()
{
    VARIO_FSM_DEBUG_PRINTLN("vario2_on");
}

void VarioFSM::vario2_on_exit()
{
    VARIO_FSM_DEBUG_PRINTLN("vario2_on_exit");
}

void VarioFSM::vario3_on_enter()
{
    VARIO_FSM_DEBUG_PRINTLN("vario3_on_enter");
    varioDisplay->displayScreen(varioDisplay->vario3Screen);
}

void VarioFSM::vario3_on()
{
    VARIO_FSM_DEBUG_PRINTLN("vario3_on");
}

void VarioFSM::vario3_on_exit()
{
    VARIO_FSM_DEBUG_PRINTLN("vario3_on_exit");
}

void VarioFSM::sound_on_enter()
{
    VARIO_FSM_DEBUG_PRINTLN("sound_on_enter");
    varioDisplay->displayScreen(varioDisplay->soundScreen);
}

void VarioFSM::sound_on()
{
    VARIO_FSM_DEBUG_PRINTLN("sound_on");
}

void VarioFSM::sound_on_exit()
{
    VARIO_FSM_DEBUG_PRINTLN("sound_on_exit");
    _notifyObserver(VOLUME_SAVE_ASKED);
}

void VarioFSM::statistic_on_enter()
{
    VARIO_FSM_DEBUG_PRINTLN("statistic_on_enter");
    varioDisplay->displayScreen(varioDisplay->statisticScreen);
}

void VarioFSM::statistic_on()
{
    VARIO_FSM_DEBUG_PRINTLN("statistic_on");
}

void VarioFSM::statistic_on_exit()
{
    VARIO_FSM_DEBUG_PRINTLN("statistic_on_exit");
}

void VarioFSM::statistic_init_on_exit()
{
    VARIO_FSM_DEBUG_PRINTLN("statistic_init_on_exit");
    _notifyObserver(VARIO_START_ASKED);
}

void VarioFSM::soundedit_on_enter()
{
    VARIO_FSM_DEBUG_PRINTLN("soundedit_on_enter");
    // varioDisplay->displayScreen(varioDisplay->soundeditScreen);
}

void VarioFSM::soundedit_on()
{
    VARIO_FSM_DEBUG_PRINTLN("soundedit_on");
}

void VarioFSM::soundedit_on_exit()
{
    VARIO_FSM_DEBUG_PRINTLN("soundedit_on_exit");
    // sauvegarde du volume
}

void VarioFSM::onSignalReceived(uint8_t _val)
{
    if ((fsm.is_in_state(_state_vario1) || fsm.is_in_state(_state_vario2) || fsm.is_in_state(_state_vario3)) && _val == BTN_SHORT_B)
    {
        // toggle mute
        VARIO_FSM_DEBUG_PRINTLN("toggle mute");
        _notifyObserver(VOLUME_TOGGLE_MUTE_ASKED);
    }
    else if (fsm.is_in_state(_state_sound_edit))
    {
        // gestion de l'edition du son
        VARIO_FSM_DEBUG_PRINTLN("sound edit");
        switch (_val)
        {
        case BTN_SHORT_A:
            // volume down
            _notifyObserver(VOLUME_DOWN_ASKED);
            break;
        case BTN_SHORT_C:
            // volume up
            _notifyObserver(VOLUME_UP_ASKED);
            break;
        default:
            break;
        }
    }
    else
    {
        fsm.trigger(_val);
    }
}

void VarioFSM::run()
{
    fsm.run_machine();
}