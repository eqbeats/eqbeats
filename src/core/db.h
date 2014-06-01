#ifndef DB_H
#define DB_H

#ifdef HAVE_CONFIG_H
#  include "../../config.h"
#endif

#include <string>
#include <vector>

struct redisContext;
struct pg_conn;

namespace DB{

bool connect(std::string name="");
void close();

void setPgDatabase(struct pg_conn *pg);

void healthCheck();

#ifdef HAVE_LIBHIREDIS
struct redisContext* redis();
void blindRedisCommand(const std::string &q, ...);
#endif

typedef std::vector<std::string> Row;
typedef std::vector<Row> Result;

Result query(const std::string &q);
Result query(const std::string &q, const std::vector<std::string> &params);
Result query(const std::string &q, const std::string &p1);
Result query(const std::string &q, const std::string &p1, const std::string &p2);
Result query(const std::string &q, const std::string &p1, const std::string &p2, const std::string &p3);
Result query(const std::string &q, const std::string &p1, const std::string &p2, const std::string &p3, const std::string &p4);
Result query(const std::string &q, const std::string &p1, const std::string &p2, const std::string &p3, const std::string &p4, const std::string &p5);

}

#endif // DB_H
