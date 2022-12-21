#include "VarioDisplay.h"
#define ENABLE_GxEPD2_GFX 1
#include <GxEPD2_BW.h>

// select the display class and display driver class in the following file (new style):
#include "GxEPD2_display_selection_new_style.h"
#include "version.h"
#include "VarioDebug/VarioDebug.h"

SemaphoreHandle_t VarioDisplay::screenMutex;
SemaphoreHandle_t VarioDisplay::displayMutex;
TaskHandle_t VarioDisplay::screenTaskHandler;
TaskHandle_t VarioDisplay::bufferTaskHandler;

uint32_t VarioDisplay::lastDisplayTime = 0;
uint32_t VarioDisplay::lastDisplayTimeToggle = 0;
uint16_t VarioDisplay::minTimeRefresh = 0;
bool forceIgnoreMinTimeRefresh = false;

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

    displayMutex = xSemaphoreCreateBinary();
    xSemaphoreGive(VarioDisplay::displayMutex);

    VARIO_PROG_DEBUG_PRINTLN("TaskDisplay started");
    xTaskCreatePinnedToCore(screenTask, "TaskDisplay", SCREEN_STACK_SIZE, NULL, SCREEN_PRIORITY, &screenTaskHandler, SCREEN_CORE);

    display.init();
    display.setRotation(0);
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&NotoSans6pt7b);
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
    char sound[] = "SOUND ...";

    // construction de l'écran de démarrage
    char tmpbuffer[50];
    bootScreen = new VarioScreen(bootScreenData, varioLanguage);
    bootScreen->getTextWidget1()->setText(varioLanguage->getText(TITRE_DEMAR));

    bootScreen->getTextWidget2()->setText("GNUVARIO-E2");
    bootScreen->getTextWidget2()->setBlinkFreq(1);

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

    sprintf(tmpbuffer, "%d%% (%.2fV)", fc.getPowerCapacite(), fc.getPowerTension());
    bootScreen->getTextWidget5()->setText(tmpbuffer);

    // construction de l'écran WIFI
    wifiScreen = new VarioScreen(wifiScreenData, varioLanguage);
    wifiScreen->getTextWidget1()->setText(varioLanguage->getText(TITRE_CONNECT));
    wifiScreen->getTextWidget1()->setIndexTxtFC(1);

    fc.setText1(false, varioLanguage->getText(TITRE_CONNECTA));

    // sprintf(fc.text.text1, "%s", varioLanguage->getText(TITRE_CONNECTA));
    wifiScreen->getTextWidget2()->setText("");
    wifiScreen->getTextWidget2()->setTextSize(1);
    wifiScreen->getTextWidget2()->setIndexTxtFC(2);
    wifiScreen->getTextWidget3()->setText("");
    wifiScreen->getTextWidget3()->setTextSize(1);
    wifiScreen->getTextWidget3()->setIndexTxtFC(3);

    // construction de l'écran calibration
    calibrationScreen = new VarioScreen(calibrationScreenData, varioLanguage);
    calibrationScreen->getTextWidget1()->setText(varioLanguage->getText(TITRE_CALIBR));

    // construction des écrans vario
    vario1Screen = new VarioScreen(vario1ScreenData, varioLanguage);
    vario2Screen = new VarioScreen(vario2ScreenData, varioLanguage);
    vario3Screen = new VarioScreen(vario3ScreenData, varioLanguage);

    // construction de l'écran son
    soundScreen = new VarioScreen(soundScreenData, varioLanguage);
    soundScreen->getTextWidget1()->setText(sound);

    statisticScreen = new VarioScreen(statisticScreenData, varioLanguage);
    // statisticScreen->getTextWidget1()->setText(varioLanguage->getText(TITRE_STAT));
    statisticScreen->getTextWidget1()->setIndexTxtFC(1);
    statisticScreen->getTextWidget2()->setIndexTxtFC(2);
    statisticScreen->getTextWidget3()->setIndexTxtFC(3);
    statisticScreen->getTextWidget4()->setIndexTxtFC(4);
    statisticScreen->getTextWidget5()->setIndexTxtFC(5);
    statisticScreen->getTextWidget6()->setIndexTxtFC(6);
    statisticScreen->getTextWidget7()->setIndexTxtFC(7);
    statisticScreen->getTextWidget8()->setIndexTxtFC(8);

    // construction de l'écran de reboot
    rebootScreen = new VarioScreen(rebootScreenData, varioLanguage);
    rebootScreen->getTextWidget1()->setText(varioLanguage->getText(TITRE_REDEMAR));

    // construction de l'écran de message
    messageScreen = new VarioScreen(messageScreenData, varioLanguage);
    messageScreen->getTextWidget1()->setText("");
    messageScreen->getTextWidget1()->setTextSize(1);
    messageScreen->getTextWidget1()->setIndexTxtFC(1);

    messageScreen->getTextWidget2()->setText("");
    messageScreen->getTextWidget2()->setTextSize(1);
    messageScreen->getTextWidget2()->setIndexTxtFC(2);
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
            // VARIO_PROG_DEBUG_PRINTLN("screen refresh");

            display.setFullWindow();
            display.display(true); // partial update

            display.powerOff();

            xSemaphoreGive(screenMutex);
        }

        if (xSemaphoreTake(displayMutex, portMAX_DELAY) == pdTRUE)
        {
            if (VarioDisplay::bufferTaskHandler != NULL)
            {
                xTaskNotify(VarioDisplay::bufferTaskHandler, 0, eNoAction);
            }
            xSemaphoreGive(displayMutex);
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
    bool firstRun = true;
    uint8_t nbToggle = 0;
    uint8_t tabToggle[10][2];
    while (true)
    {
        bool notifyTask = true;
        // /* wait */
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

        if (!firstRun)
        {
            nbToggle = 0;
            // no more than once per second
            if (forceIgnoreMinTimeRefresh || (millis() - lastDisplayTimeToggle > minTimeRefresh))
            {
                // toggle to next widget if necessary
                for (uint8_t i = 0; i < _currentScreen->getNbWidgets(); i++)
                {
                    if (_currentScreen->tabWidgets[i]->getIsActif())
                    {
                        uint8_t altWidgetIndex = _currentScreen->tabWidgets[i]->getAltWidgetIndex();
                        if (altWidgetIndex != 99)
                        {
                            // Serial.println("Activation du widget " + String(altWidgetIndex) + " avec ancien wigdet =" + String(i));
                            tabToggle[nbToggle][0] = i;
                            tabToggle[nbToggle][1] = altWidgetIndex;
                            nbToggle++;
                        }
                    }
                }
                for (uint8_t i = 0; i < nbToggle; i++)
                {
                    _currentScreen->tabWidgets[tabToggle[i][0]]->setIsActif(false);
                    _currentScreen->tabWidgets[tabToggle[i][0]]->setForceRefresh();

                    _currentScreen->tabWidgets[tabToggle[i][1]]->setIsActif(true);
                    _currentScreen->tabWidgets[tabToggle[i][1]]->setForceRefresh();
                    _currentScreen->tabWidgets[tabToggle[i][1]]->setForceClearZone();
                }
                lastDisplayTimeToggle = millis();
                forceIgnoreMinTimeRefresh = false;
            }
        }
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

                // Serial.println("needRefresh");
                xSemaphoreGive(screenMutex);
                updateScreen();
                lastDisplayTime = millis();
                notifyTask = false; // task will be notified at the end of the screen refresh
            }
        }

        if (firstRun)
        {
            // toggle to next widget if necessary
            for (uint8_t i = 0; i < _currentScreen->getNbWidgets(); i++)
            {
                if (_currentScreen->tabWidgets[i]->getIsActif())
                {
                    uint8_t altWidgetIndex = _currentScreen->tabWidgets[i]->getAltWidgetIndex();
                    if (altWidgetIndex != 99)
                    {
                        _currentScreen->tabWidgets[i]->setForceRefresh();
                    }
                }
            }
            firstRun = false;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
        if (notifyTask)
        {
            if (xSemaphoreTake(displayMutex, portMAX_DELAY) == pdTRUE)
            {
                if (VarioDisplay::bufferTaskHandler != NULL)
                {
                    xTaskNotify(VarioDisplay::bufferTaskHandler, 0, eNoAction);
                }
                xSemaphoreGive(displayMutex);
            }
        }
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
        forceIgnoreMinTimeRefresh = true;
        display.fillScreen(GxEPD_WHITE);
        xSemaphoreGive(screenMutex);
    }

    if (xSemaphoreTake(displayMutex, portMAX_DELAY) == pdTRUE)
    {
        if (bufferTaskHandler == NULL)
        {
            if (xTaskCreatePinnedToCore(this->startTaskBuffer, "TaskBuffer", SCREEN_STACK_SIZE, this, SCREEN_PRIORITY, &bufferTaskHandler, SCREEN_CORE) == pdPASS)
            {
                VARIO_PROG_DEBUG_PRINTLN("TaskBuffer created");
            }
            else
            {
                VARIO_PROG_DEBUG_PRINTLN("TaskBuffer not created");
            }
        }
        if (bufferTaskHandler != NULL)
        {
            xTaskNotify(VarioDisplay::bufferTaskHandler, 0, eNoAction);
        }
        xSemaphoreGive(displayMutex);
    }
}

void VarioDisplay::stopDisplay()
{
    // Serial.println("stopDisplay");
    if (xSemaphoreTake(displayMutex, portMAX_DELAY) == pdTRUE)
    {
        if (bufferTaskHandler != NULL)
        {
            vTaskDelete(VarioDisplay::bufferTaskHandler);
            VarioDisplay::bufferTaskHandler = NULL;
        }
        xSemaphoreGive(displayMutex);
    }
}

void VarioDisplay::powerOff()
{
    display.powerOff();
}

void VarioDisplay::setMinTimeRefresh(uint16_t _minTimeRefresh)
{
    minTimeRefresh = _minTimeRefresh;
}