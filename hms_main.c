#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include "include/colors.h"
#include <time.h>
#include <stdbool.h>
#include <windows.h>
#include<unistd.h>
#define MAX_USERS 1000
#define FILE_PATIENT_LIST "Patients.txt"
#define FILE_USERS "Users.txt"
#define MAX_UNAME_LENGTH 30
#define TEMP_FILE_USERS "tempUsers.txt"
#define TEMP_FILE_PATIENTS "tempPatients.txt"
#define MAX_PASSWORD_LENGTH 8
#define MAX_PATIENTS 1000
#define FILE_DATABASE "database.txt"




// STRUCTURE DECLARATIONS DEFINITIONS //

struct User{
    char username[MAX_UNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

struct Patient{
    int id;
    char full_name[50];
    int age;
    char gender[50];
    char adm_date[20];
    char dis_date[20];
};

struct Patient patients[MAX_PATIENTS];
struct User users[MAX_USERS];





// PROTOTYPE FUNCTIONS //




// INTERFACE FUNCTION PROTOTYPES //

void delay(int seconds);
void centre_div();
void clear_buffer();
void press_enter();
void display_brand();
void display_front_screen();


// USER-HELPING MODULE FUNCTION PROTOTYPES //


int init_menu();
int login(struct User users[], int num_of_users);
int handle_login_failure(struct User users[], int num_of_users);
int register_user(struct User users[], int *num_of_users);
void delete_user_account(struct User users[], int *num_of_users, const char *username);
int is_user_exist(char username[], struct User users[], int num_of_users);
void create_new_record(char username[]);
void hide_password_input(char *password);
void check_user_data(struct User users[], int *num_of_users);
void write_user_data_to_file(struct User user);



void main_menu( char username[], int *num_of_users);
void add_patient(char username[],struct Patient patients[], int* number_of_patients);
void patient_list(char username[], int num_of_patients);
void discharge_patient(int patient_id);
void delete_patient_record(int patient_id, int* num_of_patients);
int main();







/// FUNCTION DEFINITIONS AND BUILDING ///




// INTERFACE FUNCTION DEFINITIONS //


void delay(int seconds) {                      // SLEEP FUNCTION DEFINITION //
    Sleep(seconds * 1000); 
}


void centre_div(){
    printf("\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t");
}

void clear_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void press_enter()
{
    printf("Press any key to continue...");
    fflush(stdout);
    clear_buffer();
    getchar();
}

void display_brand(){
    printf("\n\n\t\t\t\t\t\t\t\t    %s+%s %sCare%s%sWave%s  %sHealth%s  %sAssis%s%stant%s %s+%s",HRED,reset,HBLU,reset,HWHT,reset,HGRN,reset,HWHT,reset,HBLU,reset,HRED,reset);

}

void display_front_screen(){
    centre_div();
    printf("%s+%s %sCare%s%sWave%s  %sHealth%s  %sAssis%s%stant%s %s+%s",HRED,reset,HBLU,reset,HWHT,reset,HGRN,reset,HWHT,reset,HBLU,reset,HRED,reset);
    delay(1);
    init_menu();
}



// INITIAL MENU FUNCTION DEFINITION //

int init_menu(){
    int ch;
    int userIndex,newUserIndex;
    int num_of_users=0;
    do{
    system("cls");
    check_user_data(users, &num_of_users);
    display_brand();
    centre_div();
    printf("%s1%s. %sLogin%s\n\n",HYEL,reset,HGRN,reset);
    printf("\t\t\t\t\t\t\t\t%s2%s. %sRegister%s with %sCare%s%sWave%s\n\n",HYEL,reset,HGRN,reset,HBLU,reset,HWHT,reset);
    printf("\t\t\t\t\t\t\t\t%s3%s. %sExit%s to desktop\n\n",HYEL, reset,BRED,reset);
    printf(BMAG"\n\n\t\t\t\t\t\t\t\tEnter your choice: "reset);
    scanf("%d", &ch);
    char uname[MAX_UNAME_LENGTH];
    switch(ch){
        case 1:
            userIndex = login(users, num_of_users);
            if (userIndex != -1) {
                strcpy(uname, users[userIndex].username);
                main_menu(uname, &num_of_users);
            }
            break;
        case 2:
            newUserIndex = register_user(users, &num_of_users);
                if (newUserIndex != -1) {
                    strcpy(uname, users[newUserIndex].username);
                    main_menu(uname, &num_of_users);
                }
                break;
        case 3:
            printf(RED "\n\n\t\t\t\t\t\t\t\tExiting Application...\n" reset);
            exit(0);
        default:
            printf(HRED"\n\n\t\t\t\t\t\t\t\tEnter a valid choice.\n" reset);
            break;
    }
    }while(ch!=3);
    
}




// USER-LOGIN FUNCTION DEFINITION //

int login(struct User users[], int num_of_users) {
    system("cls");
    display_brand();
    centre_div();
    printf("\t%sLOGIN%s\n", BMAG,reset);
    printf(HGRN "\t\t\t\t\t\t\t\tPlease enter your username: " reset);
    char username[MAX_UNAME_LENGTH];
    scanf("%s", username);

    int userIndex = is_user_exist(username, users, num_of_users);

    if (userIndex != -1) {
        int chances = 5;

        while (chances > 0) {
            char password[MAX_PASSWORD_LENGTH];
            hide_password_input(&password);

            if (strcmp(users[userIndex].password, password) == 0) {
                system("cls");
                display_brand();
                centre_div();
                printf(BGRN "Login successful. Please Wait. Redirecting...\n" reset);
                delay(1);
                return userIndex; 
            } else {
                chances--;
                printf(LIGHT_RED "\n\n\n\t\t\t\t\t\t\t\tIncorrect password. Please try again!\n" reset);
                printf("\t\t\t\t\t\t\t\t[%sRemaining attempts:%s %s%d%s]\n\n", CYN, reset, LIGHT_YELLOW, chances, reset);
                delay(3);
            }
        }
        printf(HMAG "\n\n\n\t\t\t\t\t\t\t\tYou've run out of login attempts. Please try again later.\n" reset);
        delay(2);
    } else {
        return handle_login_failure(users,num_of_users);
    }

    printf(LIGHT_RED "\n\n\n\t\t\t\t\t\t\t\tLogin failed. Returning to the main menu...\n" reset);
    delay(1);
    return -1;
}


//LOGIN ERROR-HANDLING FUNCTION DEFINITION //

int handle_login_failure(struct User users[], int num_of_users) {
    printf(LIGHT_RED "\n\t\t\t\t\t\t\t\tThe entered username does not exist.\n" reset);
    delay(2);
    system("cls");
    display_brand();
    centre_div();
    printf(MAG"Please choose an option.\n"reset);
    printf("\t\t\t\t\t\t\t\t%s1.%s %sLogin Again%s\n", HCYN, reset,HGRN,reset);
    printf("\t\t\t\t\t\t\t\t%s2.%s %sRegister%s with %sCare%s%sWave%s\n", HCYN, reset, HGRN,reset,HBLU,reset,HWHT,reset);
    printf("\t\t\t\t\t\t\t\t%s3.%s %sExit%s to home screen.\n", HCYN, reset,HRED,reset);
    printf("\n\t\t\t\t\t\t\t\t%sEnter option:%s ", BYEL, reset);

    int choice;
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            return login(users, num_of_users);
        case 2:
            register_user(users, &num_of_users);
            return num_of_users - 1;
        default:
            printf(LIGHT_RED "\nExiting...\n" reset);
            return init_menu();
    }
}


// USER REGISTRATION FUNCTION DEFINITION //

int register_user(struct User users[], int *num_of_users) {
    struct User newUser;
    system("cls");
    display_brand();
    centre_div();
    printf("\t%sREGISTRATION%s\n\n", BMAG, reset);
    while (1) {
        char newUsername[MAX_UNAME_LENGTH];
        printf(HCYN"\n\t\t\t\t\t\t\t\tEnter a username that you will use to login: "reset);
        scanf("%s", newUsername);

        int userIndex = is_user_exist(newUsername, users, *num_of_users);
        if (userIndex != -1) {
            printf("\n\n\t\t\t\t\t\t\t\tProvided Username is in use. Please select a different username.\n");
        } else {
            strcpy(newUser.username, newUsername);
            break;
        }
    }

    while (1) {
        char newPassword[MAX_PASSWORD_LENGTH];
        printf(HCYN"\n\t\t\t\t\t\t\t\tPlease enter a password within 8 characters: "reset);
        scanf("%s", newPassword);
        if (strlen(newPassword) > MAX_PASSWORD_LENGTH || strlen(newPassword) == 0) {
            printf(HRED"\n\t\t\t\t\t\t\t\tInvalid password format. Please enter a password within 1 to 8 characters.\n"reset);
        } else {
            strcpy(newUser.password, newPassword);
            break;
        }
    }

    users[*num_of_users] = newUser;
    (*num_of_users)++;
    write_user_data_to_file(newUser);
    create_new_record(newUser.username);

    printf(HGRN"\n\n\n\t\t\t\t\t\t\t\tAccount registered successfully!\n"reset);
    printf(BGRN"\n\n\t\t\t\t\t\t\t\tYou're now being logged in...\n"reset);
    delay(1);
    int newUserIndex = *num_of_users-1;
    return newUserIndex;
}


// DELETE USER ACCOUNT FUNCTION DEFINITION //


void delete_user_account(struct User users[], int *num_of_users, const char *username) {
    FILE *originalFile = fopen(FILE_USERS, "r");
    FILE *tempFile = fopen("tempUsers.txt", "w");
    char ch;
    printf("\n\n\t\t\t\t\t\t\t\tAre you sure you want to delete this account, %s%s%s? [Y/N]: ",BHCYN,username,reset);
    scanf(" %c",&ch);
    delay(1);
    if(ch=='y'|| ch=='Y'){
        
    if (originalFile == NULL || tempFile == NULL) {
        printf(RED "Error opening files.\n" reset);
        return;
    }
    int found = 0;
    char buffer[100]; 
    while (fgets(buffer, sizeof(buffer), originalFile) != NULL) {
        char currentUsername[MAX_UNAME_LENGTH];
        sscanf(buffer, "%s", currentUsername);
        if (strcmp(currentUsername, username) != 0) {
            fputs(buffer, tempFile);
        } else {
            found = 1;
        }
    }
    fclose(originalFile);
    fclose(tempFile);

    if (!found) {
        printf(RED "\n\n\n\t\t\t\t\t\t\t\tUser not found.\n" reset);
        remove("tempUsers.txt");
        return;
    }
    if (remove(FILE_USERS) != 0) {
        printf(RED "\n\n\n\t\t\t\t\t\t\t\tError deleting original file.\n" reset);
        return;
    }
    if (rename("tempUsers.txt", FILE_USERS) != 0) {
        printf(RED "\n\n\n\t\t\t\t\t\t\t\tError renaming temporary file.\n" reset);
        return;
    }

    (*num_of_users)--;
    printf(GRN "\n\n\n\t\t\t\t\t\t\t\tAccount deleted successfully.\n" reset);
    }
    else{
    printf(BHYEL"\n\n\t\t\t\t\t\t\t\tAccount Deletion Cancelled.\n" reset);
    }
    printf(BHYEL"\n\n\t\t\t\t\t\t\t\tRedirecting you back..."reset);
    delay(3);
    return;
}





// USER VALIDATOR FUNCTION DEFINITION //

int is_user_exist(char username[], struct User users[], int num_of_users) {
    for (int i = 0; i < num_of_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}




// CREATE A REFRENCE RECORD FOR NEW USER IN THE DATABASE //

void create_new_record(char username[]) {
    FILE *file = fopen(FILE_DATABASE, "a");
    if (file == NULL) {
        printf(BRED"\n\n\n\t\t\t\t\t\t\t\tError opening file.\n"reset);
        delay(2);
        return;
    }
    fprintf(file, "%s 0.00\n", username);
    fclose(file);
}


// HIDE PASSWORD FUNCTION DEFINITION //

void hide_password_input(char *password) {
    int i = 0;
    char ch;
    printf(HGRN"\t\t\t\t\t\t\t\tPlease enter your password:"reset);
    while (1) {
        if (_kbhit()) {
            ch = _getch();

            if (ch == '\r') {
                password[i] = '\0';
                break;
            } else if (ch == '\b' && i > 0) {
                printf("\b \b");
                i--;
            } else if (ch != '\b') {
                password[i] = ch;
                printf("*");
                i++;
            }
        }
    }
}


// CHECK-USER-DATA FUNCTION DEFINITION //

void check_user_data(struct User users[], int *num_of_users){
    FILE *file = fopen(FILE_USERS, "r");

    if(file == NULL){
        printf(LIGHT_RED"\n\n\n\t\t\t\t\t\t\t\tUnable to access User data.\n" reset);
        return;
    }
    int count = 0;
    while(count< MAX_USERS && fscanf(file, "%s %s", users[count].username, &users[count].password)==2){
        count++;
    }
    *num_of_users =  count;
    fclose(file);
}




// SAVE USER DATA FUNCTION DEFINITION //

void write_user_data_to_file(struct User user) {
    FILE *file = fopen(FILE_USERS, "a");
    if (file == NULL) {
        printf(BRED"\n\n\t\t\t\t\t\t\t\tError opening file.\n"reset);
        return;
    }
    fprintf(file, "%s %s\n", user.username, user.password);
    fclose(file);
}



/// MISC DECLARATION ///
struct Patient patients[MAX_PATIENTS];


// MAIN (SERVICES) MENU FUNCTION DEFINITION //

void main_menu( char username[], int *num_of_users){
char user_name[MAX_UNAME_LENGTH];
strcpy(user_name, username);
int ch;
FILE *fp = FILE_PATIENT_LIST;
int patient_id;
int num_of_patients = 0;
do{
system("cls");
display_brand();
printf(COMMENT_GREY"\n\n\t\t\t\t\t\t\t\t\t\tUser: %s"reset, user_name);
centre_div();
printf(BHMAG"\tOUR SERVICES\n\n\n" reset);
printf(BHBLU "\n\t\t\t\t\t\t\t\t1. View Patients List.\n"reset);
printf(BHBLU "\n\t\t\t\t\t\t\t\t2. Add a Patient Record.\n"reset);
printf(BHBLU "\n\t\t\t\t\t\t\t\t3. Discharge a Patient.\n"reset);
printf(BHBLU "\n\t\t\t\t\t\t\t\t4. Delete Patient Record.\n"reset);
printf(BHBLU "\n\t\t\t\t\t\t\t\t5. Logout\n"reset);
printf(BHRED "\n\t\t\t\t\t\t\t\t6. Delete Your Account.\n"reset);
printf("\n\n\t\t\t\t\t\t\t\tEnter choice: ");
scanf("%d", &ch);

    switch(ch){
        case 1:
            patient_list(user_name, num_of_patients);
            break;
        case 2:
            add_patient(user_name,patients, &num_of_patients);
            break;
        case 3:
            system("cls");
            centre_div();
            printf("Enter the patient ID to discharge: ");
            scanf("%d", &patient_id);
            discharge_patient(patient_id);
            break;
        case 4:
            system("cls");
            display_brand();
            printf(COMMENT_GREY"\n\n\t\t\t\t\t\t\t\t\t\tUser: %s"reset, user_name);
            centre_div();
            
            printf("Enter the ID of the patient whose record you want to delete: ");
            scanf("%d", &patient_id);
            delete_patient_record(patient_id,&num_of_patients);
            break;
        case 5:
            system("cls");
            display_brand();
            printf(COMMENT_GREY"\n\n\t\t\t\t\t\t\t\t\t\tUser: %s"reset, user_name);
            centre_div();
            printf(BHRED"\n\n\n\t\t\t\t\t\t\t\tLogging you out...",reset);
            delay(3);
           return;
        case 6:
            delete_user_account(users,&num_of_users,user_name);
        default:
            printf(RED"\nEnter a valid choice!" reset);

    }
    
}while(ch!=6);
}



// ADD-PATIENT FUNCTION DEFINITION //

void add_patient(char username[], struct Patient patients[], int *num_of_patients) {
    if (*num_of_patients >= MAX_PATIENTS) {
        printf("Maximum number of patients reached.\n");
        return;
    }
    char user_name_cur[MAX_UNAME_LENGTH];
    strcpy(user_name_cur, username);
    struct Patient newPatient;
    system("cls");
    display_brand();
    printf(COMMENT_GREY"\n\n\t\t\t\t\t\t\t\t\t\tUser: %s"reset, user_name_cur);
    centre_div();
    printf("         %sPATIENT REGISTRATION\n%s", MAG, reset);
    printf(HGRN"\n\n\t\t\t\t\t\t\t\tEnter patient ID: "reset);
    scanf("%d", &newPatient.id);
    getchar(); 

    FILE *file = fopen(FILE_PATIENT_LIST, "rb");
    if (file == NULL) {
        printf("Error opening patient file for reading.\n");
        return;
    }

    struct Patient tempPatient;
    while (fread(&tempPatient, sizeof(struct Patient), 1, file) == 1) {
        if (tempPatient.id == newPatient.id) {
            printf("\n\n\t\t\t\t%sError: Patient with ID%s %s%d%s %salready exists.\n\t\t\t\tRedirecting you to previous page...%s",HRED,reset,HWHT, newPatient.id,reset,HRED,reset);
            fclose(file);
            delay(2);
            return;
        }
    }
    fclose(file);

    printf(HGRN"\n\t\t\t\t\t\t\t\tEnter patient's full name: "reset);
    fgets(newPatient.full_name, sizeof(newPatient.full_name), stdin);
    strtok(newPatient.full_name, "\n"); 

    printf(HGRN"\n\t\t\t\t\t\t\t\tEnter patient's age: "reset);
    scanf("%d", &newPatient.age);
    getchar(); 

    printf(HGRN"\n\t\t\t\t\t\t\t\tEnter patient's gender: "reset);
    fgets(newPatient.gender, sizeof(newPatient.gender), stdin);
    strtok(newPatient.gender, "\n"); 
    strcpy(newPatient.dis_date, "");

    time_t current_time;
    struct tm *local_time;
    time(&current_time);
    local_time = localtime(&current_time);
    strftime(newPatient.adm_date, sizeof(newPatient.adm_date), "%Y-%m-%d", local_time);

    file = fopen(FILE_PATIENT_LIST, "ab"); 
    if (file == NULL) {
        printf(BRED"\n\n\t\t\t\t\t\t\t\tError opening file for writing.\n"reset);
        return;
    }

    fwrite(&newPatient, sizeof(struct Patient), 1, file);
    fclose(file);

    patients[*num_of_patients] = newPatient; 
    (*num_of_patients)++; 
    printf(BHGRN"\n\n\t\t\t\t\t\t\t\tPatient Successfully registered in the database.\n\n\t\t\t\t\t\t\t\tRedirecting you back.\n"reset);
    delay(3);
}





//DISPLAY PATIENT LIST FUNCTION DEFINITION //

void patient_list(char username[], int num_of_patients) {
    
    int num_of_pat = 0;
    char user_name_cur[MAX_UNAME_LENGTH];
    strcpy(user_name_cur, username);

    FILE *file = fopen(FILE_PATIENT_LIST, "rb");
    if (file != NULL) {
        while (fread(&patients[num_of_pat], sizeof(struct Patient), 1, file) == 1) {
            num_of_pat++;
        }
        fclose(file);
        system("cls");
        display_brand();
        printf(COMMENT_GREY"\n\n\t\t\t\t\t\t\t\t\t\tUser: %s"reset, user_name_cur);
        centre_div();
        printf(HMAG"\t      PATIENT DETAILS\n\n\n"reset);
        printf(BHBLU"\t\t\t\t\t+------+----------------------+------+-----------+-----------------+-----------------+\n"reset);
        printf(BHWHT"\t\t\t\t\t| %4s | %-20s | %4s | %-9s | %-15s | %-15s |\n"reset, "ID", "Name", "Age", "Gender", "Admission Date", "Discharge Date",reset);
        printf(BHBLU"\t\t\t\t\t+------+----------------------+------+-----------+-----------------+-----------------+\n"reset);

        for (int i = 0; i < num_of_pat; i++) {
            printf(BHWHT"\t\t\t\t\t| %4d | %-20s | %4d | %-9s | %-15s | %-15s |\n", patients[i].id, patients[i].full_name, patients[i].age, patients[i].gender,patients[i].adm_date, strlen(patients[i].dis_date) > 0 ? patients[i].dis_date : "N/A");
            printf("\t\t\t\t\t+------+----------------------+------+-----------+-----------------+-----------------+\n"reset);
        }
    } else {
        printf("\n\n\n\t\t\t\t\t\t\t\tError opening file for reading.\n");
        delay(3);
    }
    delay(2);
    printf("\n\n\n\n\n\t\t\t\t\t\t\t\t");
    press_enter();
}

// void discharge_patient(struct Patient patients[], int num_of_patients) {
//     int patient_id;
//     printf("Enter the patient ID to discharge: ");
//     scanf("%d", &patient_id);
//     // Find the patient by ID
//     int index = -1;
//     for (int i = 0; i < num_of_patients; i++) {
//         if (patients[i].id == patient_id) {
//             index = i;
//             break;
//         }
//     }
//     if (index != -1) {
//         // Get the current date
//         time_t current_time;
//         struct tm *local_time;
//         time(&current_time);
//         local_time = localtime(&current_time);
//         strftime(patients[index].dis_date, sizeof(patients[index].dis_date), "%Y-%m-%d", local_time);
//         // Update the patient information in the file
//         FILE *file = fopen(FILE_PATIENT_LIST, "wb");
//         if (file != NULL) {
//             fwrite(patients, sizeof(struct Patient), num_of_patients, file);
//             fclose(file);
//         } else {
//             printf("Error opening file for writing.\n");
//         }
//         printf("Patient with ID %d has been discharged.\n", patient_id);
//     } else {
//         printf("Patient with ID %d not found.\n", patient_id);
//     }
// }



// DISCHARGE-PATIENT FUNCTION DEFINITION //
void discharge_patient(int patient_id) {

    FILE *file = fopen(FILE_PATIENT_LIST, "rb+");
    if (file == NULL) {
        printf("\n\n\t\t\t\t\t\t\t\tError opening file for reading and writing.\n");
        delay(3);
        return;
    }
    
    struct Patient patient;
    int found = 0;
    while (fread(&patient, sizeof(struct Patient), 1, file) == 1) {
        if (patient.id == patient_id) {
            found = 1;
            if(strcmp(patient.dis_date,"")==0){
                time_t current_time;
                struct tm *local_time;
                time(&current_time);
                local_time = localtime(&current_time);
                strftime(patient.dis_date, sizeof(patient.dis_date), "%Y-%m-%d", local_time);
                fseek(file, -sizeof(struct Patient), SEEK_CUR);
                fwrite(&patient, sizeof(struct Patient), 1, file);
                printf(BHGRN"\n\n\t\t\t\t\t\t\t\tPatient with ID %d has been discharged.\n"reset, patient_id);
                break;
            }
            else{
                printf(BHRED"\n\n\t\t\t\t\t\t\t\tPatient with ID %d has already been discharged.\n"reset, patient_id);
            }
        }
    }

    if (!found) {
        printf(BRED"\n\n\n\t\t\t\t\t\t\t\tPatient with ID %d not found.\n"reset, patient_id);
    }

    fclose(file);
    delay(2);
}


// DELETE PATIENT-RECORD FUNCTION DEIFINITION //

void delete_patient_record(int patient_id, int *num_of_patients) {

    FILE *file = fopen(FILE_PATIENT_LIST, "rb");
    if (file == NULL) {
        printf("\n\n\t\t\t\t\t\t\t\tError opening patient file for reading.\n");
        delay(3);
        return;
    }

    struct Patient tempPatient;
    FILE *tempFile = fopen(TEMP_FILE_PATIENTS, "wb");
    if (tempFile == NULL) {
        printf(BRED"\n\n\t\t\t\t\t\t\t\tError opening temporary file for writing.\n"reset);
        fclose(file);
        delay(3);
        return;
    }

    int found = 0;
    while (fread(&tempPatient, sizeof(struct Patient), 1, file) == 1) {
        if (tempPatient.id != patient_id) {
            fwrite(&tempPatient, sizeof(struct Patient), 1, tempFile);
        } else {
            found = 1;
            printf("\n\n\n\t\t\t\t\t\t\t\t%sPatient with ID%s %s%d%s %sfound and deleted.%s\n",BHCYN,reset,BHWHT,patient_id,reset,BHCYN,reset);
            (*num_of_patients)--;
        }
    }

    fclose(file);
    fclose(tempFile);

    if (!found) {
        printf(BRED"\n\n\t\t\t\t\t\t\t\tPatient with ID %d not found.\n"reset, patient_id);
        remove(TEMP_FILE_PATIENTS); 
    } else {
        remove(FILE_PATIENT_LIST); 
        rename(TEMP_FILE_PATIENTS, FILE_PATIENT_LIST); 
    }
    delay(2);
}



/// MAIN FUNCTION ///

int main(){
    display_front_screen();
    exit(0);
}


/// THIS IS IT ///  