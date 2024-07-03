#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 300
#define PROC_FILE "/proc/secrets"

void write_secret(const char *secret) {
    int file = open(PROC_FILE, O_WRONLY);
    if (file < 0) {
        perror("Failed to open /proc/secrets for writing");
        return;
    }

    if (write(file, secret, strlen(secret)) < 0) {
        perror("Failed to write to /proc/secrets");
    } else {
        printf("Secret written successfully\n");
    }

    close(file);
}

void read_secret(int id) {
    int file = open(PROC_FILE, O_RDWR);
    char buffer[BUFFER_SIZE];
    char id_str[BUFFER_SIZE];

    if (file < 0) {
        perror("Failed to open /proc/secrets for reading");
        return;
    }

    snprintf(id_str, BUFFER_SIZE, "%d", id);

    if (write(file, id_str, strlen(id_str)) < 0) {
        perror("Failed to write to /proc/secrets for read");
    } else {
        int len = read(file, buffer, BUFFER_SIZE);
        if (len < 0) {
            perror("Failed to read from /proc/secrets");
        }
        else {
            buffer[len] = '\0';
            printf("Read from /proc/secrets:\n%s\n", buffer);
        }
    }

    close(file);
}

void delete_secret(int id) {
    int file = open(PROC_FILE, O_WRONLY);
    char id_str[BUFFER_SIZE];

    if (file < 0) {
        perror("Failed to open /proc/secrets for deletion");
        return;
    }

    snprintf(id_str, BUFFER_SIZE, "delete:%d", id);

    if (write(file, id_str, strlen(id_str)) < 0) {
        perror("Failed to write to /proc/secrets for delete");
    } else {
        printf("Secret with id %d deleted successfully\n", id);
    }

    close(file);

}

int main() {
    char secret[BUFFER_SIZE];
    int choice, id;
    while (1) {
        printf("Please choose the action you want to perform.\nTo write a secret press: 1.\n"
               "To read a secret press: 2.\nTo delete a secret press: 3.\nTo exit press: 4.\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the secret you want to save: ");
                fgets(secret, BUFFER_SIZE, stdin);
                secret[strcspn(secret, "\n")] = 0; // Removing \n character
                write_secret(secret);
                break;
            case 2:
                printf("Enter the id of the secret you want to read: ");
                scanf("%d", &id);
                read_secret(id);
                break;
            case 3:
                printf("Enter the id of the secret you want to delete: ");
                scanf("%d", &id);
                delete_secret(id);
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid option. Please try again.\n");
        }
    }
    return 0;
}