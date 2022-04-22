#pragma once

class Observer
{
public:
    virtual ~Observer(){};
    virtual void update(uint8_t _val) = 0;
};