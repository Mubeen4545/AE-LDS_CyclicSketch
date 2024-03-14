#ifndef __HIREDIS_ASYNC_H
#define __HIREDIS_ASYNC_H
#include "hiredis.h"

#ifdef __cplusplus
extern "C" {
#endif

struct redisAsyncContext;
struct dict;

typedef void (redisCallbackFn)(struct redisAsyncContext*, void*, void*);
typedef struct redisCallback {
    struct redisCallback *next;
    redisCallbackFn *fn;
    int pending_subs;
    void *privdata;
} redisCallback;

typedef struct redisCallbackList {
    redisCallback *head, *tail;
} redisCallbackList;

typedef void (redisDisconnectCallback)(const struct redisAsyncContext*, int status);
typedef void (redisConnectCallback)(const struct redisAsyncContext*, int status);
typedef void(redisTimerCallback)(void *timer, void *privdata);

typedef struct redisAsyncContext {
    redisContext c;
    int err;
    char *errstr;
    void *data;
    void (*dataCleanup)(void *privdata);
    struct {
        void *data;
        void (*addRead)(void *privdata);
        void (*delRead)(void *privdata);
        void (*addWrite)(void *privdata);
        void (*delWrite)(void *privdata);
        void (*cleanup)(void *privdata);
        void (*scheduleTimer)(void *privdata, struct timeval tv);
    } ev;
    redisDisconnectCallback *onDisconnect;
    redisConnectCallback *onConnect;
    redisCallbackList replies;
    struct sockaddr *saddr;
    size_t addrlen;
    struct {
        redisCallbackList invalid;
        struct dict *channels;
        struct dict *patterns;
    } sub;
    redisAsyncPushFn *push_cb;
} redisAsyncContext;

redisAsyncContext *redisAsyncConnectWithOptions(const redisOptions *options);
redisAsyncContext *redisAsyncConnect(const char *ip, int port);
redisAsyncContext *redisAsyncConnectBind(const char *ip, int port, const char *source_addr);
redisAsyncContext *redisAsyncConnectBindWithReuse(const char *ip, int port, const char *source_addr);
redisAsyncContext *redisAsyncConnectUnix(const char *path);
int redisAsyncSetConnectCallback(redisAsyncContext *ac, redisConnectCallback *fn);
int redisAsyncSetDisconnectCallback(redisAsyncContext *ac, redisDisconnectCallback *fn);
redisAsyncPushFn *redisAsyncSetPushCallback(redisAsyncContext *ac, redisAsyncPushFn *fn);
int redisAsyncSetTimeout(redisAsyncContext *ac, struct timeval tv);
void redisAsyncDisconnect(redisAsyncContext *ac);
void redisAsyncFree(redisAsyncContext *ac);
void redisAsyncHandleRead(redisAsyncContext *ac);
void redisAsyncHandleWrite(redisAsyncContext *ac);
void redisAsyncHandleTimeout(redisAsyncContext *ac);
void redisAsyncRead(redisAsyncContext *ac);
void redisAsyncWrite(redisAsyncContext *ac);
int redisvAsyncCommand(redisAsyncContext *ac, redisCallbackFn *fn, void *privdata, const char *format, va_list ap);
int redisAsyncCommand(redisAsyncContext *ac, redisCallbackFn *fn, void *privdata, const char *format, ...);
int redisAsyncCommandArgv(redisAsyncContext *ac, redisCallbackFn *fn, void *privdata, int argc, const char **argv, const size_t *argvlen);
int redisAsyncFormattedCommand(redisAsyncContext *ac, redisCallbackFn *fn, void *privdata, const char *cmd, size_t len);

#ifdef __cplusplus
}
#endif

#endif
