#ifndef LOGGING_HXX
#define LOGGING_HXX

#include <vector>
#include <cstdarg>
#include <cstdio>
#include <ctime>


#define _FILE_LINE_STR2(file, line) file ":" #line
#define _FILE_LINE_STR1(file, line) _FILE_LINE_STR2(file, line)
#define _FILE_LINE_STR() _FILE_LINE_STR1(__FILE__, __LINE__)


namespace logging {

#define LOG_GROUPS                              \
    DEFINE_LOG_GROUP(SHAPE)                     \
    DEFINE_LOG_GROUP(DIAGRAM)


enum LogGroup {
#define DEFINE_LOG_GROUP(GROUP) LOGGROUP_##GROUP,
    LOG_GROUPS
#undef DEFINE_LOG_GROUP
    LOGGROUP_MAX
};


enum LogLevel {
    LOGLEVEL_CRITICAL,
    LOGLEVEL_ERROR,
    LOGLEVEL_WARNING,
    LOGLEVEL_INFO,
    LOGLEVEL_DEBUG,
    LOGLEVEL_DEBUG2,
    LOGLEVEL_DEBUG3,
    LOGLEVEL_DEBUG4,
    LOGLEVEL_DEBUG5,
    LOGLEVEL_MAX
};

const char* loglevel_name(LogLevel level);    

const char* loggroup_name(LogGroup group);


struct LogMessageInfo {
    LogMessageInfo()
        :fileline(0), fmt(0)
    {
    }
    
    LogMessageInfo(const char* afileline, LogGroup agroup, LogLevel alevel, const char* afmt)
        :fileline(afileline), group(agroup), level(alevel), fmt(afmt)
    {
    }

    const char* fileline;
    LogGroup    group;
    LogLevel    level;
    const char* fmt;
};


struct LogEventInfo {
    static const int Max_Params = 4;
    
    LogMessageInfo message;
    char*          param[Max_Params];
};


class Dispatcher {
public:
    virtual bool filter(const LogMessageInfo& info) const = 0;
    virtual int vprintf(const LogMessageInfo& info, va_list va) = 0;
};



class EventDispatcher : public Dispatcher {
public:
    virtual bool filter(const LogMessageInfo& info) const;        
    virtual int vprintf(const LogMessageInfo& info, va_list va);
    
private:
    static const int Max_Events = 1024;
    static int m_ievent;
    static LogEventInfo m_events[Max_Events];
};

class StdioDispatcher : public Dispatcher {
public:
    StdioDispatcher(FILE* file);
        
    virtual bool filter(const LogMessageInfo& info) const;
        
    virtual int vprintf(const LogMessageInfo& info, va_list va);
        
private:
    static const int Max_Timestamp = 128;
        
    FILE*  m_file;
    char   m_timestamp[Max_Timestamp];
    time_t m_now;
};

class LogConfig {
public:
    typedef std::vector<Dispatcher*>   DispatchersType;

    LogConfig(LogLevel default_level=LOGLEVEL_INFO);
    ~LogConfig();

    LogLevel group_level(LogGroup group) const;
    void     group_level(LogGroup group, LogLevel level);
    
    DispatchersType& dispatchers();

private:
    const LogLevel           m_default_level;
    std::vector<LogLevel>    m_group_levels;
    std::vector<Dispatcher*> m_dispatchers;
    EventDispatcher*         m_event_dispatcher;
};
    

void sendLogMessage(const LogMessageInfo& info, ...);

}

#define DLOG(GROUP, LEVEL, FMT, ARGS...)                                \
    do {                                                                \
        static logging::LogMessageInfo _logmsg_info(                    \
            _FILE_LINE_STR(),                                           \
            logging::LOGGROUP_##GROUP, logging::LOGLEVEL_##LEVEL, FMT); \
        logging::sendLogMessage(_logmsg_info, ##ARGS);                  \
    } while (0)

#endif //LOGGING_HXX
