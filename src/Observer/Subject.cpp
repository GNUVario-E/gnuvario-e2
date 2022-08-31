#include "Subject.h"
#include "Observer.h"

Subject::Subject()
{
}

void Subject::registerObserver(Observer *observer)
{
    mObserver[observerIndex] = observer;
    observerIndex = observerIndex + 1;
}

void Subject::unregisterObserver(Observer *observer)
{
    uint8_t removedIndex = 0;
    bool isFound = false;
    for (uint8_t i = 0; i < observerIndex; i++)
    {
        if (mObserver[i] == observer)
        {
            removedIndex = i;
            mObserver[i] = nullptr;
            isFound = true;
            break;
        }
    }

    if (isFound)
    {
        //  remove observer from array
        for (uint8_t i = removedIndex; i < observerIndex; i++)
        {
            mObserver[i] = mObserver[i + 1];
        }
        observerIndex = observerIndex - 1;
    }
}

void Subject::_notifyObserver(uint8_t _val)
{
    for (uint8_t i = 0; i < observerIndex; i++)
    {
        mObserver[i]->onSignalReceived(_val);
    }
}
