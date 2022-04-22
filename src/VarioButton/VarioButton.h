#ifndef _VARIOBUTTON_H_
#define _VARIOBUTTON_H_

#include "Button.h"
#include "HardwareConfig/HardwareConfig.h"
#include "Observer/Subject.h"
#include "VarioDebug/VarioDebug.h"
#include "event.h"

#define DEBOUNCE_MS 30

class Observer;

class VarioButton : public Subject
{
private:
	// void _notifyObserver();
	const TickType_t delayT100 = pdMS_TO_TICKS(100);
	TaskHandle_t _taskVarioButtonHandle = NULL;
	static void startTaskImpl(void *);
	void task();
	bool _stateBA = false;
	bool _stateBB = false;
	bool _stateBC = false;
	void handleButton(uint8_t btn);

public:
	VarioButton();
	void startTask();
	Button BtnA = Button(PIN_BUTTON_A, true, DEBOUNCE_MS);
	Button BtnB = Button(PIN_BUTTON_B, true, DEBOUNCE_MS);
	Button BtnC = Button(PIN_BUTTON_C, true, DEBOUNCE_MS);

	void update();
};

#endif
