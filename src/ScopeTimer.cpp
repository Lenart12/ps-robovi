//
// Created by lk3751 on 1/1/23.
//
#include "ScopeTimer.h"
#include <memory>


uint8_t ScopeTimer::s_callDepth = 0;

ScopeTimer::ScopeTimer(string name, bool bShowZero) :
    m_name(std::move(name)) , m_bShowZero(bShowZero){
    m_time = high_resolution_clock::now();
    s_callDepth++;
}

ScopeTimer::~ScopeTimer()
{
    auto end_time = high_resolution_clock::now();
    s_callDepth--;
    auto elapsed_ms = duration_cast<milliseconds>(end_time - m_time).count();
    if (elapsed_ms == 0 && !m_bShowZero) return;
    printf("|%*s%s timer finished after %dms\n", s_callDepth * 2, "", m_name.c_str(), elapsed_ms);
}
