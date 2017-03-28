#include <nan.h>

#include "../cougar_lib_commons/Logger.h"
#include "./serial/macOS/InterfacesOSX.h"

namespace Cougar {
    
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Object;
    using v8::String;
    using v8::Value;
    using v8::Function;
    using v8::Persistent;
    using v8::CopyablePersistentTraits;
    
    void Method(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
    }
    
    void SetLogCallback(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        Local<Function> cb = Local<Function>::Cast(args[0]);
        
        // Store the function for future callbacks
        Log::LogFunction() = Persistent<Function, CopyablePersistentTraits<Function> >(isolate, cb);
        Log::LogLevel() = Log::LOG_LEVEL::eDEBUG;
        
        LOGD << "Log callback set.";
        
        InterfacesOSX* interf = new InterfacesOSX();
        interf->GetDevices();
    }
    
    // Mandatory init method
    NAN_MODULE_INIT(Init)
    {
        NODE_SET_METHOD(target, "hello", Method);
        NODE_SET_METHOD(target, "SetLogCallback", SetLogCallback);
    }
    
    // Export macro
    NODE_MODULE(Cougar, Init)
 
}  // namespace Cougar
