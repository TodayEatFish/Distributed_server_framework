/**
  ******************************************************************************
  * @file           : log.h
  * @author         : mimang
  * @brief          : 日志模块封装
  * @attention      : None
  * @date           : 2023/4/15 / 14:19
  ******************************************************************************
  */

#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__


#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdarg.h>
#include <map>
#include "util.h"
#include "singleton.h"

/**
 * @brief 使用流式方式将日志级别level的日志写入到logger
 */
#define SYLAR_LOG_LEVEL(logger, level) \
    if(logger->getLevel() <= level) \
        sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, sylar::GetThreadId(),\
                sylar::GetFiberId(), time(0), sylar::Thread::GetName()))).getSS()

/**
 * @brief 使用流式方式将日志级别debug的日志写入到logger
 */
#define SYLAR_LOG_DEBUG(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::DEBUG)

/**
 * @brief 使用流式方式将日志级别info的日志写入到logger
 */
#define SYLAR_LOG_INFO(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::INFO)

/**
 * @brief 使用流式方式将日志级别warn的日志写入到logger
 */
#define SYLAR_LOG_WARN(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::WARN)

/**
 * @brief 使用流式方式将日志级别error的日志写入到logger
 */
#define SYLAR_LOG_ERROR(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::ERROR)

/**
 * @brief 使用流式方式将日志级别fatal的日志写入到logger
 */
#define SYLAR_LOG_FATAL(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::FATAL)

/**
 * @brief 使用格式化方式将日志级别level的日志写入到logger
 */
