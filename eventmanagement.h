#ifndef EVENTMANAGEMENT_H
#define EVENTMANAGEMENT_H

#define MAX_EVENT 5

typedef struct{
    int id;
    char name[50];
    int capacity;
}Event;

void add_event();
void list_events();
void update_event();
void delete_event();
int find_event(int id, Event *out);
int seats_taken(int event_id);
void event_menu();

#endif