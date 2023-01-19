#pragma once

#include "Arduino.h"

#define PARAM_BT_ENABLE "BT_ENABLE"
#define PARAM_NO_RECORD "NO_RECORD"
#define PARAM_ALARM_SDCARD "ALARM_SDCARD"

#define PARAM_BT_ENABLE_DEFAULT false
#define PARAM_NO_RECORD_DEFAULT false
#define PARAM_ALARM_SDCARD_DEFAULT true

template <typename T>
class ConfigParameter
{
public:
    ConfigParameter(const char *key, T defaultValue) : key(key), value(defaultValue) {}
    void setValue(T _value);
    T getValue();
    const char *getKey() { return key; }

private:
    const char *key;
    T value;
};

template <typename T>
T ConfigParameter<T>::getValue()
{
    return value;
}

template <typename T>
void ConfigParameter<T>::setValue(T _value)
{
    value = _value;
}
