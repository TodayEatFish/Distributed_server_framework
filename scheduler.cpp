#include "scheduler.hpp"
#include "log.h"
#include "macro.h"

namespace sylar {
    static  sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");

    static thread_local Scheduler* t_scheduler = nullptr;
    static thread_local Fiber* t_fiber = nullptr;

    Scheduler::Scheduler(size_t threads, bool use_caller, const std::string& name) {
        SYLAR_ASSERT(threads > 0);

        if(use_caller) {
            sylar::Fiber::GetThis();
            --threds;

            SYLAR_ASSERT(GetThis() == nullptr);
            t_scheduler = this;

            m_rootFiber.reset(new Fiber(std::bind(&Scheduler::run, this)));
            sylar::Thread::SetName(m_name);

            t_fiber = m_rootFiber.get();
            m_rootThread = sylar::GetThreadId();
            m_threadIds.push_back(m_rootThread);
        } else {
            m_rootThread = -1;
        }
        m_threadCount = threads;
    }

    Scheduler::~Scheduler() {
        SYLAR_ASSERT(m_stopping);
        if(GetThis() == this) {
            t_scheduler = nullptr;
        }
    }

    Scheduler* Scheduler::GetThis() {
        return t_scheduler;
    }

    Fiber* Scheduler::GetMainFiber() {
        return t_fiber;
    }

    void Scheduler::start() {
        MutexType::Lock lock(m_mutex);
        if(!m_stopping) {
            return;
        }
        m_stopping = false;
        SYLAR_ASSERT(m_threads.empty());

        m_threads.resize(m_threadCount);
        for(size_t i = 0; i < m_threadCount; ++i) {
            m_threads[i].reset(new Thread(std::bind(&Scheduler::run, this)
                                          , m_name + "_" + std::to_string(i)));
            m_threadIds.push_back(m_threads[i]->getId());
        }
    }

    void Scheduler::stop() {
        m_autoStop = true;
        if(m_rootFiber
                && m_threadCount == 0
                && (m_rootFiber->getState() == Fiber::TERM
                    || m_rootFiber->getState() == Fiber::INIT)) {
            SYLAR_LOG_INFO(g_logger) << this << " stopped";
            m_stopping = true;

            if(stopping()) {
                return;
            }
        }

        bool exit_on_this_fiber = false;
        if(m_rootFiber != -1) {
            SYLAR_ASSERT(GetThis() == this);
        } else {
            SYLAR_ASSERT(GetThis() != this);
        }

        m_stopping = true;
        for (size_t i = 0; i < m_threadCount; ++i) {
            tickle();
        }

        if(m_rootFiber) {
            tickle();
        }

        if(stopping()) {
            return;
        }

//        if(exit_on_this_fiber) {
//        }
    }

    void Scheduler::setThis() {
        t_scheduler = this;
    }

    void Scheduler::run() {
        setThis();
        if(sylar::GetThreadId() != m_rootThread) {
            t_fiber = Fiber::GetThis().get();
        }

        Fiber::ptr idle_fiber(new Fiber(std::bind(&Scheduler::idle, this)));
        Fiber::ptr cb_fiber;

        FiberAndThread ft;
        while(true) {
            ft.reset();
            bool tickle_me = false;
            {
                MutexType::Lock lock(m_mutex);
                auto it = m_fibers.begin();
                while(it != m_fibers.end()) {
                    if(it->thread != -1 && it->thread != sylar::GetThread()) {

                    }
                }
            }
        }
    }

}