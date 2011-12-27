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
    const char *val[n];
    va_list vl;
    va_start(vl, n);
    for(int i=0; i<n; i++)
        val[i] = va_arg(vl, const char*);
    va_end(vl);
    return toResult(PQexecParams(db, q.c_str(), n, 0, val, 0, 0, 0));
}

Result DB::query(const std::string &q){
    return toResult(PQexec(db, q.c_str()));
}

Result DB::query(const std::string &q, const std::string &p1){ return queryH(q, 1, p1.c_str()); }
Result DB::query(const std::string &q, const std::string &p1, const std::string &p2){ return queryH(q, 2, p1.c_str(), p2.c_str()); }
Result DB::query(const std::string &q, const std::string &p1, const std::string &p2, const std::string &p3){ return queryH(q, 3, p1.c_str(), p2.c_str(), p3.c_str()); }
Result DB::query(const std::string &q, const std::string &p1, const std::string &p2, const std::string &p3, const std::string &p4){ return queryH(q, 4, p1.c_str(), p2.c_str(), p3.c_str(), p4.c_str()); }
Result DB::query(const std::string &q, const std::string &p1, const std::string &p2, const std::string &p3, const std::string &p4, const std::string &p5){ return queryH(q, 5, p1.c_str(), p2.c_str(), p3.c_str(), p4.c_str(), p5.c_str()); }
