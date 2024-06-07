#include <stdio.h>
#include "transaction.h"
#include "database.h"

// 存款
void deposit(const char *card_number, double amount) {
    char sql[256];
    snprintf(sql, sizeof(sql), "UPDATE Account SET Balance = Balance + %lf WHERE CardNumber='%s'", amount, card_number);

    if (execute_sql(sql) == SQLITE_OK) {
        printf("存款成功。\n");

        char location[50];
        printf("请输入存款地点: ");
        scanf("%s", location);

        char sql_log[256];
        snprintf(sql_log, sizeof(sql_log),
                 "INSERT INTO DepositWithdraw (CardNumber, Time, Amount, LocationCode) VALUES ('%s', datetime('now', 'localtime'), %lf, '%s')",
                 card_number, amount, location);
        execute_sql(sql_log);
    } else {
        printf("存款失败。\n");
    }
}

// 取款
void withdraw(const char *card_number, double amount) {
    char sql[256];
    snprintf(sql, sizeof(sql), "UPDATE Account SET Balance = Balance - %lf WHERE CardNumber='%s' AND Balance >= %lf",
             amount, card_number, amount);

    if (execute_sql(sql) == SQLITE_OK) {
        printf("取款成功。\n");

        char location[50];
        printf("请输入取款地点: ");
        scanf("%s", location);

        char sql_log[256];
        snprintf(sql_log, sizeof(sql_log),
                 "INSERT INTO DepositWithdraw (CardNumber, Time, Amount, LocationCode) VALUES ('%s', datetime('now', 'localtime'), %lf, '%s')",
                 card_number, -amount, location);
        execute_sql(sql_log);
    } else {
        printf("余额不足或取款失败。\n");
    }
}

// 转账
void transfer(const char *from_card_number, const char *to_card_number, double amount) {
    char sql_out[256];
    snprintf(sql_out, sizeof(sql_out),
             "UPDATE Account SET Balance = Balance - %lf WHERE CardNumber='%s' AND Balance >= %lf", amount,
             from_card_number, amount);

    if (execute_sql(sql_out) != SQLITE_OK) {
        printf("余额不足或转账失败。\n");
        return;
    }

    char sql_in[256];
    snprintf(sql_in, sizeof(sql_in), "UPDATE Account SET Balance = Balance + %lf WHERE CardNumber='%s'", amount,
             to_card_number);

    if (execute_sql(sql_in) != SQLITE_OK) {
        // 如果转入失败，回滚转出操作
        snprintf(sql_out, sizeof(sql_out), "UPDATE Account SET Balance = Balance + %lf WHERE CardNumber='%s'", amount,
                 from_card_number);
        execute_sql(sql_out);
        printf("转账失败。\n");
        return;
    }

    printf("转账成功。\n");

    char sql_log[256];
    snprintf(sql_log, sizeof(sql_log),
             "INSERT INTO Transfer (CardNumber, ToCardNumber, Time, Amount) VALUES ('%s', '%s', datetime('now', 'localtime'), %lf)",
             from_card_number, to_card_number, amount);
    execute_sql(sql_log);
}


int query_transaction_callback(void *data, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s: %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    return 0;
}

// 查询交易记录
void query_transaction(const char *card_number) {
    char sql[256];
    snprintf(sql, sizeof(sql),
             "SELECT * FROM DepositWithdraw WHERE CardNumber='%s' UNION SELECT * FROM Transfer WHERE CardNumber='%s' OR ToCardNumber='%s' ORDER BY Time DESC",
             card_number, card_number, card_number);
    execute_sql_with_callback(sql, query_transaction_callback, NULL);
}
