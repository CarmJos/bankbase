#include "database.h"
#include <stdio.h>
#include <stdlib.h>

static sqlite3 *db = NULL;
static char *err_msg = NULL;

// 初始化数据库
int init_database(const char *db_name) {
    int rc = sqlite3_open(db_name, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    return SQLITE_OK;
}

// 关闭数据库
void close_database() {
    if (db) {
        sqlite3_close(db);
    }
}

// 执行SQL语句
int execute_sql(const char *sql) {
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    return rc;
}

// 带回调函数的执行SQL语句
int execute_sql_with_callback(const char *sql, int (*callback)(void *, int, char **, char **), void *data) {
    int rc = sqlite3_exec(db, sql, callback, data, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    return rc;
}

// 获取数据库错误信息
const char *get_db_error() {
    return err_msg ? err_msg : "No error";
}
