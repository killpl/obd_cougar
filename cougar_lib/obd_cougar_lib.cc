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
    
    ////////////////////////////////////////////////////////////////////////////////

    /*
    struct ParentDevice
    {
        // COMMON
        std::string name;           //> Device name (BTName, USB Product Name)
        DeviceType type;            //> Device type, also indicates possibly filled fields
        //  for parent device - USB or Bluetooth.
        
        // BLUETOOTH
        uint32_t channel;           //> Bluetooth channel number
        uint32_t connectionType;    //> Serial device type
        
        // USB
        uint32_t vendorId;          //> USB vendor ID
        uint32_t productId;         //> USB product ID
        uint32_t serialNumber;      //> USB device serial number
        
        std::string vendorName;     //> USB vendor name string
        
    };
    */
    
    ////////////////////////////////////////////////////////////////////////////////
    
    class Parent_Device : public Nan::ObjectWrap
    {
    public:
        static NAN_MODULE_INIT(Init)
        {
            LOGD << __PRETTY_FUNCTION__;
            
            v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
            tpl->SetClassName(Nan::New("Parent_Device").ToLocalChecked());
            tpl->InstanceTemplate()->SetInternalFieldCount(1);
            
            Isolate* isolate = v8::Isolate::GetCurrent();
            
            // Strings
            Nan::SetAccessor(tpl->InstanceTemplate(), v8::String::NewFromUtf8(isolate, "Name"), Name);
            Nan::SetAccessor(tpl->InstanceTemplate(), v8::String::NewFromUtf8(isolate, "VendorName"), VendorName);
            
            // Integers
            Nan::SetAccessor(tpl->InstanceTemplate(), v8::String::NewFromUtf8(isolate, "Channel"), Channel);
            Nan::SetAccessor(tpl->InstanceTemplate(), v8::String::NewFromUtf8(isolate, "ConnectionType"), ConnectionType);
            Nan::SetAccessor(tpl->InstanceTemplate(), v8::String::NewFromUtf8(isolate, "VendorID"), VendorID);
            Nan::SetAccessor(tpl->InstanceTemplate(), v8::String::NewFromUtf8(isolate, "ProductID"), ProductID);
            Nan::SetAccessor(tpl->InstanceTemplate(), v8::String::NewFromUtf8(isolate, "SerialNumber"), SerialNumber);
            
            constructor().Reset(tpl->GetFunction());
        }
        
        static v8::Local<v8::Object> NewObject(ParentDevice device)
        {
            LOGD << __PRETTY_FUNCTION__;
            
            Nan::EscapableHandleScope scope;
            v8::Local<v8::Object> instance = Nan::New<v8::Function>(constructor())->NewInstance();
            
            // Set internal data
            ObjectWrap::Unwrap<Parent_Device>(instance)->nativeDevice = device;
            return scope.Escape(instance);
        }
        
    public:
        
    #define NAN_GETTER_STRING(name, property) \
        NAN_GETTER(name) { \
        auto obj = ObjectWrap::Unwrap<Parent_Device>(info.Holder()); \
        info.GetReturnValue().Set(Nan::New(obj->nativeDevice.property.c_str()).ToLocalChecked()); \
    }
        
    #define NAN_GETTER_INTEGER(name, property) \
        NAN_GETTER(name) { \
        auto obj = ObjectWrap::Unwrap<Parent_Device>(info.Holder()); \
        info.GetReturnValue().Set(Nan::New(obj->nativeDevice.property)); \
    }
        
        static NAN_GETTER_STRING(Name,          name)
        static NAN_GETTER_STRING(VendorName,    vendorName)
        
        static NAN_GETTER_INTEGER(Channel,        channel)
        static NAN_GETTER_INTEGER(ConnectionType, connectionType)
        static NAN_GETTER_INTEGER(VendorID,       vendorId)
        static NAN_GETTER_INTEGER(ProductID,      productId)
        static NAN_GETTER_INTEGER(SerialNumber,   serialNumber)
        
    #undef NAN_GETTER_STRING
        
    private:
        explicit Parent_Device() {}
        virtual ~Parent_Device() {}
        
        static NAN_METHOD(New)
        {
            LOGD << __PRETTY_FUNCTION__;
            
            Parent_Device* obj = new Parent_Device();
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        }
        
        // Static field: constructor
        static inline Nan::Persistent<v8::Function> & constructor()
        {
            static Nan::Persistent<v8::Function> my_constructor;
            return my_constructor;
        }
        
        
        ParentDevice nativeDevice;
    };
    
    ////////////////////////////////////////////////////////////////////////////////
    
    class Serial_Device : public Nan::ObjectWrap
    {
    public:
        static NAN_MODULE_INIT(Init)
        {
            LOGD << __PRETTY_FUNCTION__;
            
            v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
            tpl->SetClassName(Nan::New("Serial_Device").ToLocalChecked());
            tpl->InstanceTemplate()->SetInternalFieldCount(1);
            
            Isolate* isolate = v8::Isolate::GetCurrent();
            
            Nan::SetAccessor(tpl->InstanceTemplate(), v8::String::NewFromUtf8(isolate, "Name"), Name);
            Nan::SetAccessor(tpl->InstanceTemplate(), v8::String::NewFromUtf8(isolate, "Class"), Class);
            Nan::SetAccessor(tpl->InstanceTemplate(), v8::String::NewFromUtf8(isolate, "CalloutDevice"), CalloutDevice);
            Nan::SetAccessor(tpl->InstanceTemplate(), v8::String::NewFromUtf8(isolate, "DialinDevice"), DialinDevice);
            Nan::SetAccessor(tpl->InstanceTemplate(), v8::String::NewFromUtf8(isolate, "Parent"), Parent);
            
            constructor().Reset(tpl->GetFunction());
        }
        
        static v8::Local<v8::Object> NewObject(SerialDevice device)
        {
            LOGD << __PRETTY_FUNCTION__;
            
            Nan::EscapableHandleScope scope;
            v8::Local<v8::Object> instance = Nan::New<v8::Function>(constructor())->NewInstance();
            
            // Set internal data
            ObjectWrap::Unwrap<Serial_Device>(instance)->nativeDevice = device;
            return scope.Escape(instance);
        }
        
    public:
        
    #define NAN_GETTER_STRING(name, property) \
        NAN_GETTER(name) { \
            auto obj = ObjectWrap::Unwrap<Serial_Device>(info.Holder()); \
            info.GetReturnValue().Set(Nan::New(obj->nativeDevice.property.c_str()).ToLocalChecked()); \
        }
    
        static NAN_GETTER_STRING(Name,          name)
        static NAN_GETTER_STRING(Class,         deviceClass)
        static NAN_GETTER_STRING(CalloutDevice, calloutDevice)
        static NAN_GETTER_STRING(DialinDevice,  dialinDevice)
        
        static NAN_GETTER(Parent) {
            auto obj = ObjectWrap::Unwrap<Serial_Device>(info.Holder());
            info.GetReturnValue().Set(Parent_Device::NewObject(obj->nativeDevice.parent));
        }
        
    #undef NAN_GETTER_STRING
        
    private:
        explicit Serial_Device() {}
        virtual ~Serial_Device() {}
        
        static NAN_METHOD(New)
        {
            LOGD << __PRETTY_FUNCTION__;
            
            Serial_Device* obj = new Serial_Device();
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        }
        
        // Static field: constructor
        static inline Nan::Persistent<v8::Function> & constructor()
        {
            static Nan::Persistent<v8::Function> my_constructor;
            return my_constructor;
        }


        SerialDevice nativeDevice;
    };

    ////////////////////////////////////////////////////////////////////////////////

    class Serial_Port : public Nan::ObjectWrap
    {
    public:
        static NAN_MODULE_INIT(Init)
        {
            LOGD << __PRETTY_FUNCTION__;
            
            v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
            tpl->InstanceTemplate()->SetInternalFieldCount(1);
            tpl->SetClassName(Nan::New("SerialPort").ToLocalChecked());
            
            Nan::SetPrototypeMethod(tpl, "GetDevices", GetDevices);
            
            constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
            Nan::Set(target, Nan::New("SerialPort").ToLocalChecked(),
                     Nan::GetFunction(tpl).ToLocalChecked());
        }
        
        static NAN_METHOD(GetDevices)
        {
            LOGD << __PRETTY_FUNCTION__;
            
            Isolate* isolate = info.GetIsolate();
            
            InterfacesOSX* interfaces = new InterfacesOSX();
            
            auto devices = interfaces->GetDevices();
            
            v8::Handle<v8::Array> array = v8::Array::New(isolate, devices.size());
            for (size_t i=0; i<devices.size(); i++)
            {
                LOGD << __PRETTY_FUNCTION__ << " Create object for device: " << devices[i].name;

                v8::Local<v8::Object> obj = Serial_Device::NewObject(devices[i]);
                array->Set(i, obj);
            }
            delete interfaces;
            
            info.GetReturnValue().Set(array);
        }
        
    private:
        explicit Serial_Port() {}
        virtual ~Serial_Port() {}
        
        static NAN_METHOD(New)
        {
            if (info.IsConstructCall()) {
                LOGD << __PRETTY_FUNCTION__ << " Constructor call";
                
                //Cougar_Bluetooth * obj = new Cougar_Bluetooth();
                //obj->Wrap(info.This());
                info.GetReturnValue().Set(info.This());
            } else {
                LOGD << __PRETTY_FUNCTION__ << " Execute call";
                
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = {info[0]};
                v8::Local<v8::Function> cons = Nan::New(constructor());
                info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
            }
        }
        
        // Static field: constructor
        static inline Nan::Persistent<v8::Function>& constructor()
        {
            static Nan::Persistent<v8::Function> my_constructor;
            return my_constructor;
        }

    };
    
    ////////////////////////////////////////////////////////////////////////////////
    
    void SetLogCallback(const FunctionCallbackInfo<Value>& args)
    {
        LOGD << __PRETTY_FUNCTION__;
        
        Isolate* isolate = args.GetIsolate();
        Local<Function> cb = Local<Function>::Cast(args[0]);
        
        // Store the function for future callbacks
        Log::LogFunction() = Persistent<Function, CopyablePersistentTraits<Function> >(isolate, cb);
        Log::LogLevel() = Log::LOG_LEVEL::eDEBUG;
        
        LOGD << "Log callback set.";
    }
    
    ////////////////////////////////////////////////////////////////////////////////

    // Mandatory init method
    NAN_MODULE_INIT(Init)
    {
        NODE_SET_METHOD(target, "SetLogCallback", SetLogCallback);
        
        Serial_Port::Init(target);
        Serial_Device::Init(target);
        Parent_Device::Init(target);
    }
    
    // Export macro
    NODE_MODULE(Cougar, Init)
 
}  // namespace Cougar
