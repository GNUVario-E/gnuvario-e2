#include "VarioScreen.h"

VarioScreen::VarioScreen(ScreenData data, VarioLanguage *_varioLanguage)
{
    logoWidget = new LogoWidget(_varioLanguage, data.logo.altWidgetIndex, data.logo.x, data.logo.y, data.logo.w, data.logo.h);
    logoWidget->setIsActif(data.logo.isactif);
    logoWidget->setIsBorder(data.logo.isborder);

    textWidget1 = new TextWidget(_varioLanguage, data.txt1.altWidgetIndex, data.txt1.x, data.txt1.y, data.txt1.w, data.txt1.h);
    textWidget1->setIsActif(data.txt1.isactif);
    textWidget1->setIsBorder(data.txt1.isborder);
    textWidget1->setText("TEXTE 1");
    // textWidget1->setBlinkFreq(1);

    textWidget2 = new TextWidget(_varioLanguage, data.txt2.altWidgetIndex, data.txt2.x, data.txt2.y, data.txt2.w, data.txt2.h);
    textWidget2->setIsActif(data.txt2.isactif);
    textWidget2->setIsBorder(data.txt2.isborder);
    textWidget2->setText("TEXTE 2");
    // textWidget2->setBlinkFreq(2);

    textWidget3 = new TextWidget(_varioLanguage, data.txt3.altWidgetIndex, data.txt3.x, data.txt3.y, data.txt3.w, data.txt3.h);
    textWidget3->setIsActif(data.txt3.isactif);
    textWidget3->setIsBorder(data.txt3.isborder);
    textWidget3->setText("TEXTE 3");
    // textWidget3->setBlinkFreq(2);

    textWidget4 = new TextWidget(_varioLanguage, data.txt4.altWidgetIndex, data.txt4.x, data.txt4.y, data.txt4.w, data.txt4.h);
    textWidget4->setIsActif(data.txt4.isactif);
    textWidget4->setIsBorder(data.txt4.isborder);
    textWidget4->setText("TEXTE 4");
    // textWidget4->setBlinkFreq(2);

    textWidget5 = new TextWidget(_varioLanguage, data.txt5.altWidgetIndex, data.txt5.x, data.txt5.y, data.txt5.w, data.txt5.h);
    textWidget5->setIsActif(data.txt5.isactif);
    textWidget5->setIsBorder(data.txt5.isborder);
    textWidget5->setText("TEXTE 5");
    // textWidget5->setBlinkFreq(2);

    textWidget6 = new TextWidget(_varioLanguage, data.txt6.altWidgetIndex, data.txt6.x, data.txt6.y, data.txt6.w, data.txt6.h);
    textWidget6->setIsActif(data.txt6.isactif);
    textWidget6->setIsBorder(data.txt6.isborder);
    textWidget6->setText("TEXTE 6");
    // textWidget6->setBlinkFreq(2);

    textWidget7 = new TextWidget(_varioLanguage, data.txt7.altWidgetIndex, data.txt7.x, data.txt7.y, data.txt7.w, data.txt7.h);
    textWidget7->setIsActif(data.txt7.isactif);
    textWidget7->setIsBorder(data.txt7.isborder);
    textWidget7->setText("TEXTE 7");
    // textWidget7->setBlinkFreq(2);

    textWidget8 = new TextWidget(_varioLanguage, data.txt8.altWidgetIndex, data.txt8.x, data.txt8.y, data.txt8.w, data.txt8.h);
    textWidget8->setIsActif(data.txt8.isactif);
    textWidget8->setIsBorder(data.txt8.isborder);
    textWidget8->setText("TEXTE 8");
    // textWidget8->setBlinkFreq(2);

    altiWidget = new AltiWidget(_varioLanguage, data.alti.altWidgetIndex, data.alti.x, data.alti.y, data.alti.w, data.alti.h);
    altiWidget->setIsActif(data.alti.isactif);
    altiWidget->setIsBorder(data.alti.isborder);

    varioWidget = new VarioWidget(_varioLanguage, data.vario.altWidgetIndex, data.vario.x, data.vario.y, data.vario.w, data.vario.h);
    varioWidget->setIsActif(data.vario.isactif);
    varioWidget->setIsBorder(data.vario.isborder);

    toolbarWidget = new ToolbarWidget(_varioLanguage, data.toolbar.altWidgetIndex, data.toolbar.x, data.toolbar.y, data.toolbar.w, data.toolbar.h);
    toolbarWidget->setIsActif(data.toolbar.isactif);
    toolbarWidget->setIsBorder(data.toolbar.isborder);

    speedWidget = new SpeedWidget(_varioLanguage, data.speed.altWidgetIndex, data.speed.x, data.speed.y, data.speed.w, data.speed.h);
    speedWidget->setIsActif(data.speed.isactif);
    speedWidget->setIsBorder(data.speed.isborder);

    bearingWidget = new BearingWidget(_varioLanguage, data.bearing.altWidgetIndex, data.bearing.x, data.bearing.y, data.bearing.w, data.bearing.h);
    bearingWidget->setIsActif(data.bearing.isactif);
    bearingWidget->setIsBorder(data.bearing.isborder);

    timeWidget = new TimeWidget(_varioLanguage, data.time.altWidgetIndex, data.time.x, data.time.y, data.time.w, data.time.h);
    timeWidget->setIsActif(data.time.isactif);
    timeWidget->setIsBorder(data.time.isborder);

    flightTimeWidget = new FlightTimeWidget(_varioLanguage, data.flighttime.altWidgetIndex, data.flighttime.x, data.flighttime.y, data.flighttime.w, data.flighttime.h);
    flightTimeWidget->setIsActif(data.flighttime.isactif);
    flightTimeWidget->setIsBorder(data.flighttime.isborder);

    latWidget = new LatWidget(_varioLanguage, data.lat.altWidgetIndex, data.lat.x, data.lat.y, data.lat.w, data.lat.h);
    latWidget->setIsActif(data.lat.isactif);
    latWidget->setIsBorder(data.lat.isborder);

    lonWidget = new LonWidget(_varioLanguage, data.lat.altWidgetIndex, data.lon.x, data.lon.y, data.lon.w, data.lon.h);
    lonWidget->setIsActif(data.lon.isactif);
    lonWidget->setIsBorder(data.lon.isborder);

    altiGpsWidget = new AltiGpsWidget(_varioLanguage, data.altigps.altWidgetIndex, data.altigps.x, data.altigps.y, data.altigps.w, data.altigps.h);
    altiGpsWidget->setIsActif(data.altigps.isactif);
    altiGpsWidget->setIsBorder(data.altigps.isborder);

    aglWidget = new AglWidget(_varioLanguage, data.agl.altWidgetIndex, data.agl.x, data.agl.y, data.agl.w, data.agl.h);
    aglWidget->setIsActif(data.agl.isactif);
    aglWidget->setIsBorder(data.agl.isborder);

    tabWidgets[nb_widgets++] = logoWidget;
    tabWidgets[nb_widgets++] = textWidget1;
    tabWidgets[nb_widgets++] = textWidget2;
    tabWidgets[nb_widgets++] = textWidget3;
    tabWidgets[nb_widgets++] = textWidget4;
    tabWidgets[nb_widgets++] = textWidget5;
    tabWidgets[nb_widgets++] = textWidget6;
    tabWidgets[nb_widgets++] = textWidget7;
    tabWidgets[nb_widgets++] = textWidget8;
    tabWidgets[nb_widgets++] = altiWidget;
    tabWidgets[nb_widgets++] = varioWidget;
    tabWidgets[nb_widgets++] = toolbarWidget;
    tabWidgets[nb_widgets++] = speedWidget;
    tabWidgets[nb_widgets++] = bearingWidget;
    tabWidgets[nb_widgets++] = timeWidget;
    tabWidgets[nb_widgets++] = flightTimeWidget;
    tabWidgets[nb_widgets++] = latWidget;
    tabWidgets[nb_widgets++] = lonWidget;
    tabWidgets[nb_widgets++] = altiGpsWidget;
    tabWidgets[nb_widgets++] = aglWidget;
}

