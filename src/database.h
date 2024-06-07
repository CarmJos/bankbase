#ifndef BANKBASE_DATABASE_H
#define BANKBASE_DATABASE_H

#include <sqlite3.h>

// 数据库初始化和关闭
int init_database(const char *db_name);

void close_database();

// 执行SQL语句
int execute_sql(const char *sql);

int execute_sql_with_callback(const char *sql, int (*callback)(void *, int, char **, char **), void *data);

// 错误处理
const char *get_db_error();


#endif //BANKBASE_DATABASE_H
