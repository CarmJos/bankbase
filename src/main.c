#include <stdio.h>
#include <stdlib.h>
#include "database.h"
#include "user.h"
#include "admin.h"

#define DB_NAME "bank_system.db"

// 菜单选项
void show_main_menu();

void show_admin_menu();

void show_user_menu();

// 主函数
int main() {
    if (init_database(DB_NAME) != SQLITE_OK) {
        return EXIT_FAILURE;
    }

    int choice;
    while (1) {
        show_main_menu();
        printf("请选择: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (admin_login()) {
                    int admin_choice;
                    do {
                        show_admin_menu();
                        printf("请选择: ");
                        scanf("%d", &admin_choice);
                        switch (admin_choice) {
                            case 1:
                                add_customer();
                                break;
                            case 2:
                                update_customer();
                                break;
                            case 3:
                                delete_customer();
                                break;
                            case 4:
                                query_customer();
                                break;
                            case 5:
                                add_account();
                                break;
                            case 6:
                                update_account();
                                break;
                            case 7:
                                suspend_account();
                                break;
                            case 8:
                                delete_account();
                                break;
                            case 9:
                                query_account();
                                break;
                            case 10:
                                query_transaction();
                                break;
                            case 0:
                                break;
                            default:
                                printf("无效选择，请重新选择。\n");
                                break;
                        }
                    } while (admin_choice != 0);
                } else {
                    printf("管理员登录失败。\n");
                }
                break;
            case 2:
                if (user_login()) {
                    int user_choice;
                    do {
                        show_user_menu();
                        printf("请选择: ");
                        scanf("%d", &user_choice);
                        switch (user_choice) {
                            case 1:
                                change_password();
                                break;
                            case 2:
                                deposit_money();
                                break;
                            case 3:
                                withdraw_money();
                                break;
                            case 4:
                                transfer_money();
                                break;
                            case 5:
                                check_balance();
                                break;
                            case 6:
                                query_transaction();
                                break;
                            case 0:
                                break;
                            default:
                                printf("无效选择，请重新选择。\n");
                                break;
                        }
                    } while (user_choice != 0);
                } else {
                    printf("用户登录失败。\n");
                }
                break;
            case 0:
                close_database();
                printf("退出系统。\n");
                return EXIT_SUCCESS;
            default:
                printf("无效选择，请重新选择。\n");
                break;
        }
    }
    return 0;
}

void show_main_menu() {
    printf("\n=== 银行存取款管理系统 ===\n");
    printf("1. 管理员登录\n");
    printf("2. 用户登录\n");
    printf("0. 退出\n");
}

void show_admin_menu() {
    printf("\n=== 管理员菜单 ===\n");
    printf("1. 添加客户\n");
    printf("2. 修改客户信息\n");
    printf("3. 删除客户\n");
    printf("4. 查询客户信息\n");
    printf("5. 添加银行卡账户\n");
    printf("6. 修改银行卡账户信息\n");
    printf("7. 挂失银行卡账户\n");
    printf("8. 注销银行卡账户\n");
    printf("9. 查询银行卡账户信息\n");
    printf("10. 查询交易信息\n");
    printf("0. 返回主菜单\n");
}

void show_user_menu() {
    printf("\n=== 用户菜单 ===\n");
    printf("1. 修改密码\n");
    printf("2. 存款\n");
    printf("3. 取款\n");
    printf("4. 转账\n");
    printf("5. 查询余额\n");
    printf("6. 查询交易记录\n");
    printf("0. 返回主菜单\n");
}
