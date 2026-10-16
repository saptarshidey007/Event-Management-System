//Module 2: Participant Registration

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eventmanagement.h"
#include "participantregistration.h"


void register_participant()
{
    Participant p, temp;
    Event e;
    FILE *fp;

    printf("Enter Participant ID: ");
    scanf("%d", &p.pid);

    if (find_participant(p.pid, &temp))
    {
        printf("Participant ID already exists!\n");
        return;
    }

    printf("Enter Event ID: ");
    scanf("%d", &p.eventID);

    if (!find_event(p.eventID, &e))
    {
        printf("Event not found!\n");
        return;
    }

    if (count_for_event(p.eventID) >= e.capacity)
    {
        printf("Sorry, this event is full!\n");
        return;
    }

    printf("Enter Name: ");
    scanf(" %49[^\n]", p.cname);

    printf("Enter Age: ");
    scanf("%d", &p.age);

    printf("Enter Email: ");
    scanf("%49s", p.email);

    printf("Enter Phone: ");
    scanf("%10s", p.phone);

    printf("Enter College: ");
    scanf(" %49[^\n]", p.college);

    fp = fopen("participants.dat", "ab");

    if (fp == NULL)
    {
        printf("File error!\n");
        return;
    }

    fwrite(&p, sizeof(Participant), 1, fp);
    fclose(fp);

    printf("Participant registered successfully!\n");
}

void list_participants()
{
    Participant p;
    FILE *fp = fopen("participants.dat", "rb");

    if (fp == NULL)
    {
        printf("No participants found!\n");
        return;
    }

    printf("\nID\t\tName\t\tAge\t\tEmail\t\tPhone\t\tCollege\t\tEvent ID\n");

    while (fread(&p, sizeof(Participant), 1, fp) == 1)
    {
        printf("%d\t\t%s\t\t%d\t\t%s\t\t%s\t\t%s\t\t\t%d\n",
               p.pid, p.cname, p.age,
               p.email, p.phone, p.college, p.eventID);
    }

    fclose(fp);
}

int find_participant(int pid, Participant *out)
{
    Participant p;
    FILE *fp = fopen("participants.dat", "rb");

    if (fp == NULL)
        return 0;

    while (fread(&p, sizeof(Participant), 1, fp) == 1)
    {
        if (p.pid == pid)
        {
            *out = p;
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void update_participant()
{
    Participant p;
    int id;
    FILE *fp = fopen("participants.dat", "rb+");

    if (fp == NULL)
    {
        printf("No participants found!\n");
        return;
    }

    printf("Enter Participant ID to update: ");
    scanf("%d", &id);

    while (fread(&p, sizeof(Participant), 1, fp) == 1)
    {
        if (p.pid == id)
        {
            printf("Enter new Name: ");
            scanf(" %49[^\n]", p.cname);

            printf("Enter new Age: ");
            scanf("%d", &p.age);

            printf("Enter new Email: ");
            scanf("%49s", p.email);

            printf("Enter new Phone: ");
            scanf("%10s", p.phone);

            printf("Enter new College: ");
            scanf(" %49[^\n]", p.college);

            fseek(fp, -(long)sizeof(Participant), SEEK_CUR);
            fwrite(&p, sizeof(Participant), 1, fp);

            printf("Participant updated successfully!\n");
            fclose(fp);
            return;
        }
    }

    printf("Participant not found!\n");
    fclose(fp);
}

void cancel_participant()
{
    Participant p;
    int id, found = 0;

    FILE *fp = fopen("participants.dat", "rb");

    if (fp == NULL)
    {
        printf("No participants found!\n");
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");

    if (temp == NULL)
    {
        fclose(fp);
        printf("File error!\n");
        return;
    }

    printf("Enter Participant ID to cancel: ");
    scanf("%d", &id);

    while (fread(&p, sizeof(Participant), 1, fp) == 1)
    {
        if (p.pid == id)
            found = 1;
        else
            fwrite(&p, sizeof(Participant), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if (found)
    {
        remove("participants.dat");
        rename("temp.dat", "participants.dat");
        printf("Registration cancelled successfully!\n");
    }
    else
    {
        remove("temp.dat");
        printf("Participant not found!\n");
    }
}

int count_for_event(int event_id)
{
    Participant p;
    int count = 0;
    FILE *fp = fopen("participants.dat", "rb");

    if (fp == NULL)
        return 0;

    while (fread(&p, sizeof(Participant), 1, fp) == 1)
    {
        if (p.eventID == event_id)
            count++;
    }

    fclose(fp);
    return count;
}

void participant_menu()
{
    int choice;

    do
    {
        printf("\n--- PARTICIPANT REGISTRATION ---\n");
        printf("1. Register Participant\n");
        printf("2. List Participants\n");
        printf("3. Update Participant\n");
        printf("4. Cancel Registration\n");
        printf("5. Find Participant\n");
        printf("6. Participants in an Event\n");
        printf("7. Return to main menu\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                register_participant();
                break;

            case 2:
                list_participants();
                break;

            case 3:
                update_participant();
                break;

            case 4:
                cancel_participant();
                break;

            case 5:
            {
                int id;
                Participant p;

                printf("Enter Participant ID: ");
                scanf("%d", &id);

                if (find_participant(id, &p))
                    printf("Name: %s, Age: %d, Email: %s, Phone: %s, College: %s, Event ID: %d\n",
                           p.cname, p.age, p.email,
                           p.phone, p.college, p.eventID);
                else
                    printf("Participant not found!\n");

                break;
            }

            case 6:
            {
                int id;

                printf("Enter Event ID: ");
                scanf("%d", &id);

                printf("Participants Registered: %d\n",
                       count_for_event(id));
                break;
            }

            case 7:
                printf("Returning to main menu...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 7);
}
