//Module 3: Payments/Records

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "eventmanagement.h"
#include "participantregistration.h"
#include "payments-records.h"

static int payment_id_exists(int payid)
{
    Payment pay;
    FILE *fp = fopen("payments.dat", "rb");

    if (fp == NULL)
        return 0;

    while (fread(&pay, sizeof(Payment), 1, fp) == 1)
    {
        if (pay.payid == payid)
        {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void make_payment()
{
    Payment pay, temp;
    Participant p;
    FILE *fp;

    printf("Enter Payment ID: ");
    scanf("%d", &pay.payid);

    if (payment_id_exists(pay.payid))
    {
        printf("Payment ID already exists!\n");
        return;
    }

    printf("Enter Participant ID: ");
    scanf("%d", &pay.pid);

    if (!find_participant(pay.pid, &p))
    {
        printf("Participant not found!\n");
        return;
    }

    if (find_payment(pay.pid, &temp))
    {
        printf("This participant has already paid!\n");
        return;
    }

    pay.eventID = p.eventID;

    printf("Enter Amount: ");
    scanf("%f", &pay.amount);

    printf("Enter Payment Mode (Cash/UPI/Card): ");
    scanf("%14s", pay.mode);
    if(pay.mode!="Cash" || pay.mode!="UPI" || pay.mode!="Card"){
        printf("Invalid mode of payment!\n");
        return;
    }
    strcpy(pay.status, "Paid");
        

    fp = fopen("payments.dat", "ab");

    if (fp == NULL)
    {
        printf("File error!\n");
        return;
    }

    fwrite(&pay, sizeof(Payment), 1, fp);
    fclose(fp);

    printf("Payment recorded successfully!\n");
}

void list_payments()
{
    Payment pay;
    FILE *fp = fopen("payments.dat", "rb");

    if (fp == NULL)
    {
        printf("No payments found!\n");
        return;
    }

    printf("\nPay ID\tPart. ID\tEvent ID\tAmount\tMode\tStatus\n");

    while (fread(&pay, sizeof(Payment), 1, fp) == 1)
    {
        printf("%d\t%d\t\t%d\t\t%.2f\t%s\t%s\n",
               pay.payid, pay.pid, pay.eventID,
               pay.amount, pay.mode, pay.status);
    }

    fclose(fp);
}

/* Finds the active ("Paid") payment of a participant. */
int find_payment(int pid, Payment *out)
{
    Payment pay;
    FILE *fp = fopen("payments.dat", "rb");

    if (fp == NULL)
        return 0;

    while (fread(&pay, sizeof(Payment), 1, fp) == 1)
    {
        if (pay.pid == pid && strcmp(pay.status, "Paid") == 0)
        {
            *out = pay;
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void refund_payment()
{
    Payment pay;
    int id;
    FILE *fp = fopen("payments.dat", "rb+");

    if (fp == NULL)
    {
        printf("No payments found!\n");
        return;
    }

    printf("Enter Payment ID to refund: ");
    scanf("%d", &id);

    while (fread(&pay, sizeof(Payment), 1, fp) == 1)
    {
        if (pay.payid == id)
        {
            if (strcmp(pay.status, "Refunded") == 0)
            {
                printf("Payment already refunded!\n");
                fclose(fp);
                return;
            }

            strcpy(pay.status, "Refunded");

            fseek(fp, -(long)sizeof(Payment), SEEK_CUR);
            fwrite(&pay, sizeof(Payment), 1, fp);

            printf("Payment refunded successfully!\n");
            fclose(fp);
            return;
        }
    }

    printf("Payment not found!\n");
    fclose(fp);
}

void total_collected()
{
    Payment pay;
    float total = 0;
    FILE *fp = fopen("payments.dat", "rb");

    if (fp == NULL)
    {
        printf("No payments found!\n");
        return;
    }

    while (fread(&pay, sizeof(Payment), 1, fp) == 1)
    {
        if (strcmp(pay.status, "Paid") == 0)
            total += pay.amount;
    }

    fclose(fp);

    printf("Total Collected: %.2f\n", total);
}

void event_summary()
{
    Event e;
    Payment pay;
    FILE *ef = fopen("events.dat", "rb");

    if (ef == NULL)
    {
        printf("No events found!\n");
        return;
    }

    printf("\nEvent ID\tName\tRegistered\tPaid\tRevenue\n");

    while (fread(&e, sizeof(Event), 1, ef) == 1)
    {
        int paid_count = 0;
        float revenue = 0;
        FILE *pf = fopen("payments.dat", "rb");

        if (pf != NULL)
        {
            while (fread(&pay, sizeof(Payment), 1, pf) == 1)
            {
                if (pay.eventID == e.id && strcmp(pay.status, "Paid") == 0)
                {
                    paid_count++;
                    revenue += pay.amount;
                }
            }
            fclose(pf);
        }

        printf("%d\t\t%s\t%d\t\t%d\t%.2f\n",
               e.id, e.name, count_for_event(e.id),
               paid_count, revenue);
    }

    fclose(ef);
}

void pending_payments()
{
    Participant p;
    Payment pay;
    int found = 0;
    FILE *fp = fopen("participants.dat", "rb");

    if (fp == NULL)
    {
        printf("No participants found!\n");
        return;
    }

    printf("\nParticipants with pending payment:\n");
    printf("ID\tName\tEvent ID\n");

    while (fread(&p, sizeof(Participant), 1, fp) == 1)
    {
        if (!find_payment(p.pid, &pay))
        {
            printf("%d\t%s\t%d\n", p.pid, p.cname, p.eventID);
            found = 1;
        }
    }

    if (!found)
        printf("None - everyone has paid!\n");

    fclose(fp);
}

void receipt()
{
    int id;
    Participant p;
    Payment pay;
    Event e;

    printf("Enter Participant ID: ");
    scanf("%d", &id);

    if (!find_participant(id, &p))
    {
        printf("Participant not found!\n");
        return;
    }

    if (!find_payment(id, &pay))
    {
        printf("No payment found for this participant!\n");
        return;
    }

    printf("\n----- RECEIPT -----\n");
    printf("Payment ID  : %d\n", pay.payid);
    printf("Participant : %s (ID %d)\n", p.cname, p.pid);

    if (find_event(p.eventID, &e))
        printf("Event       : %s (ID %d)\n", e.name, e.id);
    else
        printf("Event       : ID %d\n", p.eventID);

    printf("Amount      : %.2f\n", pay.amount);
    printf("Mode        : %s\n", pay.mode);
    printf("Status      : %s\n", pay.status);
    printf("-------------------\n");
}

void payments_records_menu()
{
    int choice;

    do
    {
        printf("\n--- PAYMENTS AND RECORDS ---\n");
        printf("1. Make Payment\n");
        printf("2. List Payments\n");
        printf("3. Refund Payment\n");
        printf("4. Find Payment\n");
        printf("5. Total Collected\n");
        printf("6. Event Summary\n");
        printf("7. Pending Payments\n");
        printf("8. Print Receipt\n");
        printf("9. Return to main menu\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                make_payment();
                break;

            case 2:
                list_payments();
                break;

            case 3:
                refund_payment();
                break;

            case 4:
            {
                int id;
                Payment pay;

                printf("Enter Participant ID: ");
                scanf("%d", &id);

                if (find_payment(id, &pay))
                    printf("Payment ID: %d, Amount: %.2f, Mode: %s, Status: %s\n",
                           pay.payid, pay.amount, pay.mode, pay.status);
                else
                    printf("No payment found!\n");

                break;
            }

            case 5:
                total_collected();
                break;

            case 6:
                event_summary();
                break;

            case 7:
                pending_payments();
                break;

            case 8:
                receipt();
                break;

            case 9:
                printf("Returning to main menu...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 9);
}