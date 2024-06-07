#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bcrypt.h>
#include "account.h"
#include "database.h"

// 创建新账户
void create_account() {
    char id_number[20];
    char card_number[20];
    char bank_code[10];
    char bank_name[50];
    char initial_password[7];
    char security_question[50];
    char security_answer[50];
    double initial_balance;
    char hashed_password[100];

    printf("请输入身份证号: ");
    scanf("%s", id_number);
    printf("请输入银行卡号: ");
    scanf("%s", card_number);
    printf("请输入银行编号: ");
    scanf("%s", bank_code);
    printf("请输入银行名称: ");
    scanf("%s", bank_name);
    printf("请输入初始密码（六位数字）: ");
    scanf("%6s", initial_password);
    printf("请输入密保问题: ");
    scanf("%s", security_question);
    printf("请输入密保答案: ");
    scanf("%s", security_answer);
    printf("请输入初始余额: ");
    scanf("%lf", &initial_balance);

    // 使用BCrypt加密初始密码
    if (BCryptEncrypt(initial_password, hashed_password) != 0) {
        printf("密码加密失败。\n");
        return;
    }

    char sql[512];
    snprintf(sql, sizeof(sql),
             "INSERT INTO Account (IDNumber, CardNumber, BankCode, BankName, PasswordHash, SecurityQuestion, SecurityAnswer, Balance, Status) "
             "VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', %lf, 0)",
             id_number, card_number, bank_code, bank_name, hashed_password, security_question, security_answer,
             initial_balance);

    if (execute_sql(sql) == SQLITE_OK) {
        printf("账户创建成功。\n");
    } else {
        printf("账户创建失败。\n");
    }
}

int pw_callback(void *data, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s: %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    return 0;
}


// 更新账户信息
void update_account() {
    char card_number[20];
    printf("请输入要更新的银行卡号: ");
    scanf("%s", card_number);

    char sql[256];
    snprintf(sql, sizeof(sql), "SELECT * FROM Account WHERE CardNumber='%s'", card_number);

    execute_sql_with_callback(sql, pw_callback, NULL);

    printf("请选择要更新的项目:\n");
    printf("1. 银行名称\n");
    printf("2. 密保问题\n");
    printf("3. 密保答案\n");
    printf("4. 当前余额\n");
    printf("5. 当前状态 (0=正常, 1=挂失, 2=注销)\n");
    printf("0. 取消\n");

    int choice;
    scanf("%d", &choice);

    switch (choice) {
        case 1: {
            char bank_name[50];
            printf("请输入新银行名称: ");
            scanf("%s", bank_name);
            snprintf(sql, sizeof(sql), "UPDATE Account SET BankName='%s' WHERE CardNumber='%s'", bank_name,
                     card_number);
            break;
        }
        case 2: {
            char security_question[50];
            printf("请输入新密保问题: ");
            scanf("%s", security_question);
            snprintf(sql, sizeof(sql), "UPDATE Account SET SecurityQuestion='%s' WHERE CardNumber='%s'",
                     security_question, card_number);
            break;
        }
        case 3: {
            char security_answer[50];
            printf("请输入新密保答案: ");
            scanf("%s", security_answer);
            snprintf(sql, sizeof(sql), "UPDATE Account SET SecurityAnswer='%s' WHERE CardNumber='%s'", security_answer,
                     card_number);
            break;
        }
        case 4: {
            double balance;
            printf("请输入新余额: ");
            scanf("%lf", &balance);
            snprintf(sql, sizeof(sql), "UPDATE Account SET Balance=%lf WHERE CardNumber='%s'", balance, card_number);
            break;
        }
        case 5: {
            int status;
            printf("请输入新状态 (0=正常, 1=挂失, 2=注销): ");
            scanf("%d", &status);
            snprintf(sql, sizeof(sql), "UPDATE Account SET Status=%d WHERE CardNumber='%s'", status, card_number);
            break;
        }
        case 0:
            return;
        default:
            printf("无效选择。\n");
            return;
    }

    if (execute_sql(sql) == SQLITE_OK) {
        printf("账户信息更新成功。\n");
    } else {
        printf("账户信息更新失败。\n");
    }
}

// 挂失账户
void suspend_account() {
    char card_number[20];
    printf("请输入要挂失的银行卡号: ");
    scanf("%s", card_number);

    char sql[256];
    snprintf(sql, sizeof(sql), "UPDATE Account SET Status=1 WHERE CardNumber='%s'", card_number);

    if (execute_sql(sql) == SQLITE_OK) {
        printf("账户挂失成功。\n");
    } else {
        printf("账户挂失失败。\n");
    }
}

// 注销账户
void delete_account() {
    char card_number[20];
    printf("请输入要注销的银行卡号: ");
    scanf("%s", card_number);

    char sql[256];
    snprintf(sql, sizeof(sql), "UPDATE Account SET Status=2 WHERE CardNumber='%s'", card_number);

    if (execute_sql(sql) == SQLITE_OK) {
        printf("账户注销成功。\n");
    } else {
        printf("账户注销失败。\n");
    }
}

// 查询账户信息
void query_account() {
    char card_number[20];
    printf("请输入要查询的银行卡号: ");
    scanf("%s", card_number);

    char sql[256];
    snprintf(sql, sizeof(sql), "SELECT * FROM Account WHERE CardNumber='%s'", card_number);

    int callback(void *data, int argc, char **argv, char **azColName) {
        for (int i = 0; i < argc; i++) {
            printf("%s: %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        }
        return 0;
    }

    execute_sql_with_callback(sql, pw_callback, NULL);
}
