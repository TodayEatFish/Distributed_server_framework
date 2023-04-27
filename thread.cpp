/**
  ******************************************************************************
  * @file           : main.cpp
  * @author         : Zzzzzm
  * @brief          : 线程模块
  * @attention      : None
  * @date           : 2023/4/19   21:47
  ******************************************************************************
  */


#include "thread.h"
//#include "log.h"
namespace sylar {

    static thread_local Thread* t_thread = nullptr;
    static thread_local std::string t_thread_name = "UNKNOW";
    Thread* Thread::GetThis(){
        return t_thread;
    }
    const std::string& Thread::GetName(){
        return t_thread_name;
    }

    void Thread::SetName(const std::string &name) {
        if(t_thread) {
            t_thread->m_name = name;
        }
        t_thread_name = name;
    }


    Thread::Thread(std::function<void()> cb, const std::string &name) {
        if(name.empty()){
            m_name = "UNKNOW";
        }
        int rt = pthread_create(&m_thread, nullptr, &Thread::run, this);
        if(rt){

        }
    }
    Thread::~Thread() {

    }

    void* Thread::run(void *arg) {

    }
    const std::string& getName() const{ return m_name; }

    void join();

}