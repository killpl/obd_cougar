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
    
    class Serial_Port : public Nan::ObjectWrap {
    public:
        static NAN_MODULE_INIT(Init)
        {
            v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
            tpl->InstanceTemplate()->SetInternalFieldCount(1);
            tpl->SetClassName(Nan::New("SerialPort").ToLocalChecked());
            
            
            constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
            Nan::Set(target, Nan::New("SerialPort").ToLocalChecked(),
                     Nan::GetFunction(tpl).ToLocalChecked());
        }
        
    private:
        explicit Serial_Port()
        //: _cougarBluetooth(new Serial_Port())
        {}
        
        virtual ~Serial_Port() {}
        
        static NAN_METHOD(New)
        {
            if (info.IsConstructCall()) {
                //Cougar_Bluetooth * obj = new Cougar_Bluetooth();
                //obj->Wrap(info.This());
                info.GetReturnValue().Set(info.This());
            } else {
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = {info[0]};
                v8::Local<v8::Function> cons = Nan::New(constructor());
                info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
            }
        }
        
        // Static field: constructor
        static inline Nan::Persistent<v8::Function> & constructor()
        {
            static Nan::Persistent<v8::Function> my_constructor;
            return my_constructor;
        }

    };
    
    
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
