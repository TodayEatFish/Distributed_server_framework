/**
  ******************************************************************************
  * @file           : main.cpp
  * @author         : Zzzzzm
  * @brief          : 线程模块头文件.h
  * @attention      : None
  * @date           : 2023/4/19   20:47
  ******************************************************************************
  */



#ifndef __SYLAR_THREAD_H__
#define __SYLAR_THREAD_H__

#include <thread>
#include <functional>
#include <memory>
#include <string>
#include <pthread.h>

namespace sylar{
    class Thread{
        /**
            * @brief: 线程类
        */
    public:
        typedef std::shared_ptr<Thread> ptr;
        /**
            * @brief: 构造函数
        */
        Thread(std::function<void()> cb, const std::string& name);
        /**
            * @brief: 析构函数
        */
        ~Thread();

        tid_t getId() const { return m_id; }
        const std::string& getName() const{ return m_name; }

        void join();

        static Thread* GetThis();
        static const std::string& GetName();
        static void SetName(const std::string& name);   //写方法
    private:
        /**
            * @brief: 禁止使用拷贝
        */
        Thread(const Thread&) = delete;
        Thread(const Thread&&) = delete;
        Thread operator=(const Thread&) = delete;

        static void* run(void* arg);
    private:
        tid_t m_id = -1;
        pthread_t m_thread = 0;
        std::function<void()> m_cb;
        std::string m_name;
    };
}

#endif
