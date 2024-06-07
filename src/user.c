#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "database.h"
#include "utils.h"

// 当前登录的银行卡号
static char current_card_number[20];
static char stored_hash[100];

int pw_callback(void *data, int argc, char **argv, char **azColName) {
    if (argc > 0 && argv[0]) {
        strcpy(stored_hash, argv[0]);
    }
    return 0;
}


// 用户登录
int user_login() {
    char card_number[20];
    char password[7];  // 银行卡密码为六位数字

    printf("请输入银行卡号: ");
    scanf("%s", card_number);
    printf("请输入密码: ");
    scanf("%s", password);

    char sql[256];
    snprintf(sql, sizeof(sql), "SELECT PasswordHash FROM Account WHERE CardNumber='%s' AND Status=0", card_number);

    int rc = execute_sql_with_callback(sql, pw_callback, NULL);
    if (rc == SQLITE_OK && bcrypt_verify(password, stored_hash) == 0) {
        strcpy(current_card_number, card_number);
        return 1;
    }
    return 0;
}

// 修改密码
void change_password() {
    char new_password[7];  // 银行卡密码为六位数字
    char hashed_password[100];

    printf("请输入新密码: ");
    scanf("%6s", new_password);

    // 使用BCrypt加密新密码
    bcrypt_hash(new_password, hashed_password);

    char sql[256];
    snprintf(sql, sizeof(sql), "UPDATE Account SET PasswordHash='%s' WHERE CardNumber='%s'", hashed_password,
             current_card_number);

    if (execute_sql(sql) == SQLITE_OK) {
        printf("密码修改成功。\n");
    } else {
        printf("密码修改失败。\n");
    }

}


// 存取款
void deposit_withdraw(int type) {
    double amount;
    printf("请输入金额: ");
    scanf("%lf", &amount);

    if (type == 0) {
        // 存款
        char sql[256];
        snprintf(sql, sizeof(sql), "UPDATE Account SET Balance = Balance + %lf WHERE CardNumber='%s'", amount,
                 current_card_number);

        if (execute_sql(sql) == SQLITE_OK) {
            printf("存款成功。\n");
        } else {
            printf("存款失败。\n");
        }
    } else {
        // 取款
        char sql[256];
        snprintf(sql, sizeof(sql),
                 "UPDATE Account SET Balance = Balance - %lf WHERE CardNumber='%s' AND Balance >= %lf", amount,
                 current_card_number, amount);

        if (execute_sql(sql) == SQLITE_OK) {
            printf("取款成功。\n");
        } else {
            printf("余额不足或取款失败。\n");
        }
    }

    // 插入存取款记录
    char location[50];
    printf("请输入存取款地点: ");
    scanf("%s", location);

    char sql_log[256];
    snprintf(sql_log, sizeof(sql_log),
             "INSERT INTO DepositWithdraw (CardNumber, Time, Amount, LocationCode) VALUES ('%s', datetime('now', 'localtime'), %lf, '%s')",
             current_card_number, (type == 0 ? amount : -amount), location);

    execute_sql(sql_log);
}

// 转账
void transfer() {
    char to_card_number[20];
    double amount;

    printf("请输入转入的银行卡号: ");
    scanf("%s", to_card_number);
    printf("请输入转账金额: ");
    scanf("%lf", &amount);

    // 转出
    char sql_out[256];
    snprintf(sql_out, sizeof(sql_out),
             "UPDATE Account SET Balance = Balance - %lf WHERE CardNumber='%s' AND Balance >= %lf", amount,
             current_card_number, amount);

    if (execute_sql(sql_out) != SQLITE_OK) {
        printf("余额不足或转账失败。\n");
        return;
    }

    // 转入
    char sql_in[256];
    snprintf(sql_in, sizeof(sql_in), "UPDATE Account SET Balance = Balance + %lf WHERE CardNumber='%s'", amount,
             to_card_number);

    if (execute_sql(sql_in) != SQLITE_OK) {
        // 转入失败，回滚转出
        snprintf(sql_out, sizeof(sql_out), "UPDATE Account SET Balance = Balance + %lf WHERE CardNumber='%s'",
                 amount, current_card_number);
        execute_sql(sql_out);
        printf("转账失败。\n");
        return;
    }

    // 插入转账记录
    char sql_log[256];
    snprintf(sql_log, sizeof(sql_log),
             "INSERT INTO TransferRecord (CardNumber, ToCardNumber, Time, Amount) VALUES ('%s', '%s', datetime('now', 'localtime'), %lf)",
             current_card_number, to_card_number, amount);

    if (execute_sql(sql_log) == SQLITE_OK) {
        printf("转账成功。\n");
    } else {
        printf("转账记录插入失败。\n");
    }
}


int query_balance_callback(void *data, int argc, char **argv, char **azColName) {
    printf("当前余额: %s\n", argv[0] ? argv[0] : "0.00");
    return 0;
}

// 查询余额
void query_balance() {
    char sql[256];
    snprintf(sql, sizeof(sql), "SELECT Balance FROM Account WHERE CardNumber='%s'", current_card_number);

    execute_sql_with_callback(sql, query_balance_callback, NULL);
}


int query_transaction_callback(void *data, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s: %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

// 查询交易记录
void query_transaction() {
    char sql[256];
    snprintf(sql, sizeof(sql), "SELECT * FROM DepositWithdraw WHERE CardNumber='%s'", current_card_number);


    execute_sql_with_callback(sql, query_transaction_callback, NULL);
}

