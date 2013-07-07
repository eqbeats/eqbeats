#include "db.h"
#include "path.h"
#include <stdarg.h>
#include <libpq-fe.h>
#include <hiredis/hiredis.h>

using namespace DB;

PGconn *db;
redisContext *redis_ctx;

void DB::connect(){
    db = PQconnectdb("");
    redis_ctx = redisConnectUnix((eqbeatsDir() + "/redis.sock").c_str());
}

void DB::setName(std::string name){
        redisCommand(redis_ctx, "CLIENT SETNAME %s", name.c_str());
}

void DB::close(){
    PQfinish(db);
    redisFree(redis_ctx);
}

struct redisContext* DB::redis() {
    return redis_ctx;
}

void DB::healthCheck(){
    if(PQstatus(db) != CONNECTION_OK || redis_ctx->err){
        PQreset(db);
        redisFree(redis_ctx);
        DB::connect();
    }
}

void DB::blindRedisCommand(const std::string &q, ...){
    va_list ap;
    va_start(ap, q);
    void* r = redisvCommand(redis_ctx, q.c_str(), ap);
    if(r != NULL) freeReplyObject(r);
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
