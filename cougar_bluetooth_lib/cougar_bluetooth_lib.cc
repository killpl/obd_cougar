#include <nan.h>
#include <string>
#include <ctime>

#include "../cougar_lib_commons/Logger.h"
#include "macOS/cougar_bluetooth.h"

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
    
    
    class Cougar_Bluetooth : public Nan::ObjectWrap {
    public:
        static NAN_MODULE_INIT(Init)
        {
            v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
            tpl->InstanceTemplate()->SetInternalFieldCount(1);
            tpl->SetClassName(Nan::New("Cougar_Bluetooth").ToLocalChecked());
            
            Nan::SetPrototypeMethod(tpl, "StartScan", StartScan);
            Nan::SetPrototypeMethod(tpl, "StopScan", StopScan);
            
            Nan::SetPrototypeMethod(tpl, "GetStatus", GetStatus);
            Nan::SetPrototypeMethod(tpl, "IsScanning", GetStatus);
            
            constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
            Nan::Set(target, Nan::New("Cougar_Bluetooth").ToLocalChecked(),
                     Nan::GetFunction(tpl).ToLocalChecked());
        }
        
        /**
         * Start bluetooth scan with return results callback.
         */
        static NAN_METHOD(StartScan)
        {
            Isolate* isolate = info.GetIsolate();
            
            if (!info[0]->IsUndefined() && !info[1]->IsUndefined())
            {
                Local<Function> callback = Local<Function>::Cast(info[1]);
                v8::String::Utf8Value sUuid(info[0]->ToString());
                
                Cougar_Bluetooth* obj = Nan::ObjectWrap::Unwrap<Cougar_Bluetooth>(info.Holder());
                obj->_callbackFunction = v8::Persistent<Function, v8::CopyablePersistentTraits<Function> >(isolate, callback);
                
                obj->_cougarBluetooth->StartScan(*sUuid, std::bind(&Cougar_Bluetooth::OnScanResult, obj, std::placeholders::_1));
            }
        }
        
        /**
         * Stop bluetooth scan.
         */
        static NAN_METHOD(StopScan)
        {
            Cougar_Bluetooth* obj = Nan::ObjectWrap::Unwrap<Cougar_Bluetooth>(info.Holder());
            obj->_cougarBluetooth->StopScan();
        }
        
        /**
         * Get bluetooth scan status (as string).
         */
        static NAN_METHOD(GetStatus)
        {
            
            Cougar_Bluetooth* obj = Nan::ObjectWrap::Unwrap<Cougar_Bluetooth>(info.Holder());
            
            switch(obj->_cougarBluetooth->GetStatus())
            {
                case CougarBluetoothStatus::Off:
                    info.GetReturnValue().Set(Nan::New<v8::String>("Off").ToLocalChecked());
                    break;
                case CougarBluetoothStatus::On:
                    info.GetReturnValue().Set(Nan::New<v8::String>("On").ToLocalChecked());
                    break;
                case CougarBluetoothStatus::Unknown:
                    info.GetReturnValue().Set(Nan::New<v8::String>("Unknown").ToLocalChecked());
                    break;
                case CougarBluetoothStatus::Unsupported:
                    info.GetReturnValue().Set(Nan::New<v8::String>("Unsupported").ToLocalChecked());
                    break;
                case CougarBluetoothStatus::Unauthorized:
                    info.GetReturnValue().Set(Nan::New<v8::String>("Unauthorized").ToLocalChecked());
                    break;
                case CougarBluetoothStatus::Resetting:
                    info.GetReturnValue().Set(Nan::New<v8::String>("Resetting").ToLocalChecked());
                    break;
            }
        }
        
        /**
         * Returns true if bluetooth scan is currently active.
         */
        static NAN_METHOD(IsScanning)
        {
            Cougar_Bluetooth* obj = Nan::ObjectWrap::Unwrap<Cougar_Bluetooth>(info.Holder());
            bool bResult = obj->_cougarBluetooth->IsScanning();
            info.GetReturnValue().Set(bResult);
        }
        
    private:
        explicit Cougar_Bluetooth()
        : _cougarBluetooth(new CougarBluetooth())
        {}
        
        ~Cougar_Bluetooth() {}
        
        static NAN_METHOD(New)
        {
            if (info.IsConstructCall()) {
                Cougar_Bluetooth * obj = new Cougar_Bluetooth();
                obj->Wrap(info.This());
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
        
        void OnScanResult(std::string sName)
        {
            auto isolate = v8::Isolate::GetCurrent();
            v8::HandleScope scope(isolate);
            
            v8::Local<v8::Function> callback = v8::Local<Function>::Cast(_callbackFunction.Get(isolate));
            
            const unsigned argc = 1;
            v8::Local<v8::Value> argv[argc] = {
                v8::String::NewFromUtf8(isolate, sName.c_str())
            };
            callback->Call(v8::Null(isolate), argc, argv);
        }
        
    private:
        std::unique_ptr<CougarBluetooth> _cougarBluetooth;
        v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function> > _callbackFunction;
    };
    void SetLogCallback(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        Local<Function> cb = Local<Function>::Cast(args[0]);
        
        // Store the function for future callbacks
        Log::LogFunction() = Persistent<Function, CopyablePersistentTraits<Function> >(isolate, cb);
 
        
        LOGD << "Log callback set.";
    }
    
    // Mandatory init method
    NAN_MODULE_INIT(Init)
    {
        NODE_SET_METHOD(target, "SetLogCallback", SetLogCallback);
        
        Cougar_Bluetooth::Init(target);
    }
    
    // Export macro
    NODE_MODULE(Cougar_Bluetooth, Init)
    
}  // namespace Cougar
