#include "timer.h"
#include "util.h"

namespace sylar {

//  rewrite the compare function
bool Timer::Comparator::operator()(const Timer::ptr& lhs
                        ,const Timer::ptr& rhs) const {
    if(!lhs && !rhs) {
        return false;
    }
    if(!lhs) {
        return true;
    }
    if(!rhs) {
        return false;
    }
    if(lhs->m_next < rhs->m_next) {
        return true;
    }
    if(rhs->m_next < lhs->m_next) {
        return false;
    }
    return lhs.get() < rhs.get();
}

// set the timer class
Timer::Timer(uint64_t ms, std::function<void()> cb,
             bool recurring, TimerManager* manager)
    :m_recurring(recurring)
    ,m_ms(ms)
    ,m_cb(cb)
    ,m_manager(manager) {
    m_next = sylar::GetCurrentMS() + m_ms;
}

Timer::Timer(uint64_t next)
    :m_next(next) {
}


