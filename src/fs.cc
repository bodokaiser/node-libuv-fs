#include "fs.h"
#include "v8.h"
#include "node.h"
#include "node_buffer.h"

using namespace v8;

using node::Buffer;
using node::UVException;

void
Initialize(Handle<Object> exports) {
    exports->Set(String::NewSymbol("open"), 
            FunctionTemplate::New(Open)->GetFunction());
    exports->Set(String::NewSymbol("read"),
            FunctionTemplate::New(Read)->GetFunction());
    exports->Set(String::NewSymbol("close"), 
            FunctionTemplate::New(Close)->GetFunction());
}

Handle<Value>
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

    FS_SYNC_CALL(open, * path, * path, flags, mode);

    return scope.Close(Integer::New(FS_SYNC_RESULT));
}

Handle<Value>
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

    FS_SYNC_CALL(read, 0, fd, buf_data + off, len, pos);

    return scope.Close(Integer::New(FS_SYNC_RESULT));
}

Handle<Value>
Close(const Arguments &args) {
    HandleScope scope;

    if (!args[0]->IsNumber())
        return THROW_TYPE_ERROR("First Argument must be a number.");

    int fd = args[0]->NumberValue();

    FS_SYNC_CALL(close, 0, fd);

    return scope.Close(Undefined());
}

NODE_MODULE(fs, Initialize)
