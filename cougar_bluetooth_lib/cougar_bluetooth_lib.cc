#include <nan.h>
#include <string>
#include <ctime>

namespace Cougar_Bluetooth {
    
    using v8::FunctionCallbackInfo;
    using v8::Function;
    using v8::CopyablePersistentTraits;
    using v8::Persistent;
    using v8::Isolate;
    using v8::Local;
    using v8::Object;
    using v8::String;
    using v8::Value;
    
    Persistent<Function, CopyablePersistentTraits<Function> > _LogFunction;

    void Method(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
    }
    
    
    void SetLogCallback(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        Local<Function> cb = Local<Function>::Cast(args[0]);
        
        // Store the function for future callbacks
        _LogFunction = Persistent<Function, CopyablePersistentTraits<Function> >(isolate, cb);
        
        // Call the function to indicate the start
        const unsigned argc = 1;
        Local<Value> argv[argc] = {
            String::NewFromUtf8(isolate, ("Cougar_Bluetooth logger started at " + std::to_string(time(NULL))).c_str())
        };
        cb->Call(Null(isolate), argc, argv);
    }
    
    // Mandatory init method
    void Init(Local<Object> exports) {
        NODE_SET_METHOD(exports, "hello", Method);
        NODE_SET_METHOD(exports, "SetLogCallback", SetLogCallback);
    }
    
    // Export macro
    NODE_MODULE(Cougar_Bluetooth, Init)
    
}  // namespace Cougar