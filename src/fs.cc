#include "uv.h"
#include "v8.h"
#include "node.h"

using namespace v8;

Handle<Value> Read(const Arguments &args);

void
Initialize(Handle<Object> exports) {
    exports->Set(String::NewSymbol("read"), 
            FunctionTemplate::New(Read)->GetFunction());
}

Handle<Value>
Read(const Arguments &args) {
    HandleScope scope;

    if (!args[0]->IsString()) {
        ThrowException(Exception::TypeError(
                    String::New("First Argument must be a string.")));

        return scope.Close(Undefined());
    }

    if (!args[1]->IsFunction()) {
        ThrowException(Exception::TypeError(
                    String::New("Second Argument must be a function.")));

        return scope.Close(Undefined());
    }

    Local<String> string = args[0]->ToString();
    Local<Function> function = Handle<Function>::Cast(args[1]);

    // request loop, open file, read from file   

    return scope.Close(Undefined());
}

void
Callback(uv_fs_t * request) {
    int result = request->result;

    if (result < 0) {

    }
}

NODE_MODULE(fs, Initialize)
