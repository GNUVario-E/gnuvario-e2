#include "VarioButton/VarioButton.h"
#include "Observer/Observer.h"

#define DELAY_LONG 1500
#define BUTTON_TASK_PRIORITY 9

VarioButton::VarioButton()
{
	// Setup the button with an internal pull-up
	pinMode(PIN_BUTTON_A, INPUT_PULLUP);
	pinMode(PIN_BUTTON_B, INPUT_PULLUP);
	pinMode(PIN_BUTTON_C, INPUT_PULLUP);
}

void VarioButton::startTask()
{
	// task creation
	VARIO_PROG_DEBUG_PRINTLN("TaskVarioButton started");
	// xTaskCreate(this->startTaskImpl, "TaskVarioButton", 4096, this, BUTTON_TASK_PRIORITY, &_taskVarioButtonHandle);
	xTaskCreatePinnedToCore(this->startTaskImpl, "TaskVarioButton", 3000, this, BUTTON_TASK_PRIORITY, &_taskVarioButtonHandle, 1);
}

void VarioButton::startTaskImpl(void *parm)
{
	// wrapper for task
	static_cast<VarioButton *>(parm)->task();
}

void VarioButton::task()
{

	while (1)
	{
		update();

		// give time to other tasks
		vTaskDelay(delayT100);

		if (uxTaskGetStackHighWaterMark(NULL) < minRemainingStackSize)
		{
			minRemainingStackSize = uxTaskGetStackHighWaterMark(NULL);
			Serial.printf("TaskVarioButton stack: %d", minRemainingStackSize);
		}
	}
}

void VarioButton::update()
{
	// Button update
	BtnA.read();
	BtnB.read();
	BtnC.read();

	// BTN A
	if (BtnA.wasPressed())
	{
		_stateBA = true;
	}

	if (BtnA.pressedFor(DELAY_LONG))
	{
		if (_stateBA == true)
		{
			handleButton(BTN_LONG_A);
			_stateBA = false;
		}
	}

	if (BtnA.wasReleased())
	{
		if (_stateBA == true)
		{
			handleButton(BTN_SHORT_A);
			_stateBA = false;
		}
	}

	// BTN B
	if (BtnB.wasPressed())
	{
		_stateBB = true;
	}

	if (BtnB.pressedFor(DELAY_LONG))
	{
		if (_stateBB == true)
		{
			handleButton(BTN_LONG_B);
			_stateBB = false;
		}
	}

	if (BtnB.wasReleased())
	{
		if (_stateBB == true)
		{
			handleButton(BTN_SHORT_B);
			_stateBB = false;
		}
	}

	// BTN C
	if (BtnC.wasPressed())
	{
		_stateBC = true;
	}

	if (BtnC.pressedFor(DELAY_LONG))
	{
		if (_stateBC == true)
		{
			handleButton(BTN_LONG_C);
			_stateBC = false;
		}
	}

	if (BtnC.wasReleased())
	{
		if (_stateBC == true)
		{
			handleButton(BTN_SHORT_C);
			_stateBC = false;
		}
	}
}

void VarioButton::handleButton(uint8_t btn)
{
	switch (btn)
	{
	case BTN_SHORT_A:
		VARIO_BTN_DEBUG_PRINTLN("Appui court BTN A");
		break;
	case BTN_SHORT_B:
		VARIO_BTN_DEBUG_PRINTLN("Appui court BTN B");
		break;
	case BTN_SHORT_C:
		VARIO_BTN_DEBUG_PRINTLN("Appui court BTN C");
		break;
	case BTN_LONG_A:
		VARIO_BTN_DEBUG_PRINTLN("Appui long BTN A");
		break;
	case BTN_LONG_B:
		VARIO_BTN_DEBUG_PRINTLN("Appui long BTN B");
		break;
	case BTN_LONG_C:
		VARIO_BTN_DEBUG_PRINTLN("Appui long BTN C");
		break;
	}

	_notifyObserver(btn);
}