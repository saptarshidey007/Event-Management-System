#ifndef PAYMENTS_RECORDS_H
#define PAYMENTS_RECORDS_H

typedef struct{
    int payid;
    int pid;
    int eventID;
    float amount;
    char mode[15];
    char status[10];
}Payment;

void make_payment();
void list_payments();
void refund_payment();
int find_payment(int pid, Payment *out);
void total_collected();
void event_summary();
void pending_payments();
void receipt();
void payments_records_menu();

#endif