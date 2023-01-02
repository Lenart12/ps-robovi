//
// Created by lk3751 on 1/1/23.
//

#ifndef SCOPETIMER_H
#define SCOPETIMER_H


#include <string>
#include <chrono>
#include <pthread.h>

using std::string;
using namespace std::chrono;

#define FunctionTimer()         ScopeTimer __function_timer{__PRETTY_FUNCTION__, true}
#define FunctionNonzeroTimer()     ScopeTimer __function_zero_timer{__PRETTY_FUNCTION__, false}

class ScopeTimer
{
public:
    ScopeTimer(string name = "Unnamed timer", bool bShowZero = false);
    ScopeTimer(ScopeTimer&& other);
    ~ScopeTimer();
private:
    time_point<high_resolution_clock> m_time;
    string m_name;
    bool m_moved = false;
    bool m_bShowZero;
};

#endif // SCOPETIMER_H
