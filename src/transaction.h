#ifndef BANKBASE_TRANSACTION_H
#define BANKBASE_TRANSACTION_H

void deposit(const char *card_number, double amount);

void withdraw(const char *card_number, double amount);

void transfer(const char *from_card_number, const char *to_card_number, double amount);

void query_transaction(const char *card_number);

#endif //BANKBASE_TRANSACTION_H
