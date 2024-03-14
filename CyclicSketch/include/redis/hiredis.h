#ifndef __HIREDIS_H
#define __HIREDIS_H

#include "read.h"
#include <stdarg.h>
#ifndef _MSC_VER
#include <sys/time.h>
#else
struct timeval;
typedef long long ssize_t;
#endif
#include <stdint.h>
#include "sds.h"
#include "alloc.h"

#define HIREDIS_MAJOR 1
#define HIREDIS_MINOR 0
#define HIREDIS_PATCH 2
#define HIREDIS_SONAME 1.0.0

#define REDIS_BLOCK 0x1
#define REDIS_CONNECTED 0x2
#define REDIS_DISCONNECTING 0x4
#define REDIS_FREEING 0x8
#define REDIS_IN_CALLBACK 0x10
#define REDIS_SUBSCRIBED 0x20
#define REDIS_MONITORING 0x40
#define REDIS_REUSEADDR 0x80
#define REDIS_NO_AUTO_FREE 0x200
#define REDIS_KEEPALIVE_INTERVAL 

#define REDIS_CONNECT_RETRIES  10

struct redisAsyncContext;
struct redisContext;

#define redisIsPushReply(r) (((redisReply*)(r))->type == REDIS_REPLY_PUSH)
typedef void (redisPushFn)(void *, void *);
typedef void (redisAsyncPushFn)(struct redisAsyncContext *, void *);

#ifdef __cplusplus
extern "C" {
#endif

typedef struct redisReply {
    int type; 
    long long integer; 
    double dval; 
    size_t len; 
    char *str; 
    char vtype[4]; 
    size_t elements; 
    struct redisReply **element; 
} redisReply;

redisReader *redisReaderCreate(void);

void freeReplyObject(void *reply);

int redisvFormatCommand(char **target, const char *format, va_list ap);
int redisFormatCommand(char **target, const char *format, ...);
int redisFormatCommandArgv(char **target, int argc, const char **argv, const size_t *argvlen);
int redisFormatSdsCommandArgv(sds *target, int argc, const char ** argv, const size_t *argvlen);
void redisFreeCommand(char *cmd);
void redisFreeSdsCommand(sds cmd);

enum redisConnectionType {
    REDIS_CONN_TCP,
    REDIS_CONN_UNIX,
    REDIS_CONN_USERFD
};

struct redisSsl;

#define REDIS_OPT_NONBLOCK 0x01
#define REDIS_OPT_REUSEADDR 0x02
#define REDIS_OPT_NOAUTOFREE 0x04
#define REDIS_OPT_NO_PUSH_AUTOFREE 0x08

#ifndef _WIN32
typedef int redisFD;
#define REDIS_INVALID_FD -1
#else
#ifdef _WIN64
typedef unsigned long long redisFD; 
#else
typedef unsigned long redisFD;      
#endif
#define REDIS_INVALID_FD ((redisFD)(~0)) 
#endif

typedef struct {
    int type;
    int options;
    const struct timeval *connect_timeout;
    const struct timeval *command_timeout;
    union {
        struct {
            const char *source_addr;
            const char *ip;
            int port;
        } tcp;
        const char *unix_socket;
        redisFD fd;
    } endpoint;
    void *privdata;
    void (*free_privdata)(void *);
    redisPushFn *push_cb;
    redisAsyncPushFn *async_push_cb;
} redisOptions;

#define REDIS_OPTIONS_SET_TCP(opts, ip_, port_) \
    (opts)->type = REDIS_CONN_TCP; \
    (opts)->endpoint.tcp.ip = ip_; \
    (opts)->endpoint.tcp.port = port_;

#define REDIS_OPTIONS_SET_UNIX(opts, path) \
    (opts)->type = REDIS_CONN_UNIX;        \
    (opts)->endpoint.unix_socket = path;

#define REDIS_OPTIONS_SET_PRIVDATA(opts, data, dtor) \
    (opts)->privdata = data;                         \
    (opts)->free_privdata = dtor;                    \

typedef struct redisContextFuncs {
    void (*free_privctx)(void *);
    void (*async_read)(struct redisAsyncContext *);
    void (*async_write)(struct redisAsyncContext *);
    ssize_t (*read)(struct redisContext *, char *, size_t);
    ssize_t (*write)(struct redisContext *);
} redisContextFuncs;


typedef struct redisContext {
    const redisContextFuncs *funcs;  
    int err; 
    char errstr[128]; 
    redisFD fd;
    int flags;
    char *obuf; 
    redisReader *reader; 
    enum redisConnectionType connection_type;
    struct timeval *connect_timeout;
    struct timeval *command_timeout;
    struct {
        char *host;
        char *source_addr;
        int port;
    } tcp;
    struct {
        char *path;
    } unix_sock;
    struct sockadr *saddr;
    size_t addrlen;
    void *privdata;
    void (*free_privdata)(void *);
    void *privctx;
    redisPushFn *push_cb;
} redisContext;

redisContext *redisConnectWithOptions(const redisOptions *options);
redisContext *redisConnect(const char *ip, int port);
redisContext *redisConnectWithTimeout(const char *ip, int port, const struct timeval tv);
redisContext *redisConnectNonBlock(const char *ip, int port);
redisContext *redisConnectBindNonBlock(const char *ip, int port, const char *source_addr);
redisContext *redisConnectBindNonBlockWithReuse(const char *ip, int port, const char *source_addr);
redisContext *redisConnectUnix(const char *path);
redisContext *redisConnectUnixWithTimeout(const char *path, const struct timeval tv);
redisContext *redisConnectUnixNonBlock(const char *path);
redisContext *redisConnectFd(redisFD fd);

int redisReconnect(redisContext *c);

redisPushFn *redisSetPushCallback(redisContext *c, redisPushFn *fn);
int redisSetTimeout(redisContext *c, const struct timeval tv);
int redisEnableKeepAlive(redisContext *c);
void redisFree(redisContext *c);
redisFD redisFreeKeepFd(redisContext *c);
int redisBufferRead(redisContext *c);
int redisBufferWrite(redisContext *c, int *done);
int redisGetReply(redisContext *c, void **reply);
int redisGetReplyFromReader(redisContext *c, void **reply);
int redisAppendFormattedCommand(redisContext *c, const char *cmd, size_t len);
int redisvAppendCommand(redisContext *c, const char *format, va_list ap);
int redisAppendCommand(redisContext *c, const char *format, ...);
int redisAppendCommandArgv(redisContext *c, int argc, const char **argv, const size_t *argvlen);
void *redisvCommand(redisContext *c, const char *format, va_list ap);
void *redisCommand(redisContext *c, const char *format, ...);
void *redisCommandArgv(redisContext *c, int argc, const char **argv, const size_t *argvlen);

#ifdef __cplusplus
}
#endif

#endif
