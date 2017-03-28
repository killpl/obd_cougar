//
//  Logger.h
//  binding
//
//  Created by Adam Kozłowski on 07/03/2017.
//
//

#pragma once
#include <nan.h>

using v8::Function;
using v8::CopyablePersistentTraits;

#include <chrono>
#include <iomanip>
#include <sstream>

#define LOG(level) \
  if (level > Log::LogLevel()) ; \
  else Log().Get(level)

#define LOGD \
  LOG(Log::LOG_LEVEL::eDEBUG)

class Log
{
public:
    enum class LOG_LEVEL
    {
        eERROR,
        eWARNING,
        eINFO,
        eDEBUG
    };
    
    Log(){}
    
    virtual ~Log()
    {
        SendLog(logLevel, os.str());
    }
    
    std::ostringstream& Get(LOG_LEVEL level = LOG_LEVEL::eDEBUG)
    {
        logLevel = level;
        return os;
    }
    
    // Static field
    static v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function> >& LogFunction()
    {
        static v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function> > _logFunction;
        return _logFunction;
    }
    
    static LOG_LEVEL& LogLevel()
    {
        static LOG_LEVEL lvl;
        return lvl;
    }
    
protected:
    std::ostringstream os;

private:
    LOG_LEVEL logLevel;
    
    static void SendLog(LOG_LEVEL eLevel, const std::string& sText)
    {
        auto isolate = v8::Isolate::GetCurrent();
        v8::HandleScope scope(isolate);
        
        v8::Local<v8::Function> callback = v8::Local<Function>::Cast(LogFunction().Get(isolate));
        
        const unsigned argc = 1;
        v8::Local<v8::Value> argv[argc] = {
            v8::String::NewFromUtf8(isolate, (GetTime() + "\t" + EnumString(eLevel) + "\t" + sText).c_str())
        };
        callback->Call(v8::Null(isolate), argc, argv);
    }
    
    static std::string EnumString(LOG_LEVEL eLevel)
    {
        switch(eLevel)
        {
            case LOG_LEVEL::eERROR:     return "ERROR";
            case LOG_LEVEL::eWARNING:   return "WARNING";
            case LOG_LEVEL::eINFO:      return "INFO";
            case LOG_LEVEL::eDEBUG:     return "DEBUG";
        }
    }
    
    // http://stackoverflow.com/questions/17223096/outputting-date-and-time-in-c-using-stdchrono
    static std::string GetTime()
    {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        
        std::stringstream ss;
        // This might be localtime_r if there will be a thread safety problem
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
        return ss.str();
    }
};
