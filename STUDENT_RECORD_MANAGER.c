#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 50
#define MAX_SEM 8
#define SUBJECTS 5

/* ---------- Structures ---------- */

struct Semester {
    int semNo;
    float marks[SUBJECTS];
    float sgpa;
    float attendance;
};

struct Student {
    int roll;
    char name[50];
    int semCount;
    struct Semester sem[MAX_SEM];
};

/* ---------- Global Data ---------- */

struct Student students[MAX_STUDENTS];
int studentCount = 0;

/* ---------- Function Declarations ---------- */

void loadData();
void saveData();

void addStudent();
void addSemester();
void updateSemester();
void displayStudents();
void searchStudent();
void sortStudents();

void calculateSGPA(struct Semester *s);

/* ---------- Main Function ---------- */

int main() {
    int choice;
    loadData();

    do {
        printf("\n========================================\n");
        printf("   STUDENT RECORD MANAGER (SGPA BASED)\n");
        printf("========================================\n");
        printf("1. Add New Student\n");
        printf("2. Add Semester Details\n");
        printf("3. Update Semester Details\n");
        printf("4. Display All Records\n");
        printf("5. Search Student\n");
        printf("6. Sort Students by Roll Number\n");
        printf("7. Exit (Auto Save)\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: addSemester(); break;
            case 3: updateSemester(); break;
            case 4: displayStudents(); break;
            case 5: searchStudent(); break;
            case 6: sortStudents(); break;
            case 7: saveData();
                    printf("Data saved successfully. Exiting...\n");
                    break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while (choice != 7);

    return 0;
}

/* ---------- File Handling ---------- */

void loadData() {
    FILE *fp = fopen("students_sgpa.dat", "rb");
    if (fp == NULL)
        return;

    fread(&studentCount, sizeof(int), 1, fp);
    fread(students, sizeof(struct Student), studentCount, fp);
    fclose(fp);
}

void saveData() {
    FILE *fp = fopen("students_sgpa.dat", "wb");
    if (fp == NULL) {
        printf("File error!\n");
        return;
    }
    fwrite(&studentCount, sizeof(int), 1, fp);
    fwrite(students, sizeof(struct Student), studentCount, fp);
    fclose(fp);
}

/* ---------- Utility Functions ---------- */

void calculateSGPA(struct Semester *s) {
    float total = 0;
    for (int i = 0; i < SUBJECTS; i++) {
        if (s->marks[i] >= 90) total += 10;
        else if (s->marks[i] >= 80) total += 9;
        else if (s->marks[i] >= 70) total += 8;
        else if (s->marks[i] >= 60) total += 7;
        else if (s->marks[i] >= 50) total += 6;
        else total += 5;
    }
    s->sgpa = total / SUBJECTS;
}

/* ---------- Core Functionalities ---------- */

void addStudent() {
    if (studentCount >= MAX_STUDENTS) {
        printf("Student limit reached!\n");
        return;
    }

    struct Student s;
    printf("Enter Roll Number: ");
    scanf("%d", &s.roll);
    getchar();

    printf("Enter Name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0';

    s.semCount = 0;
    students[studentCount++] = s;

    saveData();
    printf("Student added successfully!\n");
}

void addSemester() {
    int roll;
    printf("Enter Roll Number: ");
    scanf("%d", &roll);

    for (int i = 0; i < studentCount; i++) {
        if (students[i].roll == roll) {
            if (students[i].semCount >= MAX_SEM) {
                printf("Maximum semesters reached!\n");
                return;
            }

            struct Semester *s = &students[i].sem[students[i].semCount];

            printf("Enter Semester Number: ");
            scanf("%d", &s->semNo);

            printf("Enter marks of %d subjects:\n", SUBJECTS);
            for (int j = 0; j < SUBJECTS; j++)
                scanf("%f", &s->marks[j]);

            printf("Enter Attendance Percentage: ");
            scanf("%f", &s->attendance);

            calculateSGPA(s);
            students[i].semCount++;

            saveData();
            printf("Semester added successfully!\n");

            if (s->attendance < 75)
                printf("Warning: Attendance below 75%%!\n");

            return;
        }
    }
    printf("Student not found!\n");
}

void updateSemester() {
    int roll, semNo;
    printf("Enter Roll Number: ");
    scanf("%d", &roll);

    for (int i = 0; i < studentCount; i++) {
        if (students[i].roll == roll) {
            printf("Enter Semester Number to update: ");
            scanf("%d", &semNo);

            for (int j = 0; j < students[i].semCount; j++) {
                if (students[i].sem[j].semNo == semNo) {

                    printf("Enter new marks:\n");
                    for (int k = 0; k < SUBJECTS; k++)
                        scanf("%f", &students[i].sem[j].marks[k]);

                    printf("Enter new attendance: ");
                    scanf("%f", &students[i].sem[j].attendance);

                    calculateSGPA(&students[i].sem[j]);
                    saveData();

                    printf("Semester updated successfully!\n");
                    if (students[i].sem[j].attendance < 75)
                        printf("Warning: Attendance below 75%%!\n");

                    return;
                }
            }
            printf("Semester not found!\n");
            return;
        }
    }
    printf("Student not found!\n");
}

void displayStudents() {
    if (studentCount == 0) {
        printf("No records available.\n");
        return;
    }

    for (int i = 0; i < studentCount; i++) {
        printf("\nRoll: %d | Name: %s\n", students[i].roll, students[i].name);
        for (int j = 0; j < students[i].semCount; j++) {
            printf("  Semester %d -> SGPA: %.2f | Attendance: %.2f%%\n",
                   students[i].sem[j].semNo,
                   students[i].sem[j].sgpa,
                   students[i].sem[j].attendance);
        }
    }
}

void searchStudent() {
    int roll;
    printf("Enter Roll Number: ");
    scanf("%d", &roll);

    for (int i = 0; i < studentCount; i++) {
        if (students[i].roll == roll) {
            printf("Name: %s\n", students[i].name);
            for (int j = 0; j < students[i].semCount; j++) {
                printf("  Semester %d | SGPA: %.2f | Attendance: %.2f%%\n",
                       students[i].sem[j].semNo,
                       students[i].sem[j].sgpa,
                       students[i].sem[j].attendance);
            }
            return;
        }
    }
    printf("Student not found!\n");
}

void sortStudents() {
    struct Student temp;
    for (int i = 0; i < studentCount - 1; i++) {
        for (int j = i + 1; j < studentCount; j++) {
            if (students[i].roll > students[j].roll) {
                temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }
    saveData();
    printf("Students sorted by roll number successfully!\n");
}