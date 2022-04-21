#include "VarioDataType.h"
#include "VarioDebug/VarioDebug.h"

VarioDataType::VarioDataType()
{
}

VarioDataType::VarioDataType(const char *_constName, const char *_value)
{
    setName(_constName);
    dataType = TYPE_CHAR;
    strcpy(charValue, _value);
}

VarioDataType::VarioDataType(const char *_constName, int8_t _value)
{
    setName(_constName);
    dataType = TYPE_INT8;
    int8Value = _value;
}

VarioDataType::VarioDataType(const char *_constName, int16_t _value)
{
    setName(_constName);
    dataType = TYPE_INT16;
    int16Value = _value;
}

VarioDataType::VarioDataType(const char *_constName, uint8_t _value)
{
    setName(_constName);
    dataType = TYPE_UINT8;
    uint8Value = _value;
}

VarioDataType::VarioDataType(const char *_constName, uint16_t _value)
{
    setName(_constName);
    dataType = TYPE_UINT16;
    uint16Value = _value;
}

VarioDataType::VarioDataType(const char *_constName, float _value)
{
    setName(_constName);
    dataType = TYPE_FLOAT;
    floatValue = _value;
}

VarioDataType::VarioDataType(const char *_constName, double _value)
{
    setName(_constName);
    dataType = TYPE_DOUBLE;
    doubleValue = _value;
}

VarioDataType::VarioDataType(const char *_constName, bool _value)
{
    setName(_constName);
    dataType = TYPE_BOOL;
    boolValue = _value;
}

void VarioDataType::setName(const char *_constName)
{
    constName = (char *)malloc(strlen(_constName) * sizeof(_constName));
    strcpy(constName, _constName);
}

char *VarioDataType::getName()
{
    return constName;
}

template <typename T>
T VarioDataType::getValue()
{
    switch (dataType)
    {
    case TYPE_CHAR:
        return charValue;
        break;
    case TYPE_INT8:
        return int8Value;
        break;
    case TYPE_INT16:
        return int16Value;
        break;
    case TYPE_UINT8:
        return uint8Value;
        break;
    case TYPE_UINT16:
        return uint16Value;
        break;
    case TYPE_FLOAT:
        return floatValue;
        break;
    case TYPE_DOUBLE:
        return doubleValue;
        break;
    case TYPE_BOOL:
        return boolValue;
    default:
        //Serial.print("Bad type: ");
        //Serial.println(*constName);
        VARIO_DATA_DEBUG_PRINTLN("Bad type: ");
        VARIO_DATA_DEBUG_DUMP(*constName);
        return false;
    }
    return false;
}

int8_t VarioDataType::getValueInt8()
{
    if (dataType == TYPE_INT8)
    {
        return int8Value;
    }
    else
    {
 //       Serial.print("Bad type: ");
 //       Serial.println(constName);
        VARIO_DATA_DEBUG_PRINTLN("Bad type: ");
        VARIO_DATA_DEBUG_DUMP(*constName);
        return NULL;
    }
}

uint8_t VarioDataType::getValueUInt8()
{
    if (dataType == TYPE_UINT8)
    {
        return uint8Value;
    }
    else
    {
//        Serial.print("Bad type: ");
//        Serial.println(constName);
        VARIO_DATA_DEBUG_PRINTLN("Bad type: ");
        VARIO_DATA_DEBUG_DUMP(*constName);
        return NULL;
    }
}
int16_t VarioDataType::getValueInt16()
{
    if (dataType == TYPE_INT16)
    {
        return int16Value;
    }
    else
    {
//        Serial.print("Bad type: ");
//        Serial.println(constName);
        VARIO_DATA_DEBUG_PRINTLN("Bad type: ");
        VARIO_DATA_DEBUG_DUMP(*constName);
        return NULL;
    }
}
uint16_t VarioDataType::getValueUInt16()
{
    if (dataType == TYPE_UINT16)
    {
        return uint16Value;
    }
    else
    {
//        Serial.print("Bad type: ");
//        Serial.println(constName);
        VARIO_DATA_DEBUG_PRINTLN("Bad type: ");
        VARIO_DATA_DEBUG_DUMP(*constName);
        return NULL;
    }
}
float VarioDataType::getValueFloat()
{
    if (dataType == TYPE_FLOAT)
    {
        return floatValue;
    }
    else
    {
//        Serial.print("Bad type: ");
//        Serial.println(constName);
        VARIO_DATA_DEBUG_PRINTLN("Bad type: ");
        VARIO_DATA_DEBUG_DUMP(*constName);
        return NULL;
    }
}

bool VarioDataType::getValueBool()
{
    if (dataType == TYPE_BOOL)
    {
        return boolValue;
    }
    else
    {
//        Serial.print("Bad type: ");
//        Serial.println(constName);
        VARIO_DATA_DEBUG_PRINTLN("Bad type: ");
        VARIO_DATA_DEBUG_DUMP(*constName);
        return NULL;
    }
}

double VarioDataType::getValueDouble()
{
    if (dataType == TYPE_DOUBLE)
    {
        return doubleValue;
    }
    else
    {
//        Serial.print("Bad type: ");
//        Serial.println(constName);
        VARIO_DATA_DEBUG_PRINTLN("Bad type: ");
        VARIO_DATA_DEBUG_DUMP(*constName);
        return NULL;
    }
}

char *VarioDataType::getValueChar()
{
    if (dataType == TYPE_CHAR)
    {
        return charValue;
    }
    else
    {
//        Serial.print("Bad type: ");
//        Serial.println(*constName);
        VARIO_DATA_DEBUG_PRINTLN("Bad type: ");
        VARIO_DATA_DEBUG_DUMP(*constName);
        return nullptr;
    }
}

