/* HGTReader --
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
 *                           HGTReader                                           *
 *                                                                               *
 *  version    Date     Description                                              *
 *    1.0    07/03/20                                                            *
 *                                                                               *
 *********************************************************************************
 */

#include "AglManager.h"
#include "HGTReader.h"
#include "VarioDebug/VarioDebug.h"

/**
  \brief Constructeur de classe.
  \param aglDir: répertoire contenant les fichiers HGT.
*/
//****************************************************************************************************************************
HGTReader::HGTReader(const String &aglDir) : aglDir(aglDir), currentFileName("")
//****************************************************************************************************************************
{
}

/**
  \brief Calcule la hauteur du sol à la position donnée.
  \param latitude: latitude de la position au format Degrés Minutes Secondes.
  \param longitude: longitude de la position au format Degrés Minutes Secondes.
  \return une altitude en mètres.
*/
//****************************************************************************************************************************
int HGTReader::getGroundLevel(float latitude, float longitude)
//****************************************************************************************************************************
{

  VARIO_AGL_DEBUG_PRINT("getGroundLevel(");
  VARIO_AGL_DEBUG_PRINT(latitude);
  VARIO_AGL_DEBUG_PRINT(",");
  VARIO_AGL_DEBUG_PRINT(longitude);
  VARIO_AGL_DEBUG_PRINTLN(")");

  String fileName = getFileNameForPosition(latitude, longitude);

  VARIO_AGL_DEBUG_PRINT("Filename :");
  VARIO_AGL_DEBUG_PRINTLN(fileName.c_str());

  if (fileName != currentFileName)
  {
    if (currentFileName != "")
    {
      VARIO_AGL_DEBUG_PRINT("Close :");
      VARIO_AGL_DEBUG_PRINTLN(currentFileName.c_str());

      currentFile.close();
    }
    if (openFile(fileName))
    {
      VARIO_AGL_DEBUG_PRINT("Open :");
      VARIO_AGL_DEBUG_PRINTLN(currentFileName.c_str());

      currentFileName = fileName;
    }
    else
    {
      VARIO_AGL_DEBUG_PRINTLN("Echec ouverture");

      currentFileName = "";
    }
  }
  if (currentFileName == "")
  {
    VARIO_AGL_DEBUG_PRINTLN("Pas de fichier");

    return NO_FILE_FOR_POS;
  }
  return loadGroundLevel(latitude, longitude);
}

/**
  \brief Génère le nom de fichier contenant la hauteur du sol à la position donnée.
  \param longitude: longitude de la position au format Degrés Minutes Secondes.
  \param latitude: latitude de la position au format Degrés Minutes Secondes.
  \return un nom de fichier.
*/
//****************************************************************************************************************************
String HGTReader::getFileNameForPosition(float latitude, float longitude)
//****************************************************************************************************************************
{
  int iLatDec = (int)floor(latitude);
  int iLonDec = (int)floor(longitude);
  char cLatDir = iLatDec >= 0 ? 'N' : 'S';
  char cLonDir = iLonDec >= 0 ? 'E' : 'W';
  char r[30];
  sprintf(r, "%c%02d%c%03d%s", cLatDir, abs(iLatDec), cLonDir, abs(iLonDec), String(FILE_EXTENSION).c_str());
  String tmp = String(r);

  VARIO_AGL_DEBUG_PRINTLN(tmp.c_str());

  return tmp;
}

/**
  \brief Ouvre un fichier dans le répertoire passé au constructeur de classe.
  \param file: le fichier à ouvrir.
  \return true si l'ouverture s'est bien passée.
*/
//****************************************************************************************************************************
bool HGTReader::openFile(const String &fileName)
//****************************************************************************************************************************
{
  String path = String(aglDir) + fileName;
  VARIO_AGL_DEBUG_PRINT("Path :");
  VARIO_AGL_DEBUG_PRINTLN(path.c_str());

  if ((currentFile = SD.open(path.c_str(), FILE_READ)))
  {
    VARIO_AGL_DEBUG_PRINTLN("Open OK");

    return true;
  }
  else
  {
    VARIO_AGL_DEBUG_PRINTLN("Open KO");

    return false;
  }
}

/**
  \brief Lit la hauteur du sol dans un fichier chargé.
  \param latitude: latitude de la position au format Degrés Minutes Secondes Direction.
  \param longitude: longitude de la position au format Degrés Minutes Secondes Direction.
  \return une altitude en mètres.
*/
//****************************************************************************************************************************
int HGTReader::loadGroundLevel(float latitude, float longitude)
//****************************************************************************************************************************
{
  int latDec = (int)floor(latitude);
  int lonDec = (int)floor(longitude);

  float secondsLat = (latitude - latDec) * NB_SEC_PER_DEG;
  float secondsLon = (longitude - lonDec) * NB_SEC_PER_DEG;

  int y = secondsLat / NB_SEC_PAR_TILE;
  int x = secondsLon / NB_SEC_PAR_TILE;

  int height[4];
  height[2] = getTileHeight(x, y);
  height[0] = getTileHeight(x, y + 1);
  height[3] = getTileHeight(x + 1, y);
  height[1] = getTileHeight(x + 1, y + 1);

  float dy = fmod(secondsLat, NB_SEC_PAR_TILE) / NB_SEC_PAR_TILE;
  float dx = fmod(secondsLon, NB_SEC_PAR_TILE) / NB_SEC_PAR_TILE;

  return height[0] * dy * (1 - dx) +
         height[1] * dy * (dx) +
         height[2] * (1 - dy) * (1 - dx) +
         height[3] * (1 - dy) * dx;
}

//****************************************************************************************************************************
int HGTReader::getTileHeight(int x, int y)
//****************************************************************************************************************************
{
  int offsetX = 2 * (x + (NB_PT_PER_ROW - y - 1) * NB_PT_PER_ROW);
  currentFile.seek(offsetX);
  // currentFile.seekSet(offsetX);
  // Serial.printf("offset = %d\n",offsetX);
  unsigned char b[2];
  currentFile.read(b, 2);
  int h = (b[0] << 8) + b[1];
  // Serial.printf("h = %d\n",h);
  return h;
}

/**
  \brief Fonction utilitaire : complète une chaine de caractères avec un caractère.
  \param src: la chaine à compléter.
  \param size: la taille de chaine à atteindre.
  \param c: le caractère pour compléter la chaine.
  \return une chaine de taille size (si src est moins longue que size)
*/
//****************************************************************************************************************************
String HGTReader::rightPad(const String &src, int size, char c)
//****************************************************************************************************************************
{
  String r = src;
  while (src.length() < size)
  {
    r = r + c;
  }
  return r;
}
