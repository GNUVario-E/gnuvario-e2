#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include <Arduino.h>
#include "VarioDisplay/Widget/Widget.h"
#include "VarioDisplay/Widget/LogoWidget.h"
#include "VarioDisplay/Widget/TextWidget.h"
#include "VarioDisplay/Widget/AltWidget.h"
#include "VarioDisplay/Widget/VarioWidget.h"
#include "VarioDisplay/Widget/ToolbarWidget.h"

#include <VarioDisplay/VarioScreen/ScreenData.h>

#define NB_WIDGETS 20

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
    AltWidget *altWidget;
    VarioWidget *varioWidget;
    ToolbarWidget *toolbarWidget;

protected:
public:
    Widget *tabWidgets[NB_WIDGETS];
    VarioScreen(ScreenData data);
    // void addWidget(LogoWidget *widget);
    // void addWidget(TextWidget *widget);
    // void addWidget(AltWidget *widget);
    // void addWidget(VarioWidget *widget);
    uint8_t getNbWidgets();
    bool isRefreshNeeded();
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
    AltWidget *getAltWidget();
    VarioWidget *getVarioWidget();
    ToolbarWidget *getToolbarWidget();
};