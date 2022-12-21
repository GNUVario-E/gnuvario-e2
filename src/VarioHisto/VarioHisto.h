#pragma once

#include <Arduino.h>

template <unsigned period, uint8_t count>
class VarioHisto
{
protected:
    static constexpr uint8_t altiValuesSize = count + 1;
    uint8_t altiValuesPos;
    double altiValues[count + 1];
    uint32_t lastAltiTimestamp;
    uint32_t lastSaveTimestamp;
    void saveAlti(double alti);
    double computeClimbDelta(uint8_t periodCount = count);
    bool haveClimbRate;
    double lastAlti;

public:
    void init(double alti, uint32_t timestamp);
    void setAlti(double alti, uint32_t timestamp);
    bool haveNewClimbRate(void);
    double getClimbRate(uint8_t periodCount = count);
};

template <unsigned period, uint8_t count>
void VarioHisto<period, count>::init(double firstAlti, uint32_t timestamp)
{
    for (uint8_t i = 0; i < altiValuesSize; i++)
    {
        altiValues[i] = firstAlti;
    }

    lastAltiTimestamp = timestamp;
    lastSaveTimestamp = timestamp;
}

template <unsigned period, uint8_t count>
void VarioHisto<period, count>::setAlti(double alti, uint32_t timestamp)
{
    Serial.println("setAlti");
    Serial.printf("Alti: %f timestamp: %d lastAltiTimestamp: %d lastSaveTimestamp: %d \n", alti, timestamp, lastAltiTimestamp, lastSaveTimestamp);
    /* check if time elasped */
    if (timestamp <= lastAltiTimestamp || timestamp <= lastSaveTimestamp)
    {
        return;
    }

    /* check if we need to save alti */
    if ((timestamp - lastSaveTimestamp) >= period)
    {
        /* udpate timestamp */
        lastSaveTimestamp += period; // is now the save timestamp !!

        double newAlti;
        if (lastSaveTimestamp <= lastAltiTimestamp)
        {
            newAlti = lastAlti;
        }
        else if (lastSaveTimestamp >= timestamp)
        {
            newAlti = alti;
        }
        else
        {
            /*compute linear interpolated alti */
            uint32_t remainingTime = lastSaveTimestamp - lastAltiTimestamp;
            newAlti = lastAlti + ((alti - lastAlti) * (double)(remainingTime) / (double)(timestamp - lastAltiTimestamp));
        }

        /* save */
        saveAlti(newAlti);
    }

    lastAlti = alti;
    lastAltiTimestamp = timestamp;
}

template <unsigned period, uint8_t count>
void VarioHisto<period, count>::saveAlti(double alti)
{

    altiValues[altiValuesPos] = alti;
    altiValuesPos = (altiValuesPos + 1) % altiValuesSize;

    /* we can compute a new climp rate */
    haveClimbRate = true;
}

template <unsigned period, uint8_t count>
bool VarioHisto<period, count>::haveNewClimbRate(void)
{

    return haveClimbRate;
}

template <unsigned period, uint8_t count>
double VarioHisto<period, count>::computeClimbDelta(uint8_t periodCount)
{

    /* get last alti position  */
    int8_t lastAltiPos = altiValuesPos - 1;
    if (lastAltiPos < 0)
    {
        lastAltiPos = altiValuesSize - 1;
    }

    /* get base alti position */
    int8_t baseAltiPos = lastAltiPos - periodCount;
    if (baseAltiPos < 0)
    {
        baseAltiPos += altiValuesSize;
    }

    /* compute climb delta */
    return altiValues[lastAltiPos] - altiValues[baseAltiPos];
}

template <unsigned period, uint8_t count>
double VarioHisto<period, count>::getClimbRate(uint8_t periodCount)
{

    haveClimbRate = false;
    return computeClimbDelta(periodCount) * 1000.0 / (double)((unsigned)periodCount * period); // max 65 seconds (unsigned max)
}

/*========================================================================================================*/

template <unsigned period, uint8_t count, uint8_t speedPeriodCount>
class SpeedHisto : public VarioHisto<period, count>
{
    using VarioHisto<period, count>::computeClimbDelta;
    using VarioHisto<period, count>::lastAlti;
    using VarioHisto<period, count>::lastSaveTimestamp;
    using VarioHisto<period, count>::saveAlti;

public:
    SpeedHisto(void) : speedValuesPos(0) {}
    double getGlideRatio(double speed, uint32_t timestamp, uint8_t periodCount = count);

private:
    static constexpr uint8_t speedValuesSize = (count + speedPeriodCount - 1) / speedPeriodCount;
    uint8_t speedValuesPos;
    double speedValues[(count + speedPeriodCount - 1) / speedPeriodCount];
};

template <unsigned period, uint8_t count, uint8_t speedPeriodCount>
double SpeedHisto<period, count, speedPeriodCount>::getGlideRatio(double speed, uint32_t timestamp, uint8_t periodCount)
{

    /* save speed value */
    speedValues[speedValuesPos] = speed;
    // pos updated later

    /*******************************/
    /* synchronise alti with speed */
    /*******************************/

    /* if we don't have the alti measure yet */
    /* take lastAlti                         */
    if (timestamp > lastSaveTimestamp && (timestamp - lastSaveTimestamp) > period / 2)
    {

        /* add alti value */
        saveAlti(lastAlti);
    }

    /* sync */
    lastSaveTimestamp = timestamp;

    /***********************/
    /* compute glide ratio */
    /***********************/
    unsigned sumCount = 0;

    double distance = 0.0;
    int8_t speedPos = speedValuesPos;

    while (sumCount < periodCount)
    {

        /* check if we need to update speed pos */
        if (sumCount % speedPeriodCount == 0)
        {

            speedPos -= 1;
            if (speedPos < 0)
            {
                speedPos = speedValuesSize - 1;
            }
        }

        /* add distance */
        distance += speedValues[speedPos];

        /* next period */
        sumCount++;
    }

    /* update pos */
    speedValuesPos = (speedValuesPos + 1) % speedValuesSize;

    /* compute glide ratio */
    return -distance * ((double)period / 3600.0) / computeClimbDelta(); // speed is in km/h and period in ms, so we get m
}