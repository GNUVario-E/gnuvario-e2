#ifndef _VARIO_DATATYPE_H
#define _VARIO_DATATYPE_H

#include "Arduino.h"
#include <ArduinoJson.h>

#define MAX_CHAR_LENGTH 50

#define TYPE_INT8 1U
#define TYPE_INT16 2U
#define TYPE_UINT8 3U
#define TYPE_UINT16 4U
#define TYPE_FLOAT 5U
#define TYPE_DOUBLE 6U
#define TYPE_BOOL 7U
#define TYPE_CHAR 8U

class VarioDataType
{
private:
    char *constName;
    uint8_t dataType;
    char charValue[MAX_CHAR_LENGTH];
    int8_t int8Value;
    uint8_t uint8Value;
    int16_t int16Value;
    uint16_t uint16Value;
    float floatValue;
    bool boolValue;
    double doubleValue;
    char *trimSpace(char *str);

public:
    template <typename T>
    T getValue();

    VarioDataType();
    VarioDataType(const char *_constName, const char *_value);
    VarioDataType(const char *_constName, int8_t _value);
    VarioDataType(const char *_constName, int16_t _value);
    VarioDataType(const char *_constName, uint8_t _value);
    VarioDataType(const char *_constName, uint16_t _value);
    VarioDataType(const char *_constName, float _value);
    VarioDataType(const char *_constName, double _value);
    VarioDataType(const char *_constName, bool _value);
    void setName(const char *_constName);

    char *getName();

    int8_t getValueInt8();
    uint8_t getValueUInt8();
    int16_t getValueInt16();
    uint16_t getValueUInt16();
    float getValueFloat();
    bool getValueBool();
    double getValueDouble();
    char *getValueChar();

    bool setValue(const char *_value);
    bool setValue(int8_t _value);
    bool setValue(int16_t _value);
    bool setValue(uint8_t _value);
    bool setValue(uint16_t _value);
    bool setValue(float _value);
    bool setValue(double _value);
    bool setValue(bool _value);

    bool setParameterFromJsonObject(JsonObject *section, const char *key);
};
#endif //_VARIO_DATATYPE_H