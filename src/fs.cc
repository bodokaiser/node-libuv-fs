#include "fs.h"
#include "v8.h"
#include "node.h"
#include "node_buffer.h"

using namespace v8;

using node::Buffer;

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

    if (!args[0]->IsString())
        return THROW_TYPE_ERROR("Path must be a string.");
    if (!args[1]->IsInt32())
        return THROW_TYPE_ERROR("Flags must be an integer.");
    if (!args[2]->IsInt32())
        return THROW_TYPE_ERROR("Mode must be an integer.");

    String::AsciiValue path(args[0]);
    int flags = args[1]->Int32Value();
    int mode = static_cast<int>(args[2]->Int32Value());

    uv_fs_t * request = (uv_fs_t*) malloc(sizeof(uv_fs_t));
    
    int fd = uv_fs_open(uv_default_loop(), request, * path, 
            flags, mode, NULL);

    if (fd < 0) return THROW_ERROR("Error opening file.");

    return scope.Close(Integer::New(fd));
}

Handle<Value>
ReadSync(const Arguments &args) {
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

    char * buffer_ptr = NULL;
    char * buffer_data = Buffer::Data(args[1]->ToObject());
    size_t buffer_length = Buffer::Length(args[1]->ToObject());
    size_t offset = args[2]->Int32Value();
    size_t length = args[3]->Int32Value();
    int64_t position = args[4]->NumberValue();
    
    if (offset > buffer_length)
        return THROW_ERROR("Offset is out of buffer range.");
    if (offset + length > buffer_length)
        return THROW_ERROR("Length is out of buffer range.");

    buffer_ptr = buffer_data + offset;

    uv_fs_t * req = (uv_fs_t *) malloc(sizeof(uv_fs_t));

    int read = uv_fs_read(uv_default_loop(), req, fd, buffer_ptr, 
            length, position, NULL);

    if (read < 0) return THROW_ERROR("Error on reading file.");

    return scope.Close(Integer::New(read));
}

Handle<Value>
CloseSync(const Arguments &args) {
    HandleScope scope;

    if (!args[0]->IsNumber())
        return THROW_TYPE_ERROR("First Argument must be a number.");

    int fd = args[0]->NumberValue();

    uv_fs_t * req = (uv_fs_t *) malloc(sizeof(uv_fs_t));

    int r = uv_fs_close(uv_default_loop(), req, fd, NULL);

    if (r < 0) return THROW_ERROR("Error closing file.");

    return scope.Close(Undefined());
}

NODE_MODULE(fs, Initialize)
