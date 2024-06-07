#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "admin.h"
#include "database.h"

// 登录管理员信息
static char current_admin_username[20];
static char stored_hash[100];

int pw_callback(void *data, int argc, char **argv, char **azColName) {
    if (argc > 0 && argv[0]) {
        strcpy(stored_hash, argv[0]);
    }
    return 0;
}

// 管理员登录
int admin_login() {
    char username[20];
    char password[20];

    printf("请输入用户名: ");
    scanf("%s", username);
    printf("请输入密码: ");
    scanf("%s", password);

    char sql[256];
    snprintf(sql, sizeof(sql), "SELECT PasswordHash FROM Admin WHERE Username='%s'", username);

    int rc = execute_sql_with_callback(sql, pw_callback, NULL);
    if (rc == SQLITE_OK && bcrypt_verify(password, stored_hash) == 0) {
        strcpy(current_admin_username, username);
        return 1;
    }
    return 0;
}

// 添加客户
void add_customer() {
    char id_card[19], name[20], gender[2], address[100], phone[12];

    printf("请输入身份证号: ");
    scanf("%s", id_card);
    printf("请输入姓名: ");
    scanf("%s", name);
    printf("请输入性别 (M/F): ");
    scanf("%s", gender);
    printf("请输入住址: ");
    scanf("%s", address);
    printf("请输入电话: ");
    scanf("%s", phone);

    char sql[256];
    snprintf(sql, sizeof(sql),
             "INSERT INTO Customer (IDCard, Name, Gender, Address, Phone) VALUES ('%s', '%s', '%s', '%s', '%s')",
             id_card, name, gender, address, phone);

    if (execute_sql(sql) == SQLITE_OK) {
        printf("客户添加成功。\n");
    } else {
        printf("客户添加失败。\n");
    }
}

// 显示客户当前信息
int query_customer_callback(void *data, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s: %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    return 0;
}

// 更新客户信息
void update_customer() {
    char id_card[19];
    printf("请输入要修改的客户身份证号: ");
    scanf("%s", id_card);

    char sql[256];
    snprintf(sql, sizeof(sql), "SELECT * FROM Customer WHERE IDCard='%s'", id_card);


    execute_sql_with_callback(sql, query_customer_callback, NULL);

    // 选择并修改条目
    printf("请选择要修改的条目:\n");
    printf("1. 姓名\n");
    printf("2. 性别\n");
    printf("3. 住址\n");
    printf("4. 电话\n");
    printf("0. 取消\n");
    int choice;
    printf("请输入选择: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: {
            char name[20];
            printf("请输入新姓名: ");
            scanf("%s", name);
            snprintf(sql, sizeof(sql), "UPDATE Customer SET Name='%s' WHERE IDCard='%s'", name, id_card);
            break;
        }
        case 2: {
            char gender[2];
            printf("请输入新性别 (M/F): ");
            scanf("%s", gender);
            snprintf(sql, sizeof(sql), "UPDATE Customer SET Gender='%s' WHERE IDCard='%s'", gender, id_card);
            break;
        }
        case 3: {
            char address[100];
            printf("请输入新住址: ");
            scanf("%s", address);
            snprintf(sql, sizeof(sql), "UPDATE Customer SET Address='%s' WHERE IDCard='%s'", address, id_card);
            break;
        }
        case 4: {
            char phone[12];
            printf("请输入新电话: ");
            scanf("%s", phone);
            snprintf(sql, sizeof(sql), "UPDATE Customer SET Phone='%s' WHERE IDCard='%s'", phone, id_card);
            break;
        }
        case 0:
            return;
        default:
            printf("无效选择。\n");
            return;
    }

    if (execute_sql(sql) == SQLITE_OK) {
        printf("客户信息更新成功。\n");
    } else {
        printf("客户信息更新失败。\n");
    }
}

// 删除客户
void delete_customer() {
    char id_card[19];
    printf("请输入要删除的客户身份证号: ");
    scanf("%s", id_card);

    char sql[256];
    snprintf(sql, sizeof(sql), "DELETE FROM Customer WHERE IDCard='%s'", id_card);

    if (execute_sql(sql) == SQLITE_OK) {
        printf("客户删除成功。\n");
    } else {
        printf("客户删除失败。\n");
    }
}