bool VarioDataType::setValue(const char *_value)
{
    switch (dataType)
    {
    case TYPE_CHAR:
        strcpy(charValue, _value);
        break;
    case TYPE_INT8:
        int8Value = atoi(_value);
        break;
    case TYPE_INT16:
        int16Value = atoi(_value);
        break;
    case TYPE_UINT8:
        uint8Value = strtol(_value, NULL, 10);
        break;
    case TYPE_UINT16:
        uint16Value = strtol(_value, NULL, 10);
        break;
    case TYPE_FLOAT:
        floatValue = atof(_value);
        break;
    case TYPE_DOUBLE:
        doubleValue = atof(_value);
        break;
    case TYPE_BOOL:
        if (strcmp(_value, "1") == 0 || strcmp(_value, "true") == 0 || strcmp(_value, "TRUE") == 0 || strcmp(_value, "1.00") == 0)
        {
            boolValue = true;
        }
        else
        {
            boolValue = false;
        }
        break;
    default:
//        Serial.print("Bad type: ");
//        Serial.println(*constName);
        VARIO_DATA_DEBUG_PRINTLN("Bad type: ");
        VARIO_DATA_DEBUG_DUMP(*constName);
        return false;
    }
    return true;
}

bool VarioDataType::setValue(int8_t _value)
{
    if (dataType == TYPE_INT8)
    {
        int8Value = _value;
        return true;
    }
//    Serial.print("Bad type: ");
//    Serial.println(*constName);
    VARIO_DATA_DEBUG_PRINTLN("Bad type: ");
    VARIO_DATA_DEBUG_DUMP(*constName);
    return false;
}

bool VarioDataType::setValue(int16_t _value)
{
    if (dataType == TYPE_INT16)
    {
        int16Value = _value;
        return true;
    }
//    Serial.print("Bad type: ");
//    Serial.println(*constName);
    VARIO_DATA_DEBUG_PRINTLN("Bad type: ");
    VARIO_DATA_DEBUG_DUMP(*constName);
    return false;
}

bool VarioDataType::setValue(uint8_t _value)
{
    if (dataType == TYPE_UINT8)
    {
        uint8Value = _value;
        return true;
    }
//    Serial.print("Bad type: ");
//    Serial.println(*constName);
    VARIO_DATA_DEBUG_PRINTLN("Bad type: ");
    VARIO_DATA_DEBUG_DUMP(*constName);
    return false;
}

bool VarioDataType::setValue(uint16_t _value)
{
    if (dataType == TYPE_UINT16)
    {
        uint16Value = _value;
        return true;
    }
//    Serial.print("Bad type: ");
//    Serial.println(*constName);
    VARIO_DATA_DEBUG_PRINTLN("Bad type: ");
    VARIO_DATA_DEBUG_DUMP(*constName);
    return false;
}

bool VarioDataType::setValue(float _value)
{
    if (dataType == TYPE_FLOAT)
    {
        floatValue = _value;
        return true;
    }
//    Serial.print("Bad type: ");
//    Serial.println(*constName);
    VARIO_DATA_DEBUG_PRINTLN("Bad type: ");
    VARIO_DATA_DEBUG_DUMP(*constName);
    return false;
}

bool VarioDataType::setValue(double _value)
{
    if (dataType == TYPE_DOUBLE)
    {
        doubleValue = _value;
        return true;
    }
//    Serial.print("Bad type: ");
//    Serial.println(*constName);
    VARIO_DATA_DEBUG_PRINTLN("Bad type: ");
    VARIO_DATA_DEBUG_DUMP(*constName);
    return false;
}

bool VarioDataType::setValue(bool _value)
{
    if (dataType == TYPE_BOOL)
    {
        boolValue = _value;
        return true;
    }
//    Serial.print("Bad type: ");
//    Serial.println(*constName);
    VARIO_DATA_DEBUG_PRINTLN("Bad type: ");
    VARIO_DATA_DEBUG_DUMP(*constName);
    return false;
}

bool VarioDataType::setParameterFromJsonObject(JsonObject *section, const char *key)
{
    bool isFromJson = false;
    const char *tVal;
    if (section->containsKey(key))
    {
        if ((tVal = section->getMember(key).as<const char *>()) != NULL)
        {
            setValue(tVal);
        }
        else
        {
            char tmpValue[10];
            dtostrf(section->getMember(key).as<double>(), 8, 2, tmpValue);
            tVal = trimSpace(tmpValue);
            setValue(tVal);
        }

        isFromJson = true;
    }

#ifdef SDCARD_DEBUG
    if (isFromJson)
    {
        Serial.print("Json Recup - ");
        Serial.print(key);
        Serial.print(" : ");

        switch (dataType)
        {
        case TYPE_CHAR:
            Serial.println(charValue);
            break;
        case TYPE_INT8:
            Serial.println(int8Value);
            break;
        case TYPE_INT16:
            Serial.println(int16Value);
            break;
        case TYPE_UINT8:
            Serial.println(uint8Value);
            break;
        case TYPE_UINT16:
            Serial.println(uint16Value);
            break;
        case TYPE_FLOAT:
            Serial.println(floatValue);
            break;
        case TYPE_DOUBLE:
            Serial.println(doubleValue);
            break;
        case TYPE_BOOL:
            Serial.println(boolValue);
            break;
        }
    }
    else
    {
        Serial.print("NO Recup - ");
    }
#endif

    return isFromJson;
}

char *VarioDataType::trimSpace(char *str)
{
    char *end;
    /* skip leading whitespace */
    while (isspace(*str))
    {
        str = str + 1;
    }
    /* remove trailing whitespace */
    end = str + strlen(str) - 1;
    while (end > str && isspace(*end))
    {
        end = end - 1;
    }
    /* write null character */
    *(end + 1) = '\0';
    return str;
}
