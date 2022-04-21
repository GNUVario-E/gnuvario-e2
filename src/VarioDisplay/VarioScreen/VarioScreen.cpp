#include "VarioScreen.h"

VarioScreen::VarioScreen(ScreenData data)
{
    logoWidget = new LogoWidget(data.logo.x, data.logo.y, data.logo.w, data.logo.h, data.logo.iw, data.logo.ih);
    logoWidget->setIsActif(data.logo.isactif);
    logoWidget->setIsborder(data.logo.isborder);

    textWidget1 = new TextWidget(data.txt1.x, data.txt1.y, data.txt1.w, data.txt1.h);
    textWidget1->setIsActif(data.txt1.isactif);
    textWidget1->setIsborder(data.txt1.isborder);
    textWidget1->setText("TEXTE 1     ");
    textWidget1->setBlinkFreq(1);

    textWidget2 = new TextWidget(data.txt2.x, data.txt2.y, data.txt2.w, data.txt2.h);
    textWidget2->setIsActif(data.txt2.isactif);
    textWidget2->setIsborder(data.txt2.isborder);
    textWidget2->setText("TEXTE 2");
    textWidget2->setBlinkFreq(2);

    textWidget3 = new TextWidget(data.txt3.x, data.txt3.y, data.txt3.w, data.txt3.h);
    textWidget3->setIsActif(data.txt3.isactif);
    textWidget3->setIsborder(data.txt3.isborder);
    textWidget3->setText("TEXTE 3");
    textWidget3->setBlinkFreq(2);

    textWidget4 = new TextWidget(data.txt4.x, data.txt4.y, data.txt4.w, data.txt4.h);
    textWidget4->setIsActif(data.txt4.isactif);
    textWidget4->setIsborder(data.txt4.isborder);
    textWidget4->setText("TEXTE 4");
    textWidget4->setBlinkFreq(2);

    textWidget5 = new TextWidget(data.txt5.x, data.txt5.y, data.txt5.w, data.txt5.h);
    textWidget5->setIsActif(data.txt5.isactif);
    textWidget5->setIsborder(data.txt5.isborder);
    textWidget5->setText("TEXTE 5");
    textWidget5->setBlinkFreq(2);

    textWidget6 = new TextWidget(data.txt6.x, data.txt6.y, data.txt6.w, data.txt6.h);
    textWidget6->setIsActif(data.txt6.isactif);
    textWidget6->setIsborder(data.txt6.isborder);
    textWidget6->setText("TEXTE 6");
    textWidget6->setBlinkFreq(2);

    textWidget7 = new TextWidget(data.txt7.x, data.txt7.y, data.txt7.w, data.txt7.h);
    textWidget7->setIsActif(data.txt7.isactif);
    textWidget7->setIsborder(data.txt7.isborder);
    textWidget7->setText("TEXTE 7");
    textWidget7->setBlinkFreq(2);

    textWidget8 = new TextWidget(data.txt8.x, data.txt8.y, data.txt8.w, data.txt8.h);
    textWidget8->setIsActif(data.txt8.isactif);
    textWidget8->setIsborder(data.txt8.isborder);
    textWidget8->setText("TEXTE 8");
    textWidget8->setBlinkFreq(2);

    altWidget = new AltWidget(data.alt.x, data.alt.y, data.alt.w, data.alt.h);
    altWidget->setIsActif(data.alt.isactif);
    altWidget->setIsborder(data.alt.isborder);

    varioWidget = new VarioWidget(data.vario.x, data.vario.y, data.vario.w, data.vario.h);
    varioWidget->setIsActif(data.vario.isactif);
    varioWidget->setIsborder(data.vario.isborder);

    toolbarWidget = new ToolbarWidget(data.toolbar.x, data.toolbar.y, data.toolbar.w, data.toolbar.h);
    toolbarWidget->setIsActif(data.toolbar.isactif);
    toolbarWidget->setIsborder(data.toolbar.isborder);

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

bool VarioScreen::isRefreshNeeded()
{
    for (uint8_t i = 0; i < getNbWidgets(); i++)
    {
        if (tabWidgets[i]->getIsActif() && tabWidgets[i]->isRefreshNeeded())
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