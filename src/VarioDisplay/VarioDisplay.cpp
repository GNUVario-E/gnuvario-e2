#include "VarioDisplay.h"
#define ENABLE_GxEPD2_GFX 1
#include <GxEPD2_BW.h>

// select the display class and display driver class in the following file (new style):
#include "GxEPD2_display_selection_new_style.h"

#include "version.h"
#include "VarioDebug/VarioDebug.h"

ScreenData bootScreenData = {
    logo : {true, false, 0, 0, 128, 130, 94, 74},
    txt1 : {true, false, 0, 120, 128, 21},
    txt2 : {true, false, 0, 160, 128, 21},
    txt3 : {true, false, 0, 180, 128, 21},
    txt4 : {true, false, 0, 230, 128, 41},
    txt5 : {true, false, 0, 270, 128, 41},
    txt6 : {false, true, 20, 130, 100, 41},
    txt7 : {false, true, 20, 130, 100, 41},
    txt8 : {false, true, 20, 130, 100, 41},
    alt : {false, true, 0, 170, 65, 40},
    vario : {false, true, 64, 170, 64, 40},
    toolbar : {false, true, 0, 210, 128, 98}
};

ScreenData wifiScreenData = {
    logo : {true, true, 0, 0, 128, 296, 94, 74},
    txt1 : {true, false, 0, 230, 128, 41},
    txt2 : {false, true, 20, 130, 100, 41},
    txt3 : {false, true, 20, 130, 100, 41},
    txt4 : {false, true, 20, 130, 100, 41},
    txt5 : {false, true, 20, 130, 100, 41},
    txt6 : {false, true, 20, 130, 100, 41},
    txt7 : {false, true, 20, 130, 100, 41},
    txt8 : {false, true, 20, 130, 100, 41},
    alt : {false, true, 0, 170, 65, 40},
    vario : {false, true, 64, 170, 64, 40},
    toolbar : {false, true, 0, 210, 128, 98}
};

ScreenData calibrationScreenData = {
    logo : {true, true, 0, 0, 128, 296, 94, 74},
    txt1 : {true, false, 0, 230, 128, 41},
    txt2 : {false, true, 20, 130, 100, 41},
    txt3 : {false, true, 20, 130, 100, 41},
    txt4 : {false, true, 20, 130, 100, 41},
    txt5 : {false, true, 20, 130, 100, 41},
    txt6 : {false, true, 20, 130, 100, 41},
    txt7 : {false, true, 20, 130, 100, 41},
    txt8 : {false, true, 20, 130, 100, 41},
    alt : {false, true, 0, 170, 65, 40},
    vario : {false, true, 64, 170, 64, 40},
    toolbar : {false, true, 0, 210, 128, 98}
};

ScreenData vario1ScreenData = {
    logo : {false, true, 0, 0, 128, 91, 94, 74},
    txt1 : {true, true, 0, 146, 128, 51},
    txt2 : {true, true, 0, 196, 128, 51},
    txt3 : {true, true, 0, 246, 128, 50},
    txt4 : {false, true, 20, 130, 100, 41},
    txt5 : {false, true, 20, 130, 100, 41},
    txt6 : {false, true, 20, 130, 100, 41},
    txt7 : {false, true, 20, 130, 100, 41},
    txt8 : {false, true, 20, 130, 100, 41},
    alt : {true, true, 0, 46, 128, 51},
    vario : {true, true, 0, 96, 128, 51},
    toolbar : {true, true, 0, 0, 128, 47}
};

ScreenData vario2ScreenData = {
    logo : {false, true, 0, 246, 128, 50, 94, 74},
    txt1 : {true, true, 0, 46, 128, 51},
    txt2 : {true, true, 0, 96, 128, 51},
    txt3 : {false, true, 20, 130, 100, 41},
    txt4 : {false, true, 20, 130, 100, 41},
    txt5 : {false, true, 20, 130, 100, 41},
    txt6 : {false, true, 20, 130, 100, 41},
    txt7 : {false, true, 20, 130, 100, 41},
    txt8 : {false, true, 20, 130, 100, 41},
    alt : {true, true, 0, 146, 128, 51},
    vario : {true, true, 0, 196, 128, 51},
    toolbar : {true, true, 0, 0, 128, 47}
};

