#ifndef FS_H
#define FS_H

#include "v8.h"
#include "node.h"
#include "req_wrap.h"

using namespace v8;

using node::ReqWrap;
            
class FSRequestWrap: public ReqWrap<uv_fs_t> {
    public:
        FSRequestWrap(const char * syscall)
            : syscall_(syscall) {
        }

        const char * syscall() {
            return syscall_;
        }
    private:
        const char * syscall_;
};

#define THROW_ERROR(message) \
    ThrowException(Exception::Error(String::New(message)));

#define THROW_TYPE_ERROR(message) \
    ThrowException(Exception::TypeError(String::New(message)));

static void After(uv_fs_t * req);
static Handle<Value> Open(const Arguments &args);
static Handle<Value> Read(const Arguments &args);
static Handle<Value> Close(const Arguments &args);

#define FS_SYNC_CALL(name, path, ...) \
    uv_fs_t * req = (uv_fs_t *) malloc(sizeof(uv_fs_t)); \
    int r = uv_fs_##name(uv_default_loop(), req, __VA_ARGS__, NULL); \
    if (r < 0) { \
        int code = uv_last_error(uv_default_loop()).code; \
        return ThrowException(UVException(code, #name, "", path)); \
    }

#define FS_SYNC_RESULT r

#define FS_ASYNC_CALL(name, cb, ...) \
    FSRequestWrap * req_wrap = new FSRequestWrap(#name); \
    int r = uv_fs_##name(uv_default_loop(), &req_wrap->req_, \
            __VA_ARGS__, After); \
    req_wrap->object_->Set(oncomplete_symbol, cb); \
    req_wrap->Dispatched(); \
    if (r < 0) { \
        uv_fs_t * req = &req_wrap->req_; \
        req->result = r; \
        req->path = NULL; \
        req->errorno = uv_last_error(uv_default_loop()).code; \
        After(req); \
    } \
    return scope.Close(req_wrap->object_);

#endif