#define SYLAR_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if(logger->getLevel() <= level) \
        sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, sylar::GetThreadId(),\
                sylar::GetFiberId(), time(0), sylar::Thread::GetName()))).getEvent()->format(fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别debug的日志写入到logger
 */
#define SYLAR_LOG_FMT_DEBUG(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::DEBUG, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别info的日志写入到logger
 */
#define SYLAR_LOG_FMT_INFO(logger, fmt, ...)  SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::INFO, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别warn的日志写入到logger
 */
#define SYLAR_LOG_FMT_WARN(logger, fmt, ...)  SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::WARN, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别error的日志写入到logger
 */
#define SYLAR_LOG_FMT_ERROR(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::ERROR, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别fatal的日志写入到logger
 */
#define SYLAR_LOG_FMT_FATAL(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::FATAL, fmt, __VA_ARGS__)

/**
 * @brief 获取主日志器
 */
#define SYLAR_LOG_ROOT() sylar::LoggerMgr::GetInstance()->getRoot()

/**
 * @brief 获取name的日志器
 */
#define SYLAR_LOG_NAME(name) sylar::LoggerMgr::GetInstance()->getLogger(name)

using namespace std;

namespace sylar{

    class Logger;

    /**
    * @brief: 日志级别
    */
    class LogLevel{
    public:
        enum Level{
            // 未知级别
            UNKNOW = 0,
            // DEBUG 级别
            DEBUG = 1,
            // INFO 级别
            INFO = 2,
            // WARN 级别
            WARN = 3,
            // ERROR 级别
            ERROR = 4,
            // FATAL 级别
            FATAL = 5
        };

        /**
        * @brief 将日志级别转成文本输出
        * @param[in] level 日志级别
        */
        static const char* ToString(LogLevel::Level level);
    };

    /**
  * @brief 日志事件
  */
    class LogEvent {
    public:
        typedef std::shared_ptr<LogEvent> ptr;
        /**
         * @brief 构造函数
         * @param[in] logger 日志器
         * @param[in] level 日志级别
         * @param[in] file 文件名
         * @param[in] line 文件行号
         * @param[in] elapse 程序启动依赖的耗时(毫秒)
         * @param[in] thread_id 线程id
         * @param[in] fiber_id 协程id
         * @param[in] time 日志事件(秒)
         * @param[in] thread_name 线程名称
         */
        LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level
                ,const char* file, int32_t line, uint32_t elapse
                ,uint32_t thread_id, uint32_t fiber_id, uint64_t time
                ,const std::string& thread_name);

        /**
         * @brief 返回文件名
         */
        const char* getFile() const { return m_file;}

        /**
         * @brief 返回行号
         */
        int32_t getLine() const { return m_line;}

        /**
         * @brief 返回耗时
         */
        uint32_t getElapse() const { return m_elapse;}

        /**
         * @brief 返回线程ID
         */
        uint32_t getThreadId() const { return m_threadId;}

        /**
         * @brief 返回协程ID
         */
        uint32_t getFiberId() const { return m_fiberId;}

        /**
         * @brief 返回时间
         */
        uint64_t getTime() const { return m_time;}

        /**
         * @brief 返回线程名称
         */
        const std::string& getThreadName() const { return m_threadName;}

        /**
         * @brief 返回日志内容
         */
        std::string getContent() const { return m_ss.str();}

        /**
         * @brief 返回日志器
         */
        std::shared_ptr<Logger> getLogger() const { return m_logger;}

        /**
         * @brief 返回日志级别
         */
        LogLevel::Level getLevel() const { return m_level;}

        /**
         * @brief 返回日志内容字符串流
         */
        std::stringstream& getSS() { return m_ss;}

        /**
         * @brief 格式化写入日志内容
         */
        void format(const char* fmt, ...);

        /**
         * @brief 格式化写入日志内容
         */
        void format(const char* fmt, va_list al);
    private:
        /// 文件名
        const char* m_file = nullptr;
        /// 行号
        int32_t m_line = 0;
        /// 程序启动开始到现在的毫秒数
        uint32_t m_elapse = 0;
        /// 线程ID
        uint32_t m_threadId = 0;
        /// 协程ID
        uint32_t m_fiberId = 0;
        /// 时间戳
        uint64_t m_time = 0;
        /// 线程名称
        std::string m_threadName;
        /// 日志内容流
        std::stringstream m_ss;
        /// 日志器
        std::shared_ptr<Logger> m_logger;
        /// 日志等级
        LogLevel::Level m_level;
    };

    /**
     * @brief 日志事件包装器
     */
    class LogEventWrap {
    public:

        /**
         * @brief 构造函数
         * @param[in] e 日志事件
         */
        LogEventWrap(LogEvent::ptr e);

        /**
         * @brief 析构函数
         */
        ~LogEventWrap();

        /**
         * @brief 获取日志事件
         */
        LogEvent::ptr getEvent() const { return m_event;}

        /**
         * @brief 获取日志内容流
         */
        std::stringstream& getSS();
    private:
        /**
         * @brief 日志事件
         */
        LogEvent::ptr m_event;
    };

    /**
    * @brief: 日志格式器
    */
    class LogFormatter{
    public:
        typedef std::shared_ptr<LogFormatter> ptr;
        LogFormatter(const std::string& pattern);

        //%t      %thread_id %m%n
        std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);

    public:
        class FormatItem{
        public:
            typedef std::shared_ptr<FormatItem> ptr;
            virtual ~FormatItem(){}
            virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
        };

        void init();
    private:
        std::string m_pattern;
        std::vector<FormatItem::ptr> m_items;
    };

    /**
    * @brief: 日志输出地
    */
    class LogAppender{
    public:
        typedef std::shared_ptr<LogAppender> ptr;
        virtual ~LogAppender(){}

        virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;

        void setFormatter(LogFormatter::ptr val) {m_formatter = val;}
        LogFormatter::ptr getFormatter() const {return m_formatter;}
    protected:
        LogLevel::Level m_level = LogLevel::DEBUG;
        LogFormatter::ptr m_formatter;
    };

    /**
    * @brief: 日志器
    */
    class Logger : public std::enable_shared_from_this<Logger>{
    public:
        typedef std::shared_ptr<Logger> ptr;

        Logger(const std::string& name = "root");
        void log(LogLevel::Level level, LogEvent::ptr event);

        void debug(LogEvent::ptr event);
        void info(LogEvent::ptr event);
        void warn(LogEvent::ptr event);
        void error(LogEvent::ptr event);
        void fatal(LogEvent::ptr event);

        void addAppender(LogAppender::ptr appender);
        void delAppender(LogAppender::ptr appender);
        LogLevel::Level getLevel() const{return m_level;}
        void setLevel(LogLevel::Level val) {m_level = val;}

        const std::string& getName() const {return m_name;}
    private:
        //日志名称
        std::string m_name;
        //日志级别
        LogLevel::Level m_level;
        //Appender集合
        std::list<LogAppender::ptr> m_appenders;
        LogFormatter::ptr m_formatter;
    };

    /**
    * @brief: 输出到控制台的Appender
    */
    class StdoutLogAppender : public LogAppender{
    public:
        typedef std::shared_ptr<StdoutLogAppender> ptr;
        void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;
    };

    /**
    * @brief: 定义输出到文件的Appender
    */
    class FileLogAppender : public LogAppender{
    public:
        typedef std::shared_ptr<FileLogAppender> ptr;
        FileLogAppender(const std::string& filename);
        void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

        //重新打开文件，打开文件成功返回true
        bool reopen();
    private:
        std::string m_filename;
        std::ofstream m_filestream;
    };
}

#endif