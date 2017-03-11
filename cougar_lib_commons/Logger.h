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

class Log
{
public:
    
    /**
     * Log error message using callback provided by LogFunction().
     */
    static void LOGE(std::string sText)
    {
        LOG(LOG_LEVEL::eERROR, sText);
    }

    /**
     * Log debug message using callback provided by LogFunction().
     */
    static void LOGD(std::string sText)
    {
        LOG(LOG_LEVEL::eDEBUG, sText);
    }
    
    // Static field
    static v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function> >& LogFunction()
    {
        static v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function> > _logFunction;
        return _logFunction;
    }
    
private:
    enum class LOG_LEVEL
    {
        eERROR,
        eWARNING,
        eINFO,
        eDEBUG
        
    };
    
    static void LOG(LOG_LEVEL eLevel, const std::string& sText)
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
