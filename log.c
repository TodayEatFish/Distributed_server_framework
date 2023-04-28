/**
  ******************************************************************************
  * @file           : log.cpp
  * @author         : kryiea
  * @brief          : 日志模块封装
  * @attention      : None
  * @date           : 2023/3/31  21:47
  ******************************************************************************
  */


#include <sstream>
#include <iostream>
#include "log.h"

namespace sylar{



    Logger::Logger(const std::string &name): m_name(name), m_level(LogLevel::DEBUG){

    }

    void Logger::addAppender(LogAppender::ptr appender) {
        m_appenders.push_back(appender);
    }

    void Logger::delAppender(LogAppender::ptr appender) {
        for (auto it = m_appenders.begin(); it != m_appenders.end(); ++it) {
            if(*it == appender){
                m_appenders.erase(it);
                break;
            }
        }
    }

    void Logger::log(LogLevel::Level level, LogEvent::ptr event) {
        if(level >= m_level){
            for (auto i : m_appenders) {
                i->log(level, event);
            }
        }
    }

    void Logger::debug(LogEvent::ptr event){
        log(LogLevel::DEBUG, event);

    }
    void Logger::info(LogEvent::ptr event){
        log(LogLevel::INFO, event);

    }
    void Logger::warn(LogEvent::ptr event){
        log(LogLevel::WARN, event);

    }
    void Logger::error(LogEvent::ptr event){
        log(LogLevel::ERROR, event);

    }
    void Logger::fatal(LogEvent::ptr event){
        log(LogLevel::FATAL, event);

    }


    std::string logFormatter::format(LogEvent::ptr event) {
        std::stringstream ss;
        for(auto& i: m_items){
            i->format(ss, event);
        }

    }

    void LogFormatter::init() {

    }

    LogFormatter::LogFormatter(const std::string &pattern)
        : m_pattern(pattern){

    }

    void StdoutLogAppender::log(LogLevel::Level level, LogEvent::ptr event) {
        if(level >= m_level){
            std::cout << m_formatter.format(event);
        }
    }
}