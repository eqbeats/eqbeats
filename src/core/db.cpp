#include "db.h"
#include "path.h"
#include <stdarg.h>
#include <libpq-fe.h>

#ifdef HAVE_LIBHIREDIS
#  include <hiredis/hiredis.h>
#endif

using namespace DB;

PGconn *db;

#ifdef HAVE_LIBHIREDIS
static std::string name;
static struct redisContext *redis_ctx = NULL;

struct redisContext* DB::redis() {
    return redis_ctx;
}

void DB::blindRedisCommand(const std::string &q, ...){
    va_list ap;
    va_start(ap, q);
    void* r = redisvCommand(redis_ctx, q.c_str(), ap);
    if(r != NULL) freeReplyObject(r);
}

static void connectRedis(std::string name){
    redis_ctx = redisConnectUnix(REDIS_SOCK);
    if(!name.empty()){
        ::name = name;
        blindRedisCommand("CLIENT SETNAME %s", name.c_str());
    }
    else if(!::name.empty())
        blindRedisCommand("CLIENT SETNAME %s", ::name.c_str());
}
#endif

void DB::connect(std::string name){
    db = PQconnectdb("");
#ifdef HAVE_LIBHIREDIS
    connectRedis(name);
#endif
}

void DB::close(){
    PQfinish(db);
#ifdef HAVE_LIBHIREDIS
    redisFree(redis_ctx);
#endif
}

void DB::healthCheck(){
    if(PQstatus(db) != CONNECTION_OK){
        PQreset(db);
        db = PQconnectdb("");
    }
#ifdef HAVE_LIBHIREDIS
    if (redis_ctx->err) {
        redisFree(redis_ctx);
        connectRedis(name);
    }
#endif
}

Result toResult(PGresult *res){
    Result r(PQntuples(res), Row(PQnfields(res)));
    for(int i=0; i<PQntuples(res); i++){
        for(int j=0; j<PQnfields(res); j++)
            r[i][j] = PQgetvalue(res, i, j);
    }
    PQclear(res);
    return r;
}

Result queryH(const std::string &q, int n, ...){
    const char* val[n];
    va_list vl;
    va_start(vl, n);
    for(int i=0; i<n; i++)
        val[i] = va_arg(vl, const char*);
    va_end(vl);
    PGresult* res = PQexecParams(db, q.c_str(), n, 0, val, 0, 0, 0);
    return toResult(res);
}

Result DB::query(const std::string &q, const std::vector<std::string> &params){
    const char* val[params.size()];
    for(unsigned i=0; i<params.size(); i++)
        val[i] = params[i].c_str();
    PGresult* res = PQexecParams(db, q.c_str(), params.size(), 0, val, 0, 0, 0);
    return toResult(res);
}

Result DB::query(const std::string &q){
    PGresult* res = PQexec(db, q.c_str());
    return toResult(res);
}

Result DB::query(const std::string &q, const std::string &p1){ return queryH(q, 1, p1.c_str()); }
Result DB::query(const std::string &q, const std::string &p1, const std::string &p2){ return queryH(q, 2, p1.c_str(), p2.c_str()); }
Result DB::query(const std::string &q, const std::string &p1, const std::string &p2, const std::string &p3){ return queryH(q, 3, p1.c_str(), p2.c_str(), p3.c_str()); }
Result DB::query(const std::string &q, const std::string &p1, const std::string &p2, const std::string &p3, const std::string &p4){ return queryH(q, 4, p1.c_str(), p2.c_str(), p3.c_str(), p4.c_str()); }
Result DB::query(const std::string &q, const std::string &p1, const std::string &p2, const std::string &p3, const std::string &p4, const std::string &p5){ return queryH(q, 5, p1.c_str(), p2.c_str(), p3.c_str(), p4.c_str(), p5.c_str()); }
