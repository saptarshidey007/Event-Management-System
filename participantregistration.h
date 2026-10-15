#ifndef PARTICIPANTREGISTRATION_H
#define PARTICIPANTREGISTRATION_H

typedef struct{
    int pid;
    char cname[50];
    int age;
    char email[50];
    char phone[11];
    char college[50];
    int eventID;
}Participant;

void register_participant();
void list_participants();
void update_participant();
void cancel_participant();
int find_participant(int pid, Participant *out);
int count_for_event(int event_id);
void participant_menu();

#endif