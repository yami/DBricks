#include "logging.hxx"
#include <util/stl.hxx>
#include <util/assert.hxx>


namespace logging {

int EventDispatcher::m_ievent = 0;
LogEventInfo EventDispatcher::m_events[EventDispatcher::Max_Events];



LogConfig gLogConfig;
    
static const char* Log_Group_Name[] = {
#define DEFINE_LOG_GROUP(GROUP) #GROUP,
    LOG_GROUPS
#undef DEFINE_LOG_GROUP
};

const char*
loggroup_name(LogGroup group)
{
    return Log_Group_Name[group];
}

const char*
loglevel_name(LogLevel level)
{
    switch (level) {
        case LOGLEVEL_CRITICAL:
            return "CRITICAL";
        case LOGLEVEL_ERROR:
            return "ERROR";
        case LOGLEVEL_WARNING:
            return "WARNING";
        case LOGLEVEL_INFO:
            return "INFO";
        case LOGLEVEL_DEBUG:
            return "DEBUG";
        case LOGLEVEL_DEBUG2:
            return "DEBUG2";
        case LOGLEVEL_DEBUG3:
            return "DEBUG3";
        case LOGLEVEL_DEBUG4:
            return "DEBUG4";
        case LOGLEVEL_DEBUG5:
            return "DEBUG5";
        default:
            ASSERT_NOT_REACHED();
    }
}
    
void
sendLogMessage(const LogMessageInfo& info, ...)
{
    std::vector<Dispatcher *>& ds = gLogConfig.dispatchers();
    std::vector<Dispatcher *>::iterator diter = ds.begin();
    for (;diter != ds.end(); ++diter) {
        va_list va;
        va_start(va, info);
        if ((*diter)->filter(info)) {
            (*diter)->vprintf(info, va);
        }
        va_end(va);
    }
}

    
    
LogConfig::LogConfig(LogLevel default_level)
    :m_default_level(default_level), m_group_levels(LOGGROUP_MAX)
{
    fill(m_group_levels.begin(), m_group_levels.end(), m_default_level);

    m_event_dispatcher = new EventDispatcher();
        
    m_dispatchers.push_back(m_event_dispatcher);
    m_dispatchers.push_back(new StdioDispatcher(stderr));
}

LogConfig::~LogConfig()
{
    util::delete_entries(m_dispatchers.begin(), m_dispatchers.end());
}

LogLevel
LogConfig::group_level(LogGroup group) const
{
    return m_group_levels[group];
}

void
LogConfig::group_level(LogGroup group, LogLevel level)
{
    m_group_levels[group] = level;
}

LogConfig::DispatchersType&
LogConfig::dispatchers()
{
    return m_dispatchers;
}

bool
EventDispatcher::filter(const LogMessageInfo& info) const
{
    return true;
}

int
EventDispatcher::vprintf(const LogMessageInfo& info, va_list va)
{
    int curr = (m_ievent++)%Max_Events;
        
    m_events[curr].message = info;

    for (int i=0; i<LogEventInfo::Max_Params; ++i) {
        m_events[curr].param[i] = va_arg(va, char*);
    }

    return 0;
}

StdioDispatcher::StdioDispatcher(FILE* file)
    :m_file(file), m_now(0)
{
}

bool
StdioDispatcher::filter(const LogMessageInfo& info) const
{
    return gLogConfig.group_level(info.group) >= info.level;
}

int
StdioDispatcher::vprintf(const LogMessageInfo& info, va_list va)
{
    time_t t = time(NULL);

    if (t != m_now) {
        struct tm* ptm = localtime(&t);
        strftime(m_timestamp, Max_Timestamp-1, "%m/%d/%H:%M:%S", ptm);
        m_now = t;
    }
            
    std::fprintf(m_file, "%s|%s|%s|%s|",
                 m_timestamp, loggroup_name(info.group),
                 loglevel_name(info.level), info.fileline);
    std::vfprintf(m_file, info.fmt, va);
    return std::fprintf(m_file, "\n");
}


} // namespace logging
