#include "db.h"
#include <stdarg.h>
#include <libpq-fe.h>

using namespace DB;

PGconn *db;

void DB::connect(){
    db = PQconnectdb("");
}

void DB::close(){
    PQfinish(db);
}

int healthCheck(){
    if(PQstatus(db) != CONNECTION_OK){
        PQreset(db);
        return 0;
    }
    return 1;
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
    if(PQstatus(db) != CONNECTION_OK){
        PQreset(db);
        res = PQexecParams(db, q.c_str(), n, 0, val, 0, 0, 0);
    }
    return toResult(res);
}

Result DB::query(const std::string &q, const std::vector<std::string> &params){
    const char* val[params.size()];
    for(unsigned i=0; i<params.size(); i++)
        val[i] = params[i].c_str();
    PGresult* res = PQexecParams(db, q.c_str(), params.size(), 0, val, 0, 0, 0);
    if(PQstatus(db) != CONNECTION_OK){
        PQreset(db);
        res = PQexecParams(db, q.c_str(), params.size(), 0, val, 0, 0, 0);
    }
    return toResult(res);
}

Result DB::query(const std::string &q){
    PGresult* res = PQexec(db, q.c_str());
    if(PQstatus(db) != CONNECTION_OK){
        PQreset(db);
        res = PQexec(db, q.c_str());
    }
    return toResult(res);
}

Result DB::query(const std::string &q, const std::string &p1){ return queryH(q, 1, p1.c_str()); }
Result DB::query(const std::string &q, const std::string &p1, const std::string &p2){ return queryH(q, 2, p1.c_str(), p2.c_str()); }
Result DB::query(const std::string &q, const std::string &p1, const std::string &p2, const std::string &p3){ return queryH(q, 3, p1.c_str(), p2.c_str(), p3.c_str()); }
Result DB::query(const std::string &q, const std::string &p1, const std::string &p2, const std::string &p3, const std::string &p4){ return queryH(q, 4, p1.c_str(), p2.c_str(), p3.c_str(), p4.c_str()); }
Result DB::query(const std::string &q, const std::string &p1, const std::string &p2, const std::string &p3, const std::string &p4, const std::string &p5){ return queryH(q, 5, p1.c_str(), p2.c_str(), p3.c_str(), p4.c_str(), p5.c_str()); }
