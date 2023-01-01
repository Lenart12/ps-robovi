//
// Created by lk3751 on 1/1/23.
//

#ifndef SCOPETIMER_H
#define SCOPETIMER_H


#include <string>
#include <chrono>

using std::string;
using namespace std::chrono;

#define FunctionTimer()         ScopeTimer __function_timer{__PRETTY_FUNCTION__}
#define FunctionZeroTimer()     ScopeTimer __function_zero_timer{__PRETTY_FUNCTION__, true}

class ScopeTimer
{
public:
    ScopeTimer(string name = "Unnamed timer", bool bShowZero = false);
    ~ScopeTimer();
private:
    time_point<high_resolution_clock> m_time;
    string m_name;
    bool m_bShowZero;

    static uint8_t s_callDepth;
};

#endif // SCOPETIMER_H
