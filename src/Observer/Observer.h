#pragma once

class Observer
{
public:
    virtual ~Observer(){};
    virtual void onSignalReceived(uint8_t _val) = 0;
};