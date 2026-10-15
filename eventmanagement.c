//Module 1: Event Management

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eventmanagement.h"
#include "participantregistration.h"


void add_event()
{
    Event e;
    FILE *fp = fopen("events.dat", "ab");

    if (fp == NULL)
    {
        printf("File error!\n");
        return;
    }

    printf("Enter Event ID: ");
    scanf("%d", &e.id);

    printf("Enter Event Name: ");
    scanf(" %49[^\n]", e.name);

    printf("Enter Capacity: ");
    scanf("%d", &e.capacity);

    fwrite(&e, sizeof(Event), 1, fp);
    fclose(fp);

    printf("Event added successfully!\n");
}

void list_events()
{
    Event e;
    FILE *fp = fopen("events.dat", "rb");

    if (fp == NULL)
    {
        printf("No events found!\n");
        return;
    }

    printf("\nID\tName\tCapacity\tSeats Taken\n");

    while (fread(&e, sizeof(Event), 1, fp) == 1)
    {
        printf("%d\t%s\t%d\t\t%d\n",
               e.id, e.name, e.capacity,
               seats_taken(e.id));
    }

    fclose(fp);
}

int find_event(int id, Event *out)
{
    Event e;
    FILE *fp = fopen("events.dat", "rb");

    if (fp == NULL)
        return 0;

    while (fread(&e, sizeof(Event), 1, fp) == 1)
    {
        if (e.id == id)
        {
            *out = e;
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void update_event()
{
    Event e;
    int id;
    FILE *fp = fopen("events.dat", "rb+");

    if (fp == NULL)
    {
        printf("No events found!\n");
        return;
    }

    printf("Enter Event ID to update: ");
    scanf("%d", &id);

    while (fread(&e, sizeof(Event), 1, fp) == 1)
    {
        if (e.id == id)
        {
            printf("Enter new Event Name: ");
            scanf(" %49[^\n]", e.name);

            printf("Enter new Capacity: ");
            scanf("%d", &e.capacity);

            fseek(fp, -(long)sizeof(Event), SEEK_CUR);
            fwrite(&e, sizeof(Event), 1, fp);

            printf("Event updated successfully!\n");
            fclose(fp);
            return;
        }
    }

    printf("Event not found!\n");
    fclose(fp);
}

void delete_event()
{
    Event e;
    int id, found = 0;

    FILE *fp = fopen("events.dat", "rb");

    if (fp == NULL)
    {
        printf("No events found!\n");
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");

    if (temp == NULL)
    {
        fclose(fp);
        printf("File error!\n");
        return;
    }

    printf("Enter Event ID to delete: ");
    scanf("%d", &id);

    while (fread(&e, sizeof(Event), 1, fp) == 1)
    {
        if (e.id == id)
            found = 1;
        else
            fwrite(&e, sizeof(Event), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if (found)
    {
        remove("events.dat");
        rename("temp.dat", "events.dat");
        printf("Event deleted successfully!\n");
    }
    else
    {
        remove("temp.dat");
        printf("Event not found!\n");
    }
}

int seats_taken(int event_id)
{
    (void)event_id;
    /* Registration data is required
       to calculate actual seats taken. */
    return count_for_event(event_id);
}

void event_menu()
{
    int choice;

    do
    {
        printf("\n--- EVENT MANAGEMENT ---\n");
        printf("1. Add Event\n");
        printf("2. List Events\n");
        printf("3. Update Event\n");
        printf("4. Delete Event\n");
        printf("5. Find Event\n");
        printf("6. Seats Taken\n");
        printf("7. Return to main menu...\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                add_event();
                break;

            case 2:
                list_events();
                break;

            case 3:
                update_event();
                break;

            case 4:
                delete_event();
                break;

            case 5:
            {
                int id;
                Event e;

                printf("Enter Event ID: ");
                scanf("%d", &id);

                if (find_event(id, &e))
                    printf("Event: %s, Capacity: %d\n",
                           e.name, e.capacity);
                else
                    printf("Event not found!\n");

                break;
            }

            case 6:
            {
                int id;

                printf("Enter Event ID: ");
                scanf("%d", &id);

                printf("Seats Taken: %d\n",
                       seats_taken(id));
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

