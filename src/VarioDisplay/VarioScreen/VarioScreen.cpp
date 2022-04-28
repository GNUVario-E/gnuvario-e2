#include "VarioScreen.h"

VarioScreen::VarioScreen(ScreenData data, VarioLanguage *_varioLanguage)
{
    logoWidget = new LogoWidget(_varioLanguage, data.logo.x, data.logo.y, data.logo.w, data.logo.h, data.logo.iw, data.logo.ih);
    logoWidget->setIsActif(data.logo.isactif);
    logoWidget->setIsBorder(data.logo.isborder);

    textWidget1 = new TextWidget(_varioLanguage, data.txt1.x, data.txt1.y, data.txt1.w, data.txt1.h);
    textWidget1->setIsActif(data.txt1.isactif);
    textWidget1->setIsBorder(data.txt1.isborder);
    textWidget1->setText("TEXTE 1");
    textWidget1->setBlinkFreq(1);

    textWidget2 = new TextWidget(_varioLanguage, data.txt2.x, data.txt2.y, data.txt2.w, data.txt2.h);
    textWidget2->setIsActif(data.txt2.isactif);
    textWidget2->setIsBorder(data.txt2.isborder);
    textWidget2->setText("TEXTE 2");
    textWidget2->setBlinkFreq(2);

    textWidget3 = new TextWidget(_varioLanguage, data.txt3.x, data.txt3.y, data.txt3.w, data.txt3.h);
    textWidget3->setIsActif(data.txt3.isactif);
    textWidget3->setIsBorder(data.txt3.isborder);
    textWidget3->setText("TEXTE 3");
    textWidget3->setBlinkFreq(2);

    textWidget4 = new TextWidget(_varioLanguage, data.txt4.x, data.txt4.y, data.txt4.w, data.txt4.h);
    textWidget4->setIsActif(data.txt4.isactif);
    textWidget4->setIsBorder(data.txt4.isborder);
    textWidget4->setText("TEXTE 4");
    textWidget4->setBlinkFreq(2);

    textWidget5 = new TextWidget(_varioLanguage, data.txt5.x, data.txt5.y, data.txt5.w, data.txt5.h);
    textWidget5->setIsActif(data.txt5.isactif);
    textWidget5->setIsBorder(data.txt5.isborder);
    textWidget5->setText("TEXTE 5");
    textWidget5->setBlinkFreq(2);

    textWidget6 = new TextWidget(_varioLanguage, data.txt6.x, data.txt6.y, data.txt6.w, data.txt6.h);
    textWidget6->setIsActif(data.txt6.isactif);
    textWidget6->setIsBorder(data.txt6.isborder);
    textWidget6->setText("TEXTE 6");
    textWidget6->setBlinkFreq(2);

    textWidget7 = new TextWidget(_varioLanguage, data.txt7.x, data.txt7.y, data.txt7.w, data.txt7.h);
    textWidget7->setIsActif(data.txt7.isactif);
    textWidget7->setIsBorder(data.txt7.isborder);
    textWidget7->setText("TEXTE 7");
    textWidget7->setBlinkFreq(2);

    textWidget8 = new TextWidget(_varioLanguage, data.txt8.x, data.txt8.y, data.txt8.w, data.txt8.h);
    textWidget8->setIsActif(data.txt8.isactif);
    textWidget8->setIsBorder(data.txt8.isborder);
    textWidget8->setText("TEXTE 8");
    textWidget8->setBlinkFreq(2);

    altWidget = new AltWidget(_varioLanguage, data.alt.x, data.alt.y, data.alt.w, data.alt.h);
    altWidget->setIsActif(data.alt.isactif);
    altWidget->setIsBorder(data.alt.isborder);

    varioWidget = new VarioWidget(_varioLanguage, data.vario.x, data.vario.y, data.vario.w, data.vario.h);
    varioWidget->setIsActif(data.vario.isactif);
    varioWidget->setIsBorder(data.vario.isborder);

    toolbarWidget = new ToolbarWidget(_varioLanguage, data.toolbar.x, data.toolbar.y, data.toolbar.w, data.toolbar.h);
    toolbarWidget->setIsActif(data.toolbar.isactif);
    toolbarWidget->setIsBorder(data.toolbar.isborder);

    tabWidgets[nb_widgets++] = logoWidget;
    tabWidgets[nb_widgets++] = textWidget1;
    tabWidgets[nb_widgets++] = textWidget2;
    tabWidgets[nb_widgets++] = textWidget3;
    tabWidgets[nb_widgets++] = textWidget4;
    tabWidgets[nb_widgets++] = textWidget5;
    tabWidgets[nb_widgets++] = textWidget6;
    tabWidgets[nb_widgets++] = textWidget7;
    tabWidgets[nb_widgets++] = textWidget8;
    tabWidgets[nb_widgets++] = altWidget;
    tabWidgets[nb_widgets++] = varioWidget;
    tabWidgets[nb_widgets++] = toolbarWidget;

    // addWidget(logoWidget);
    // addWidget(textWidget);
    // addWidget(textWidget2);
    // addWidget(altWidget);
    // addWidget(varioWidget);
}

// // add widget to tabWidgets array
// void VarioScreen::addWidget(LogoWidget *widget)
// {
//     tabWidgets[nb_widgets++] = widget;
// }

// void VarioScreen::addWidget(TextWidget *widget)
// {
//     tabWidgets[nb_widgets++] = widget;
// }

// void VarioScreen::addWidget(AltWidget *widget)
// {
//     tabWidgets[nb_widgets++] = widget;
// }

// void VarioScreen::addWidget(VarioWidget *widget)
// {
//     tabWidgets[nb_widgets++] = widget;
// }

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

AltWidget *VarioScreen::getAltWidget()
{
    return altWidget;
}

VarioWidget *VarioScreen::getVarioWidget()
{
    return varioWidget;
}

ToolbarWidget *VarioScreen::getToolbarWidget()
{
    return toolbarWidget;
}