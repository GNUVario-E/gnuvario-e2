#pragma once

#include <Arduino.h>

class Observer;
class Subject
{
public:
    Subject();
    virtual ~Subject(){};
    void registerObserver(Observer *observer);
    void unregisterObserver(Observer *observer);

protected:
    Observer *mObserver[4];
    uint8_t observerIndex = 0;
    void _notifyObserver(uint8_t _val);

private:
    uint8_t id;
};
