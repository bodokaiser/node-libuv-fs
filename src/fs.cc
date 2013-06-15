#include "fs.h"
#include "v8.h"
#include "node.h"
#include "node_buffer.h"

using namespace v8;

using node::Buffer;
using node::UVException;
using node::MakeCallback;

void
Initialize(Handle<Object> exports) {
    exports->Set(String::NewSymbol("open"), 
            FunctionTemplate::New(Open)->GetFunction());
    exports->Set(String::NewSymbol("read"),
            FunctionTemplate::New(Read)->GetFunction());
    exports->Set(String::NewSymbol("close"), 
            FunctionTemplate::New(Close)->GetFunction());
}

static Handle<Value>
Open(const Arguments &args) {
    HandleScope scope;

    if (!args[0]->IsString())
        return THROW_TYPE_ERROR("Path must be a string.");
    if (!args[1]->IsInt32())
        return THROW_TYPE_ERROR("Flags must be an integer.");
    if (!args[2]->IsInt32())
        return THROW_TYPE_ERROR("Mode must be an integer.");

    String::AsciiValue path(args[0]);
    int flags = args[1]->Int32Value();
    int mode = static_cast<int>(args[2]->Int32Value());

    if (args[3]->IsFunction()) {

    } else {
        FS_SYNC_CALL(open, * path, * path, flags, mode);
        return scope.Close(Integer::New(FS_SYNC_RESULT));
    }
}

static Handle<Value>
Read(const Arguments &args) {
    HandleScope scope;

    if (!args[0]->IsInt32())
        return THROW_TYPE_ERROR("File descriptor must be an integer.");
    if (!Buffer::HasInstance(args[1]))
        return THROW_TYPE_ERROR("Buffer must be a buffer.");
    if (!args[2]->IsInt32())
        return THROW_TYPE_ERROR("Offset must be an integer.");
    if (!args[3]->IsInt32())
        return THROW_TYPE_ERROR("Length must be an integer.");
    if (!args[4]->IsInt32())
        return THROW_TYPE_ERROR("Position must be an integer.");

    int fd = args[0]->Int32Value();
    
    char * buf_data = Buffer::Data(args[1]->ToObject());
    size_t buf_len = Buffer::Length(args[1]->ToObject());
    
    size_t off = args[2]->Int32Value();
    size_t len = args[3]->Int32Value();
    int64_t pos = args[4]->NumberValue();
    
    if (off > buf_len)
        return THROW_ERROR("Offset is out of buffer range.");
    if (off + len > buf_len)
        return THROW_ERROR("Length is out of buffer range.");

    if (args[5]->IsFunction()) {

    } else {
        FS_SYNC_CALL(read, 0, fd, buf_data + off, len, pos);
        return scope.Close(Integer::New(FS_SYNC_RESULT));
    }
}

static Handle<Value>
Close(const Arguments &args) {
    HandleScope scope;

    if (!args[0]->IsNumber())
        return THROW_TYPE_ERROR("First Argument must be a number.");

    int fd = args[0]->NumberValue();

    if (args[1]->IsFunction()) {

    } else {
        FS_SYNC_CALL(close, 0, fd);
        return scope.Close(Undefined());
    }
}

static void
After(uv_fs_t * req) {
    HandleScope scope;

    int argc = 1;
    Local<Value> argv[2];
    const Handle<Object> * cb = (Handle<Object> *) req->data;

    if (req->result < 0) {
        int code = uv_last_error(uv_default_loop()).code;
        argv[0] = UVException(code, "some fs io", "", req->path);
    } else {
        argc = 2;
        argv[0] = Local<Value>::New(Null());

        switch (req->fs_type) {
            case UV_FS_OPEN:
                argv[1] = Integer::New(req->result);
                break;
            case UV_FS_READ:
                argv[1] = Integer::New(req->result);
                break;
            default:
                assert(0 && "Unhandled IO response");
        }
    }

    MakeCallback(* cb, 0, argc, argv);

    uv_fs_req_cleanup(req);

    free(req);
}

NODE_MODULE(fs, Initialize)
