#include "v8.h"
#include "node.h"

using namespace v8;

Handle<Value> OpenSync(const Arguments &args);
Handle<Value> ReadSync(const Arguments &args);
Handle<Value> CloseSync(const Arguments &args);

void
Initialize(Handle<Object> exports) {
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

    String::AsciiValue path(args[0]);

    uv_fs_t * request = (uv_fs_t*) malloc(sizeof(uv_fs_t));
    
    int fd = uv_fs_open(uv_default_loop(), request, * path, 
            O_RDWR, S_IRUSR, NULL);

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