ScreenData vario3ScreenData = {
    logo : {true, true, 0, 0, 128, 296, 94, 74},
    txt1 : {false, true, 0, 100, 128, 20},
    txt2 : {false, true, 29, 210, 100, 21},
    txt3 : {false, true, 20, 130, 100, 41},
    txt4 : {false, true, 20, 130, 100, 41},
    txt5 : {false, true, 20, 130, 100, 41},
    txt6 : {false, true, 20, 130, 100, 41},
    txt7 : {false, true, 20, 130, 100, 41},
    txt8 : {false, true, 20, 130, 100, 41},
    alt : {false, true, 10, 230, 100, 20},
    vario : {false, true, 10, 190, 100, 20},
    toolbar : {false, true, 0, 230, 128, 41}
};

ScreenData soundScreenData = {
    logo : {true, true, 0, 0, 128, 296, 94, 74},
    txt1 : {true, false, 0, 230, 128, 41},
    txt2 : {false, true, 20, 130, 100, 41},
    txt3 : {false, true, 20, 130, 100, 41},
    txt4 : {false, true, 20, 130, 100, 41},
    txt5 : {false, true, 20, 130, 100, 41},
    txt6 : {false, true, 20, 130, 100, 41},
    txt7 : {false, true, 20, 130, 100, 41},
    txt8 : {false, true, 20, 130, 100, 41},
    alt : {false, true, 0, 170, 65, 40},
    vario : {false, true, 64, 170, 64, 40},
    toolbar : {false, true, 0, 210, 128, 98}
};

ScreenData statisticScreenData = {
    logo : {false, true, 0, 0, 128, 296, 94, 74},
    txt1 : {true, false, 0, 0, 128, 21},
    txt2 : {true, false, 0, 20, 128, 21},
    txt3 : {true, false, 0, 40, 128, 21},
    txt4 : {true, false, 0, 60, 128, 21},
    txt5 : {true, false, 0, 80, 128, 21},
    txt6 : {true, false, 0, 100, 128, 21},
    txt7 : {true, false, 0, 120, 128, 21},
    txt8 : {true, false, 0, 140, 128, 21},
    alt : {false, true, 0, 170, 65, 40},
    vario : {false, true, 64, 170, 64, 40},
    toolbar : {false, true, 0, 210, 128, 98}
};

SemaphoreHandle_t VarioDisplay::screenMutex;
TaskHandle_t VarioDisplay::screenTaskHandler;
TaskHandle_t VarioDisplay::bufferTaskHandler;

uint32_t VarioDisplay::lastDisplayTime = 0;

/**
 * Initialisetion des différents objets
 */
VarioDisplay::VarioDisplay()
{
}

/**
 * Initialisation de l'affichage
 */
void VarioDisplay::init(VarioLanguage *_varioLanguage)
{
    varioLanguage = _varioLanguage;

    buildScreens();

    screenMutex = xSemaphoreCreateBinary();
    xSemaphoreGive(VarioDisplay::screenMutex);

    VARIO_PROG_DEBUG_PRINTLN("TaskDisplay started");
    xTaskCreatePinnedToCore(screenTask, "TaskDisplay", SCREEN_STACK_SIZE, NULL, SCREEN_PRIORITY, &screenTaskHandler, SCREEN_CORE);

    display.init();
    display.setRotation(0);
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&FreeSans6pt7b);
    display.setTextSize(1);

    // premier affichage full
    display.setFullWindow();
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
    } while (display.nextPage());
}

void VarioDisplay::buildScreens()
{
    char wifi[] = "WIFI ...";
    char calibration[] = "CALIBRATION ...";
    char sound[] = "SOUND ...";
    char statistic[] = "STATISTIC ...";

    // construction de l'écran de démarrage
    char tmpbuffer[50];
    bootScreen = new VarioScreen(bootScreenData, varioLanguage);
    bootScreen->getTextWidget1()->setText(varioLanguage->getText(TITRE_DEMAR));

    bootScreen->getTextWidget2()->setText("GNUVARIO-E2");

    sprintf(tmpbuffer, "v%d.%02d", VERSION, SUB_VERSION);
    if (BETA_CODE > 0)
    {
        sprintf(tmpbuffer + strlen(tmpbuffer), " - b%01d", BETA_CODE);
    }
    bootScreen->getTextWidget3()->setText(tmpbuffer);
    bootScreen->getTextWidget3()->setTextSize(1);

    sprintf(tmpbuffer, "%s", __DATE__);
    bootScreen->getTextWidget4()->setText(tmpbuffer);
    bootScreen->getTextWidget4()->setTextSize(1);

    sprintf(tmpbuffer, "%d%% (%.2fV)", fc.power.capacite, fc.power.tension);
    bootScreen->getTextWidget5()->setText(tmpbuffer);

    // construction de l'écran WIFI
    wifiScreen = new VarioScreen(wifiScreenData, varioLanguage);
    wifiScreen->getTextWidget1()->setText(wifi);

    calibrationScreen = new VarioScreen(calibrationScreenData, varioLanguage);
    calibrationScreen->getTextWidget1()->setText(calibration);

    vario1Screen = new VarioScreen(vario1ScreenData, varioLanguage);
    vario2Screen = new VarioScreen(vario2ScreenData, varioLanguage);
    vario3Screen = new VarioScreen(vario3ScreenData, varioLanguage);

    soundScreen = new VarioScreen(soundScreenData, varioLanguage);
    soundScreen->getTextWidget1()->setText(sound);

    statisticScreen = new VarioScreen(statisticScreenData, varioLanguage);
    statisticScreen->getTextWidget1()->setText(statistic);
}

