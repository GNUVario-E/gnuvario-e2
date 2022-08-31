/* AgManager --
 *
 * Copyright 2020 JeromeV
 *
 * This file is part of GnuVario-E.
 *
 * ToneHAL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ToneHAL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 *********************************************************************************
 *                                                                               *
 *                           AgManager                                           *
 *                                                                               *
 *  version    Date     Description                                              *
 *    1.0    07/03/20                                                            *
 *    1.0.1  09/03/20   Ajout getGroundLevel                                     *
 *    1.0.2  12/03/20   Ajout getAgl                                             *
 *                                                                               *
 *********************************************************************************
 */

#include "AglManager.h"
#include "VarioDebug/VarioDebug.h"
#include <SD.h>
#define AGL_Directory "/AGL"

//****************************************************************************************************************************
AglManager::AglManager()
//****************************************************************************************************************************
{
    String tmpStr = AGL_Directory;
    tmpStr += "/";
    hgtReader = new HGTReader(tmpStr);
}

//****************************************************************************************************************************
bool AglManager::init(void)
//****************************************************************************************************************************
{
    char tmpFileName[15] = AGL_Directory;

    if (SD.exists(tmpFileName))
        Directory_AGL_Exists = true;
    else
        Directory_AGL_Exists = false;

    VARIO_AGL_DEBUG_PRINT("INIT AGL : Directory exists : ");
    VARIO_AGL_DEBUG_PRINTLN(Directory_AGL_Exists);

    return Directory_AGL_Exists;
}

//****************************************************************************************************************************
void AglManager::setAlti(double dAlti)
//****************************************************************************************************************************
{
    if (currentAlti != dAlti)
    {
        currentAlti = dAlti;
        computeHeight();
    }
}

//****************************************************************************************************************************
void AglManager::setAltiGps(double dAlti)
//****************************************************************************************************************************
{
    currentAltiGps = dAlti;
}

//****************************************************************************************************************************
void AglManager::setLatitude(double dLatitude)
//****************************************************************************************************************************
{
    if (currentLat != dLatitude)
    {
        currentLat = dLatitude;
        computeHeight();
    }
}

//****************************************************************************************************************************
void AglManager::setLongitude(double dLongitude)
//****************************************************************************************************************************
{
    if (currentLong != dLongitude)
    {
        currentLong = dLongitude;
        computeHeight();
    }
}

//****************************************************************************************************************************
void AglManager::computeHeight()
//****************************************************************************************************************************
{

    VARIO_AGL_DEBUG_PRINT("ComputeHeight ");
    VARIO_AGL_DEBUG_PRINT("Alti : ");
    VARIO_AGL_DEBUG_PRINT(currentAlti);
    VARIO_AGL_DEBUG_PRINT(", Lat : ");
    VARIO_AGL_DEBUG_PRINT(currentLat);
    VARIO_AGL_DEBUG_PRINT(", Long :");
    VARIO_AGL_DEBUG_PRINTLN(currentLong);

    if (IsOk())
    {
        if (currentAlti != -1 && currentLat != -1 && currentLong != -1 && currentLat != 0 && currentLong != 0)
        {
            groundLevel = hgtReader->getGroundLevel(degMinToDeg(currentLat), degMinToDeg(currentLong));
            if (groundLevel != NO_FILE_FOR_POS)
            {
                currentHeight = currentAlti - groundLevel;
            }
            else
            {
                currentHeight = currentAlti;
            }
        }
        else
        {
            currentHeight = currentAlti;
            groundLevel = -1;
        }
    }
    else
    {
        currentHeight = currentAlti;
        groundLevel = 1;
    }

    VARIO_AGL_DEBUG_PRINT("AGL Height ");
    VARIO_AGL_DEBUG_PRINTLN(currentHeight);
}

//****************************************************************************************************************************
int AglManager::getGroundLevel()
//****************************************************************************************************************************
{
    groundLevel = -1;

    if (IsOk())
    {
        if (currentAlti != -1 && currentLat != -1 && currentLong != -1 && currentLat != 0 && currentLong != 0)
        {
            groundLevel = hgtReader->getGroundLevel(degMinToDeg(currentLat), degMinToDeg(currentLong));
        }
    }

    return groundLevel;
}

//****************************************************************************************************************************
int AglManager::getAgl()
//****************************************************************************************************************************
{
    groundLevel = getGroundLevel();

    VARIO_AGL_DEBUG_PRINT("AGL Directory exists : ");
    VARIO_AGL_DEBUG_PRINTLN(Directory_AGL_Exists);

    if (groundLevel == -1 || currentAltiGps == -1)
        return -1;
    else
        return groundLevel - currentAltiGps;
}

//****************************************************************************************************************************
boolean AglManager::IsOk(void)
//****************************************************************************************************************************
{
    VARIO_AGL_DEBUG_PRINT("AGL Directory exists : ");
    VARIO_AGL_DEBUG_PRINTLN(Directory_AGL_Exists);

    return Directory_AGL_Exists;
}

/**
 \brief Fonction utilitaire : convertit un angle au format degrés.minutes vers degrés.decimales
 \param value : un angle au format degrés.minutes
 \return un angle en degrés.décimales
*/
//****************************************************************************************************************************
float AglManager::degMinToDeg(float value)
//****************************************************************************************************************************
{
    //    float r = value;
    int intValue = value;
    float min = value - intValue;
    float decimal = min / 0.6;
    return intValue + decimal;
}
