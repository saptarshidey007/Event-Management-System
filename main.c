#include <stdio.h>
#include "eventmanagement.h"
#include "participantregistration.h"
#include "payments-records.h"

int main(){
    int choice; 

    do{

        printf("\n========== EVENT MANAGEMENT SYSTEM ==========\n");
        printf("\n========== MAIN MENU ==========\n");
        printf("1. Event Management\n");
        printf("2. Participant Registration\n");
        printf("3. Payments/Records\n");
        printf("4. Exit...\n");
        printf("Enter choice: ");
        scanf("%d",&choice);

        switch(choice){
            case 1:
            event_menu();
            break;

            case 2:
            participant_menu();
            break;

            case 3:
            payments_records_menu();
            break;

            case 4:
            printf("Exiting...\n");
            break;

            default:
            printf("Invalid choice!!\n");
        }
    }while(choice!=4);
    return 0;
}