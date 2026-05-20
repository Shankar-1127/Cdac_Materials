#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<time.h>
#include<string.h>

#define ENTER 13
#define TAB 9
#define BCKSPC 8

#define ADMIN_PASSWORD "admin123"   // set your admin password here

struct user {
    char fullName[50];
    char email[50];
    char password[50];
    char username[50];
    char phone[50];
};

struct loginHistory {
    char username[50];
    char status[20];   // "Login Success", "Login Failed", "Logout"
    char timestamp[50];
};

void takeinput(char ch[50]){
    fgets(ch,50,stdin);
    ch[strlen(ch) - 1] = 0; 
}

void takepassword(char pwd[50]){
    int i = 0;
    char ch;
    while(1){
        ch = getch();
        if(ch == ENTER || ch == TAB){
            pwd[i] = '\0';
            break;
        } else if(ch == BCKSPC){
            if(i > 0){
                i--;
                printf("\b \b");
            }
        } else {
            pwd[i++] = ch;
            printf("* \b");
        }
    }
}

// Generate usernames serially: VL_User + counter
int loadCounter(){
    FILE *fp = fopen("counter.dat", "r");
    int counter = 1;
    if(fp != NULL){
        fscanf(fp, "%d", &counter);
        fclose(fp);
    }
    return counter;
}

void saveCounter(int counter){
    FILE *fp = fopen("counter.dat", "w");
    fprintf(fp, "%d", counter);
    fclose(fp);
}

void generateUsername(char username[50], int counter){
    sprintf(username, "VL_User%d", counter);
}

// Save login/logout history
void saveLoginHistory(char username[50], char status[20]){
    FILE *fp;
    struct loginHistory log;
    time_t now;
    time(&now);
    strcpy(log.username, username);
    strcpy(log.status, status);
    strftime(log.timestamp, sizeof(log.timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

    fp = fopen("LoginHistory.dat", "a+");
    fwrite(&log, sizeof(struct loginHistory), 1, fp);
    fclose(fp);
}

// View login/logout history for a specific user
void viewUserHistory(char username[50]){
    FILE *fp = fopen("LoginHistory.dat", "r");
    struct loginHistory log;
    int found = 0;
    if(fp == NULL){
        printf("\nNo login history found!\n");
        return;
    }
    printf("\n\t--- Login/Logout History for %s ---\n", username);
    while(fread(&log, sizeof(struct loginHistory), 1, fp)){
        if(!strcmp(log.username, username)){
            printf("Status: %-12s | Time: %s\n", log.status, log.timestamp);
            found = 1;
        }
    }
    if(!found) printf("No history for this user.\n");
    fclose(fp);
}

// Admin view all users
void viewAllUsers(){
    FILE *fp = fopen("Users.dat", "r");
    struct user usr;
    int i = 1;
    if(fp == NULL){
        printf("\nNo users registered yet!\n");
        return;
    }
    printf("\n\t--- Registered Users ---\n");
    while(fread(&usr, sizeof(struct user), 1, fp)){
        printf("%d. %s (%s)\n", i++, usr.username, usr.fullName);
    }
    fclose(fp);
}

int main(){
    system("color 0b");
    FILE *fp;
    int opt, usrFound = 0;
    struct user user;
    char password2[50];

    printf("\n\t\t\t\t----------Welcome to authentication system----------");
    printf("\nPlease choose your operation");
    printf("\n1.Signup");
    printf("\n2.Login");
    printf("\n3.Admin: View Users & Histories");
    printf("\n4.Exit");

    printf("\n\nYour choice:\t");
    scanf("%d",&opt);
    fgetc(stdin);

    switch(opt){
        case 1:
            system("cls");
            printf("\nEnter your full name:\t");
            takeinput(user.fullName);
            printf("Enter your email:\t");
            takeinput(user.email);
            printf("Enter your contact no:\t");
            takeinput(user.phone);
            printf("Enter your password:\t");
            takepassword(user.password);
            printf("\nConfirm your password:\t");
            takepassword(password2);

            if(!strcmp(user.password,password2)){
                int counter = loadCounter();
                generateUsername(user.username, counter);
                saveCounter(counter + 1);

                fp = fopen("Users.dat","a+");
                fwrite(&user,sizeof(struct user),1,fp);
                if(fwrite != 0) 
                    printf("\n\nUser registration success, Your username is %s",user.username);
                else 
                    printf("\n\nSorry! Something went wrong :(");
                fclose(fp);
            }
            else{
                printf("\n\nPassword did not match");
                Beep(750,300);
            }
        break;

        case 2:
            {
                char username[50], pword[50];
                struct user usr;

                printf("\nEnter your username:\t");
                takeinput(username);
                printf("Enter your password:\t");
                takepassword(pword);

                fp = fopen("Users.dat","r");
                while(fread(&usr,sizeof(struct user),1,fp)){
                    if(!strcmp(usr.username,username)){
                        if(!strcmp(usr.password,pword)){
                            system("cls");
                            printf("\n\t\t\t\t\t\tWelcome %s",usr.fullName);
                            printf("\n\n|Full Name:\t%s",usr.fullName);
                            printf("\n|Email:\t\t%s",usr.email);
                            printf("\n|Username:\t%s",usr.username);
                            printf("\n|Contact no.:\t%s",usr.phone);

                            saveLoginHistory(username, "Login Success");

                            // Logout option
                            int choice;
                            printf("\n\nPress 1 to Logout: ");
                            scanf("%d",&choice);
                            if(choice == 1){
                                saveLoginHistory(username, "Logout");
                                printf("\nYou have logged out successfully.\n");
                            }
                        }
                        else {
                            printf("\n\nInvalid Password!");
                            Beep(800,300);
                            saveLoginHistory(username, "Login Failed");
                        }
                        usrFound = 1;
                    }
                }
                if(!usrFound){
                    printf("\n\nUser is not registered!");
                    Beep(800,300);
                    saveLoginHistory(username, "Login Failed");
                }
                fclose(fp);
            }
            break;

        case 3:
            {
                char adminPwd[50];
                printf("\nEnter Admin Password: ");
                takepassword(adminPwd);
                if(strcmp(adminPwd, ADMIN_PASSWORD) == 0){
                    system("cls");
                    viewAllUsers();
                    printf("\nEnter username to view history: ");
                    char uname[50];
                    takeinput(uname);
                    viewUserHistory(uname);
                } else {
                    printf("\nInvalid Admin Password! Access Denied.\n");
                    Beep(800,300);
                }
            }
            break;

        case 4:
            printf("\t\t\tBye Bye :)");
            return 0;
    }

    return 0;
}
