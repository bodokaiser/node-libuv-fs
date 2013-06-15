#include "v8.h"
#include "node.h"

using namespace v8;

Handle<Value> OpenSync(const Arguments &args);
Handle<Value> ReadSync(const Arguments &args);
Handle<Value> CloseSync(const Arguments &args);

void
Initialize(Handle<Object> exports) {
    exports->Set(String::NewSymbol("O_CREAT"), Integer::New(O_CREAT));
    exports->Set(String::NewSymbol("O_RDONLY"), Integer::New(O_RDONLY));
    exports->Set(String::NewSymbol("O_WRONLY"), Integer::New(O_WRONLY));
    exports->Set(String::NewSymbol("O_RDWR"), Integer::New(O_RDWR));
    
    exports->Set(String::NewSymbol("openSync"), 
            FunctionTemplate::New(OpenSync)->GetFunction());
    exports->Set(String::NewSymbol("readSync"),
            FunctionTemplate::New(ReadSync)->GetFunction());
    exports->Set(String::NewSymbol("closeSync"), 
            FunctionTemplate::New(CloseSync)->GetFunction());
}

Handle<Value>
OpenSync(const Arguments &args) {
    HandleScope scope;

    if (!args[0]->IsString()) {
        ThrowException(Exception::TypeError(
                    String::New("First argument must be a string.")));

        return scope.Close(Undefined());
    }

    if (!args[1]->IsInt32()) {
        ThrowException(Exception::TypeError(
                    String::New("Second argument must be an integer.")));

        return scope.Close(Undefined());
    }

    if (!args[2]->IsInt32()) {
        ThrowException(Exception::TypeError(
                    String::New("Third argument must be an integer.")));

        return scope.Close(Undefined());
    }

    String::AsciiValue path(args[0]);
    int flags = args[1]->Int32Value();
    int mode = static_cast<int>(args[2]->Int32Value());

    uv_fs_t * request = (uv_fs_t*) malloc(sizeof(uv_fs_t));
    
    int fd = uv_fs_open(uv_default_loop(), request, * path, 
            flags, mode, NULL);

    if (fd < 0) {
        ThrowException(Exception::Error(
                    String::New("Error opening file.")));
    
        return scope.Close(Undefined());
    }

    return scope.Close(Integer::New(fd));
}

Handle<Value>
ReadSync(const Arguments &args) {
    HandleScope scope;

    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(
                    String::New("First argument must be fd.")));

        return scope.Close(Undefined());
    }

    return scope.Close(Undefined());
}

Handle<Value>
CloseSync(const Arguments &args) {
    HandleScope scope;

    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(
                    String::New("First Argument must be a number.")));

        return scope.Close(Undefined());
    }

    int fd = args[0]->NumberValue();

    uv_fs_t * req = (uv_fs_t *) malloc(sizeof(uv_fs_t));

    int r = uv_fs_close(uv_default_loop(), req, fd, NULL);

    if (r < 0) {
        ThrowException(Exception::Error(
                    String::New("Error closing file.")));

        return scope.Close(Undefined());
    }

    return scope.Close(Undefined());
}

NODE_MODULE(fs, Initialize)