// 查询客户信息
void query_customer() {
    char id_card[19];
    printf("请输入要查询的客户身份证号: ");
    scanf("%s", id_card);

    char sql[256];
    snprintf(sql, sizeof(sql), "SELECT * FROM Customer WHERE IDCard='%s'", id_card);

    execute_sql_with_callback(sql, query_customer_callback, NULL);
}

// 添加银行卡账户
void add_account() {
    char id_card[19], card_number[20], bank_code[10], bank_name[50], password[20], security_question[50], security_answer[50];
    double initial_balance;

    printf("请输入客户身份证号: ");
    scanf("%s", id_card);
    printf("请输入银行卡号: ");
    scanf("%s", card_number);
    printf("请输入银行编号: ");
    scanf("%s", bank_code);
    printf("请输入银行名称: ");
    scanf("%s", bank_name);
    printf("请输入初始密码: ");
    scanf("%s", password);
    printf("请输入密保问题: ");
    scanf("%s", security_question);
    printf("请输入密保答案: ");
    scanf("%s", security_answer);
    printf("请输入初始余额: ");
    scanf("%lf", &initial_balance);

    char sql[256];
    snprintf(sql, sizeof(sql),
             "INSERT INTO Account (IDCard, CardNumber, BankCode, BankName, InitialPassword, SecurityQuestion, SecurityAnswer, Balance, Status) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', %lf, 0)",
             id_card, card_number, bank_code, bank_name, password, security_question, security_answer, initial_balance);

    if (execute_sql(sql) == SQLITE_OK) {
        printf("银行卡账户添加成功。\n");
    } else {
        printf("银行卡账户添加失败。\n");
    }
}

// 更新银行卡账户信息
void update_account() {
    char card_number[20];
    printf("请输入要修改的银行卡号: ");
    scanf("%s", card_number);

    char sql[256];
    snprintf(sql, sizeof(sql), "SELECT * FROM Account WHERE CardNumber='%s'", card_number);
    execute_sql_with_callback(sql, query_customer_callback, NULL);

    // 选择并修改条目
    printf("请选择要修改的条目:\n");
    printf("1. 初始密码\n");
    printf("2. 密保问题\n");
    printf("3. 密保答案\n");
    printf("4. 余额\n");
    printf("5. 状态\n");
    printf("0. 取消\n");
    int choice;
    printf("请输入选择: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: {
            char password[20];
            printf("请输入新初始密码: ");
            scanf("%s", password);
            snprintf(sql, sizeof(sql), "UPDATE Account SET InitialPassword='%s' WHERE CardNumber='%s'", password,
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

// 挂失银行卡账户
void suspend_account() {
    char card_number[20];
    printf("请输入要挂失的银行卡号: ");
    scanf("%s", card_number);

    char sql[256];
    snprintf(sql, sizeof(sql), "UPDATE Account SET Status=1 WHERE CardNumber='%s'", card_number);

    if (execute_sql(sql) == SQLITE_OK) {
        printf("银行卡账户挂失成功。\n");
    } else {
        printf("银行卡账户挂失失败。\n");
    }
}

// 注销银行卡账户
void delete_account() {
    char card_number[20];
    printf("请输入要注销的银行卡号: ");
    scanf("%s", card_number);

    char sql[256];
    snprintf(sql, sizeof(sql), "UPDATE Account SET Status=2 WHERE CardNumber='%s'", card_number);

    if (execute_sql(sql) == SQLITE_OK) {
        printf("银行卡账户注销成功。\n");
    } else {
        printf("银行卡账户注销失败。\n");
    }
}

// 查询银行卡账户信息
void query_account() {
    char card_number[20];
    printf("请输入要查询的银行卡号: ");
    scanf("%s", card_number);

    char sql[256];
    snprintf(sql, sizeof(sql), "SELECT * FROM Account WHERE CardNumber='%s'", card_number);

    execute_sql_with_callback(sql, query_customer_callback, NULL);
}

// 查询交易记录
void query_transaction() {
    char card_number[20];
    printf("请输入要查询的银行卡号: ");
    scanf("%s", card_number);

    char sql[256];
    snprintf(sql, sizeof(sql), "SELECT * FROM DepositWithdraw WHERE CardNumber='%s'", card_number);

    execute_sql_with_callback(sql, query_customer_callback, NULL);
}