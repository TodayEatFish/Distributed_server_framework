/**
  ******************************************************************************
  * @file           : log.hpp
  * @author         : kryiea
  * @brief          : None
  * @attention      : None
  * @date           : 2023/3/31  20:26
  ******************************************************************************
  */

#ifndef DISTRIBUTED_SERVER_FRAMEWORK_LOG_H
#define DISTRIBUTED_SERVER_FRAMEWORK_LOG_H

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <fstream>
#include <vector>
//#include <stringstream>


namespace sylar{

//日志事件
class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent();
private:
    const char* m_file = nullptr;   //文件名
    int32_t m_line = 0;             //行号
    uint32_t m_elapse = 0;          //程序启动开始到现在的毫秒数
    uint32_t m_threadId = 0;        //线程id
    uint32_t m_fiberId = 0;         //协程id
    uint32_t m_time = 0;            //时间戳
    std::string m_content;
};

/**
  * @brief: 日志级别
*/
class LogLevel{
public:
    /**
      * @brief: 日志级别的枚举
    */
    enum Level{
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
};

/**
  * @brief: 日志格式器,将日志格式化
*/
class LogFormatter{
public:
    typedef std::shared_ptr<logFormatter> ptr;


    /**
      * @brief: 构造函数
      * @param[in]: pattern 格式模板
      * @details:
      *             %m 消息
      *             %p 日记等级
      *             %r 累计毫秒数
      *             %c 日志名称
      *             %t 线程id
      *             %n 换行
      *             %d 时间
      *             %f 文件名
      *             %l 行号
      *             %T 制表符
      *             %F 协程id
      *             %N 线程名
    */
    LogFormatter(const std::string& pattern);


    std::string format(LogEvent::ptr event);
private:
    class FormatItem{
    public:
        typedef std::shared_ptr<FormatItem> ptr;
        virtual ~FormatItem(){}
        virtual std::string format(LogEvent::ptr event) = 0;

    };

    void init();

private:
    std::string m_pattern;
    std::vector<FormatItem::ptr> m_items;
};

/**
  * @brief: 日志输出目标
*/
class LogAppender{
public:
    typedef std::shared_ptr<LogAppender> ptr;
    virtual ~LogAppender(){}

     virtual void log(LogLevel::Level level, LogEvent::ptr event) = 0;

protected:
    //日志级别
    LogLevel::Level m_level;
};

/**
  * @brief: 日志器
*/
class Logger : public std::enable_shared_from_this<Logger>{
friend class LoggerMananger;
public:
    typedef std::shared_ptr<Logger> ptr;
    typedef Spinlock MutexType;

    /**
      * @brief: 构造函数
      * @param[in] name 日志器名称
    */
    Logger(const std::string& name = "root");

    /**
      * @brief: 写日志
      * @param[in] level 日志级别
      * @param[in] event 日志事件
    */
    void log(LogLevel::Level level, LogEvent::ptr event);

    /**
     * @brief 写debug级别日志
     * @param[in] event 日志事件
     */
    void debug(LogEvent::ptr event);

    /**
     * @brief 写info级别日志
     * @param[in] event 日志事件
     */
    void info(LogEvent::ptr event);

    /**
     * @brief 写warn级别日志
     * @param[in] event 日志事件
     */
    void warn(LogEvent::ptr event);

    /**
    * @brief 写error级别日志
    * @param[in] event 日志事件
    */
    void error(LogEvent::ptr event);

    /**
     * @brief 写fatal级别日志
     * @param[in] event 日志事件
     */
    void fatal(LogEvent::ptr event);

    /**
    * @brief 添加日志目标
    * @param[in] appender 日志目标
    */
    void addAppender(LogAppender::ptr appender);

    /**
     * @brief 删除日志目标
     * @param[in] appender 日志目标
     */
    void delAppender(LogAppender::ptr appender);
    
    /**
      * @brief: 清空日志目标
    */
    void clearAppender();

    /**
     * @brief 返回日志级别
     */
    LogLevel::Level getLevel() const { return m_level; }

    /**
    * @brief 设置日志级别
    */
    void setLevel(LogLevel::Level val) { m_level = val; }

    /**
      * @brief: 返回日志名称
    */
    const std::string & getname(){return m_name};

    

private:
    std::string m_name;                             //日志名称
    LogLevel::Level m_level;                        //日志级别
    std::list<LogAppender::ptr> m_appenders;        //Appender集合
};






/**
  * @brief: 输出到控制台的Appender
*/
class StdoutLogAppender : public LogAppender{
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    virtual void log(LogLevel::Level level, LogEvent::ptr event) override;


};

/**
  * @brief: 输出到文件的Appender
*/
class FlieLogAppender : public LogAppender{
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    FlieLogAppender(const std::string& filename);

    virtual void log(LogLevel::Level level, LogEvent::ptr event) override;

    /**
      * @brief: 重新打开日志文件
      * @return: 成功返回true
    */
    bool reopen();
private:
    // 文件路径
    std::string m_filename;
    // 文件流
    std::ofstream m_filestream;
    // 上次打开的时间
    uint64_t m_lastTime = 0;

};



/**
  * @brief: 日志管理类
*/
class LoggerManager{
public:
    typedef Spinlock MutexType;

};




















}






















#endif //DISTRIBUTED_SERVER_FRAMEWORK_LOG_H
