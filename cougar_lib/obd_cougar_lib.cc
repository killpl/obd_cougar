#include <node.h>

namespace Cougar {
    
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Object;
    using v8::String;
    using v8::Value;
    
    void Method(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
    }
    
    // Mandatory init method
    void Init(Local<Object> exports) {
        NODE_SET_METHOD(exports, "hello", Method);
    }
    
    // Export macro
    NODE_MODULE(addon, Init)
    
}  // namespace Cougar