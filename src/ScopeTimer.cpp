//
// Created by lk3751 on 1/1/23.
//
#include "ScopeTimer.h"
#include <memory>


ScopeTimer::ScopeTimer(string name, bool bShowZero) :
    m_name(std::move(name)) , m_bShowZero(bShowZero){
    m_time = high_resolution_clock::now();
}

ScopeTimer::ScopeTimer(ScopeTimer &&other)
    : m_time(std::move(other.m_time))
    , m_name(std::move(other.m_name))
    , m_moved(std::move(other.m_moved))
    , m_bShowZero(std::move(other.m_bShowZero))
{
    other.m_moved = true;
}

ScopeTimer::~ScopeTimer()
{
    if (m_moved) return;
    auto end_time = high_resolution_clock::now();
    auto elapsed_ms = duration_cast<milliseconds>(end_time - m_time).count();
    if (elapsed_ms == 0 && !m_bShowZero) return;
    printf("%s timer finished after %dms\n", m_name.c_str(), elapsed_ms);
}