/**
 * Tache d'affichage
 */
void VarioDisplay::screenTask(void *parameter)
{
    while (true)
    {
        /* wait */
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
        /* launch interrupt */
        if (xSemaphoreTake(screenMutex, portMAX_DELAY) == pdTRUE)
        {
            // d = millis();
            // Serial.println(millis());
            VARIO_PROG_DEBUG_PRINTLN("screen refresh");
            display.setFullWindow();
            display.display(true); // partial update

            display.powerOff();
            lastDisplayTime = millis();
            xSemaphoreGive(screenMutex);
            // Serial.print("duration: ");
            // Serial.println(millis() - d);
        }
    }
}

void VarioDisplay::startTaskBuffer(void *parm)
{
    // wrapper for task
    static_cast<VarioDisplay *>(parm)->bufferTask();
}

/**
 * Tache de buffer
 */
void VarioDisplay::bufferTask()
{
    while (true)
    {
        // /* wait */
        // xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
        if (_doDisplay)
        {

            /* launch interrupt */

            // VARIO_PROG_DEBUG_PRINTLN("bufferTask");

            if (_currentScreen->isRefreshNeeded(lastDisplayTime))
            {
                if (xSemaphoreTake(screenMutex, portMAX_DELAY) == pdTRUE)
                {
                    // loop over widgets
                    for (uint8_t i = 0; i < _currentScreen->getNbWidgets(); i++)
                    {
                        if (_currentScreen->tabWidgets[i]->getIsActif() && _currentScreen->tabWidgets[i]->isRefreshNeeded(lastDisplayTime))
                        {
                            _currentScreen->tabWidgets[i]->addToBuffer(display);
                        }
                    }

                    Serial.println("needRefresh");
                    xSemaphoreGive(screenMutex);
                    updateScreen();
                }
            }

            // xSemaphoreGive(screenMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

/**
 * Méthode de déclenchement du raffraichissement
 */
void VarioDisplay::updateScreen(void)
{
    // Serial.println("updateScreen");
    xTaskNotify(VarioDisplay::screenTaskHandler, 0, eNoAction);
}

/**
 * Méthode de remplissage du buffer de l'ecran
 */

void VarioDisplay::displayScreen(VarioScreen *screen)
{
    // Serial.println("displayScreen");
    stopDisplay();

    _currentScreen = screen;

    // loop over widgets
    for (uint8_t i = 0; i < _currentScreen->getNbWidgets(); i++)
    {
        _currentScreen->tabWidgets[i]->setForceRefresh();
    }
    if (xSemaphoreTake(screenMutex, portMAX_DELAY) == pdTRUE)
    {
        // Serial.println("mutex displayScreen");
        display.fillScreen(GxEPD_WHITE);
        xSemaphoreGive(screenMutex);
        _doDisplay = true;
        xTaskCreatePinnedToCore(this->startTaskBuffer, "TaskBuffer", SCREEN_STACK_SIZE, this, SCREEN_PRIORITY, &bufferTaskHandler, SCREEN_CORE);
        // xTaskNotify(VarioDisplay::bufferTaskHandler, 0, eNoAction);
    }
}

void VarioDisplay::stopDisplay()
{
    // Serial.println("stopDisplay");
    _doDisplay = false;
    if (bufferTaskHandler != NULL)
    {
        vTaskDelete(bufferTaskHandler);
        bufferTaskHandler = NULL;
    }
}