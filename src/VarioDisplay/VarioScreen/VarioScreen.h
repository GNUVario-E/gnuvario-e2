#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include <Arduino.h>
#include "VarioDisplay/Widget/Widget.h"
#include "VarioDisplay/Widget/LogoWidget.h"
#include "VarioDisplay/Widget/TextWidget.h"
#include "VarioDisplay/Widget/AltiWidget.h"
#include "VarioDisplay/Widget/VarioWidget.h"
#include "VarioDisplay/Widget/ToolbarWidget.h"
#include "VarioDisplay/Widget/SpeedWidget.h"
#include "VarioDisplay/Widget/BearingWidget.h"
#include "VarioDisplay/Widget/TimeWidget.h"
#include "VarioDisplay/Widget/FlightTimeWidget.h"
#include "VarioDisplay/Widget/LatWidget.h"
#include "VarioDisplay/Widget/LonWidget.h"
#include "VarioDisplay/Widget/AltiGpsWidget.h"
#include "VarioDisplay/Widget/AglWidget.h"
#include "VarioDisplay/Widget/WindWidget.h"
#include "VarioLanguage/VarioLanguage.h"
#include <VarioDisplay/VarioScreen/ScreenData.h>

#define NB_WIDGETS 25

class VarioScreen
{
private:
    uint8_t nb_widgets = 0;
    LogoWidget *logoWidget;
    TextWidget *textWidget1;
    TextWidget *textWidget2;
    TextWidget *textWidget3;
    TextWidget *textWidget4;
    TextWidget *textWidget5;
    TextWidget *textWidget6;
    TextWidget *textWidget7;
    TextWidget *textWidget8;
    AltiWidget *altiWidget;
    VarioWidget *varioWidget;
    ToolbarWidget *toolbarWidget;
    SpeedWidget *speedWidget;
    BearingWidget *bearingWidget;
    TimeWidget *timeWidget;
    FlightTimeWidget *flightTimeWidget;
    LatWidget *latWidget;
    LonWidget *lonWidget;
    AltiGpsWidget *altiGpsWidget;
    AglWidget *aglWidget;
    WindWidget *windWidget;
    // SoundeditWidget *soundEditWidget;

protected:
public:
    Widget *tabWidgets[NB_WIDGETS];
    VarioScreen(ScreenData data, VarioLanguage *varioLanguage);

    uint8_t getNbWidgets();
    bool isRefreshNeeded(uint32_t lastDisplayTime);
    void display();
    LogoWidget *getLogoWidget();
    TextWidget *getTextWidget1();
    TextWidget *getTextWidget2();
    TextWidget *getTextWidget3();
    TextWidget *getTextWidget4();
    TextWidget *getTextWidget5();
    TextWidget *getTextWidget6();
    TextWidget *getTextWidget7();
    TextWidget *getTextWidget8();
    AltiWidget *getAltiWidget();
    VarioWidget *getVarioWidget();
    ToolbarWidget *getToolbarWidget();
    SpeedWidget *getSpeedWidget();
    BearingWidget *getBearingWidget();
    TimeWidget *getTimeWidget();
    FlightTimeWidget *getFlightTimeWidget();
    LatWidget *getLatWidget();
    LonWidget *getLonWidget();
    AltiGpsWidget *getAltiGpsWidget();
    AglWidget *getAglWidget();
    WindWidget *getWindWidget();
};