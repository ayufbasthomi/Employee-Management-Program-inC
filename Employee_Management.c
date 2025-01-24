#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILENAME "employees.txt"
#define MAX_EMPLOYEES 100
#define MAX_STRING 100

typedef struct {
    char id[MAX_STRING];
    char name[MAX_STRING];
    char phone[MAX_STRING];
    char position[MAX_STRING];
    char email[MAX_STRING];
} Employee;

Employee employees[MAX_EMPLOYEES];
int employeeCount = 0;

// Helper function to validate ID (3-digit numeric)
int isValidID(const char *id) {
    if (strlen(id) != 3) return 0;
    for (int i = 0; i < 3; i++) {
        if (!isdigit(id[i])) return 0;
    }
    return 1;
}

// Helper function to validate phone number (XX-XXX-XXXX or XXX-XXXX-XXXX)
int isValidPhone(const char *phone) {
    int len = strlen(phone);
    if (len < 11 || len > 13) return 0; // Check overall length
    if (phone[2] != '-' && phone[3] != '-') return 0; // Check for dash at the right place
    for (int i = 0; i < len; i++) {
        if (i == 2 || i == 6) {
            if (phone[i] != '-') return 0; // Ensure dashes are in the right places
        } else if (!isdigit(phone[i])) {
            return 0; // Ensure all other characters are digits
        }
    }
    return 1;
}

// Helper function to validate email format
int isValidEmail(const char *email) {
    const char *at = strchr(email, '@');
    const char *dot = strrchr(email, '.');
    return (at && dot && at < dot && (dot - at) > 1 && strlen(dot) > 1);
}

// Load employees from file
void loadEmployees() {
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        return;
    }

    char line[MAX_STRING];
    employeeCount = 0;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%s",
                   employees[employeeCount].id,
                   employees[employeeCount].name,
                   employees[employeeCount].phone,
                   employees[employeeCount].position,
                   employees[employeeCount].email) == 5) {
            employeeCount++;
        }
    }

    fclose(file);
}

// Save employees to file
void saveEmployees() {
    FILE *file = fopen(FILENAME, "w");
    if (!file) {
        printf("Error saving employees to file.\n");
        return;
    }

    for (int i = 0; i < employeeCount; i++) {
        fprintf(file, "%s,%s,%s,%s,%s\n",
                employees[i].id,
                employees[i].name,
                employees[i].phone,
                employees[i].position,
                employees[i].email);
    }

    fclose(file);
}

// Add a new employee
void addEmployee() {
    if (employeeCount >= MAX_EMPLOYEES) {
        printf("Maximum employee limit reached.\n");
        return;
    }

    Employee emp;
    printf("Enter Employee ID (3-digit numeric): ");
    scanf("%s", emp.id);
    if (!isValidID(emp.id)) {
        printf("Invalid ID format! Must be 3 digits.\n");
        return;
    }

    for (int i = 0; i < employeeCount; i++) {
        if (strcmp(employees[i].id, emp.id) == 0) {
            printf("Employee ID must be unique!\n");
            return;
        }
    }

    printf("Enter Name: ");
    getchar(); // Consume newline character
    fgets(emp.name, MAX_STRING, stdin);
    emp.name[strcspn(emp.name, "\n")] = 0;

    printf("Enter Phone Number (XX-XXX-XXXX or XXX-XXXX-XXXX): ");
    scanf("%s", emp.phone);
    if (!isValidPhone(emp.phone)) {
        printf("Invalid phone number format!\n");
        return;
    }

    printf("Enter Position: ");
    getchar();
    fgets(emp.position, MAX_STRING, stdin);
    emp.position[strcspn(emp.position, "\n")] = 0;

    printf("Enter Email Address: ");
    scanf("%s", emp.email);
    if (!isValidEmail(emp.email)) {
        printf("Invalid email address format!\n");
        return;
    }

    employees[employeeCount++] = emp;
    saveEmployees();
    printf("Employee added successfully!\n");
}

// List all employees
void listEmployees() {
    printf("Employee List:\n");
    for (int i = 0; i < employeeCount; i++) {
        printf("ID: %s, Name: %s, Phone: %s\n",
               employees[i].id, employees[i].name, employees[i].phone);
    }
}

// View detailed information of an employee
void viewEmployee() {
    char id[MAX_STRING];
    printf("Enter Employee ID: ");
    scanf("%s", id);

    for (int i = 0; i < employeeCount; i++) {
        if (strcmp(employees[i].id, id) == 0) {
            printf("ID: %s\nName: %s\nPhone: %s\nPosition: %s\nEmail: %s\n",
                   employees[i].id, employees[i].name, employees[i].phone,
                   employees[i].position, employees[i].email);
            return;
        }
    }

    printf("Employee not found!\n");
}

// Edit an employee's information
void editEmployee() {
    char id[MAX_STRING];
    printf("Enter Employee ID to edit: ");
    scanf("%s", id);

    for (int i = 0; i < employeeCount; i++) {
        if (strcmp(employees[i].id, id) == 0) {
            printf("Editing Employee: %s\n", employees[i].name);

            printf("Enter New Name (leave blank to keep current): ");
            getchar();
            char name[MAX_STRING];
            fgets(name, MAX_STRING, stdin);
            if (strcmp(name, "\n") != 0) {
                name[strcspn(name, "\n")] = 0;
                strcpy(employees[i].name, name);
            }

            printf("Enter New Phone (leave blank to keep current): ");
            char phone[MAX_STRING];
            fgets(phone, MAX_STRING, stdin);
            if (strcmp(phone, "\n") != 0 && isValidPhone(phone)) {
                phone[strcspn(phone, "\n")] = 0;
                strcpy(employees[i].phone, phone);
            }

            printf("Enter New Position (leave blank to keep current): ");
            char position[MAX_STRING];
            fgets(position, MAX_STRING, stdin);
            if (strcmp(position, "\n") != 0) {
                position[strcspn(position, "\n")] = 0;
                strcpy(employees[i].position, position);
            }

            printf("Enter New Email (leave blank to keep current): ");
            char email[MAX_STRING];
            fgets(email, MAX_STRING, stdin);
            if (strcmp(email, "\n") != 0 && isValidEmail(email)) {
                email[strcspn(email, "\n")] = 0;
                strcpy(employees[i].email, email);
            }

            saveEmployees();
            printf("Employee updated successfully!\n");
            return;
        }
    }

    printf("Employee not found!\n");
}

// Delete an employee
void deleteEmployee() {
    char id[MAX_STRING];
    printf("Enter Employee ID to delete: ");
    scanf("%s", id);

    int found = 0;
    for (int i = 0; i < employeeCount; i++) {
        if (strcmp(employees[i].id, id) == 0) {
            for (int j = i; j < employeeCount - 1; j++) {
                employees[j] = employees[j + 1];
            }
            employeeCount--;
            found = 1;
            break;
        }
    }

    if (found) {
        saveEmployees();
        printf("Employee deleted successfully!\n");
    } else {
        printf("Employee not found!\n");
    }
}

// Main menu
int main() {
    loadEmployees();

    while (1) {
        printf("\nEmployee Management System\n");
        printf("1. Add Employee\n");
        printf("2. List Employees\n");
        printf("3. View Employee\n");
        printf("4. Edit Employee\n");
        printf("5. Delete Employee\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addEmployee();
                break;
            case 2:
                listEmployees();
                break;
            case 3:
                viewEmployee();
                break;
            case 4:
                editEmployee();
                break;
            case 5:
                deleteEmployee();
                break;
            case 6:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}
