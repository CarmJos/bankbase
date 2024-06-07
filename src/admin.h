//
// Created by Karmu on 24-6-7.
//

#ifndef BANKBASE_ADMIN_H
#define BANKBASE_ADMIN_H

// 管理员登录
int admin_login();

// 管理员功能
void add_customer();

void update_customer();

void delete_customer();

void query_customer();

void add_account();

void update_account();

void suspend_account();

void delete_account();

void query_account();

void query_transaction();

#endif //BANKBASE_ADMIN_H