uint8_t VarioScreen::getNbWidgets()
{
    return nb_widgets;
}

bool VarioScreen::isRefreshNeeded(uint32_t lastDisplayTime)
{
    for (uint8_t i = 0; i < getNbWidgets(); i++)
    {
        if (tabWidgets[i]->getIsActif() && tabWidgets[i]->isRefreshNeeded(lastDisplayTime))
        {
            // on marque comme modifié pour la prochaine boucle de  bufferTask de VarioDisplay
            tabWidgets[i]->setForceRefresh();
            return true;
        }
    }
    return false;
}

LogoWidget *VarioScreen::getLogoWidget()
{
    return logoWidget;
}

TextWidget *VarioScreen::getTextWidget1()
{
    return textWidget1;
}

TextWidget *VarioScreen::getTextWidget2()
{
    return textWidget2;
}

TextWidget *VarioScreen::getTextWidget3()
{
    return textWidget3;
}

TextWidget *VarioScreen::getTextWidget4()
{
    return textWidget4;
}

TextWidget *VarioScreen::getTextWidget5()
{
    return textWidget5;
}

TextWidget *VarioScreen::getTextWidget6()
{
    return textWidget6;
}

TextWidget *VarioScreen::getTextWidget7()
{
    return textWidget7;
}

TextWidget *VarioScreen::getTextWidget8()
{
    return textWidget8;
}

AltiWidget *VarioScreen::getAltiWidget()
{
    return altiWidget;
}

VarioWidget *VarioScreen::getVarioWidget()
{
    return varioWidget;
}

ToolbarWidget *VarioScreen::getToolbarWidget()
{
    return toolbarWidget;
}

SpeedWidget *VarioScreen::getSpeedWidget()
{
    return speedWidget;
}

BearingWidget *VarioScreen::getBearingWidget()
{
    return bearingWidget;
}

TimeWidget *VarioScreen::getTimeWidget()
{
    return timeWidget;
}

FlightTimeWidget *VarioScreen::getFlightTimeWidget()
{
    return flightTimeWidget;
}

LatWidget *VarioScreen::getLatWidget()
{
    return latWidget;
}

LonWidget *VarioScreen::getLonWidget()
{
    return lonWidget;
}

AltiGpsWidget *VarioScreen::getAltiGpsWidget()
{
    return altiGpsWidget;
}

AglWidget *VarioScreen::getAglWidget()
{
    return aglWidget;
}