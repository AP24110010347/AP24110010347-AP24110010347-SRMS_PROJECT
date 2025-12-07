#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct Student {
    int roll;
    char name[50];
    float marks;
};

char currentRole[10];
char currentUser[50];

// Function Prototypes
int loginsystem(void);
void mainMenu(void);
void adminMenu(void);
void userMenu(void);
void staffMenu(void);
void guestMenu(void);
void addStudent(void);
void displayStudents(void);
void searchStudent(void);
void updateStudent(void);
void deleteStudent(void);

int main() {
    if (loginsystem()) {
        mainMenu();
    } else {
        printf("\nAccess Denied...\n");
    }
    return 0;
}

/*-------------------------- LOGIN SYSTEM --------------------------*/
int loginsystem() {
    char username[50], password[50];
    char fileUsername[50], filePassword[50], fileRole[20];

    printf("===== LOGIN =====\n");
    printf("Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;

    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) {
        printf("ERROR: credentials.txt NOT FOUND!\n");
        return 0;
    }

    while (fscanf(fp, "%s %s %s", fileUsername, filePassword, fileRole) == 3) {
        if (strcmp(username, fileUsername) == 0 &&
            strcmp(password, filePassword) == 0) {

            strcpy(currentRole, fileRole);
            strcpy(currentUser, fileUsername);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    printf("Invalid Login!\n");
    return 0;
}

/*-------------------------- MAIN MENU --------------------------*/
void mainMenu() {
    if (strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else if (strcmp(currentRole, "STAFF") == 0)
        staffMenu();
    else if (strcmp(currentRole, "USER") == 0)
        userMenu();
    else if (strcmp(currentRole, "GUEST") == 0)
        guestMenu();
    else
        printf("INVALID ROLE!\n");
}

/*-------------------------- ADMIN MENU --------------------------*/
void adminMenu() {
    int ch;

    while (1) {
        printf("\n===== ADMIN MENU =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &ch);
        getchar();

        switch (ch) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: return;
            default: printf("Invalid Choice!\n");
        }
    }
}

/*-------------------------- STAFF MENU --------------------------*/
void staffMenu() {
    int ch;

    while (1) {
        printf("\n===== STAFF MENU =====\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &ch);
        getchar();

        switch (ch) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: return;
            default: printf("Invalid Choice!\n");
        }
    }
}

/*-------------------------- USER MENU --------------------------*/
void userMenu() {
    int ch;

    while (1) {
        printf("\n===== USER MENU =====\n");
        printf("1. Display Students\n");
        printf("2. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &ch);
        getchar();

        switch (ch) {
            case 1: displayStudents(); break;
            case 2: return;
            default: printf("Invalid Choice!\n");
        }
    }
}

/*-------------------------- GUEST MENU --------------------------*/
void guestMenu() {
    printf("\nGuests do not have access to the system.\n");
}

/*-------------------------- ADD STUDENT --------------------------*/
void addStudent() {
    struct Student s;

    FILE *fp = fopen(STUDENT_FILE, "a");
    if (!fp) {
        printf("ERROR OPENING FILE!\n");
        return;
    }

    printf("\nEnter Roll Number: ");
    scanf("%d", &s.roll);
    getchar();

    printf("Enter Name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = 0;

    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fprintf(fp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    fclose(fp);

    printf("Student Added Successfully!\n");
}

/*-------------------------- DISPLAY STUDENTS --------------------------*/
void displayStudents() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student s;

    if (!fp) {
        printf("No Record Found!\n");
        return;
    }

    printf("\n===== STUDENT LIST =====\n");
    while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3) {
        printf("Roll: %d | Name: %s | Marks: %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
}

/*-------------------------- SEARCH STUDENT --------------------------*/
void searchStudent() {
    int roll, found = 0;
    struct Student s;

    printf("\nEnter Roll Number: ");
    scanf("%d", &roll);

    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("FILE NOT FOUND!\n");
        return;
    }

    while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == roll) {
            printf("\nSTUDENT FOUND:\n");
            printf("Roll: %d | Name: %s | Marks: %.2f\n", s.roll, s.name, s.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Student Not Found!\n");

    fclose(fp);
}

/*-------------------------- UPDATE STUDENT --------------------------*/
void updateStudent() {
    int roll, found = 0;
    struct Student s;

    printf("\nEnter Roll Number to Update: ");
    scanf("%d", &roll);
    getchar();

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("ERROR OPENING FILE!\n");
        return;
    }

    while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == roll) {
            found = 1;

            printf("Enter New Name: ");
            fgets(s.name, sizeof(s.name), stdin);
            s.name[strcspn(s.name, "\n")] = 0;

            printf("Enter New Marks: ");
            scanf("%f", &s.marks);
        }
        fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("STUDENT UPDATED SUCCESSFULLY!\n");
    else
        printf("Student Not Found!\n");
}

/*-------------------------- DELETE STUDENT --------------------------*/
void deleteStudent() {
    int roll, found = 0;
    struct Student s;

    printf("\nEnter Roll Number to Delete: ");
    scanf("%d", &roll);

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("ERROR OPENING FILE!\n");
        return;
    }

    while (fscanf(fp, "%d %49s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll != roll) {
            fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("STUDENT DELETED SUCCESSFULLY!\n");
    else
        printf("Student Not Found!\n");
}


