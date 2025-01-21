#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <string.h>
#include <unistd.h>

#define OTP_VALIDITY 30 
#define MAX_ATTEMPTS 3  
#define MAX_USERS 20   
#define DATA_FILE "users_data.txt" 
int figlet()
{
    printf("___________________________________________________________________________________________________________\n");
    printf(" /-------------------------------------------------------------------------------------------------------\\\n");
    printf("| |                                                                                                     | |\n");
    printf("|   |                                                                                                 |   |\n");
    printf("|       |-     <<<< WELCOME TO THE PROTOTYPE MODEL OF TWO FACTOR AUTHENTICATION SYSTEM >>>>      -|       |\n");
    printf("|   |                                                                                                 |   |\n");
    printf("| |                                                                                                     | |\n");
    printf("|---------------------------------------------------------------------------------------------------------|\n");
    printf(" \\______________________________________________________________________________________________________//\n");
}

typedef struct
{
    char user_name[50];
    char password[50]; 
} User;

User users[MAX_USERS];
int user_count = 0;

int generate_otp()
{
    srand(time(NULL));
    return rand() % 900000 + 100000;
}

int is_registered(const char *user_name)
{
    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(users[i].user_name, user_name) == 0)
        {
            return i;
        }
    }
    return -1;
}

void load_users()
{
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL)
    {
        return;
    }

    user_count = 0;
    while (fscanf(file, "%s %s", users[user_count].user_name, users[user_count].password) == 2)
    {
        user_count++;
        if (user_count >= MAX_USERS) break;
    }
    fclose(file);
}


void save_users()
{
    FILE *file = fopen(DATA_FILE, "w");
    if (file == NULL)
    {
        printf("__________________________________________\n");
        printf("\n");
        printf("         Error saving user data.          \n");
        printf("\n");
        printf("__________________________________________\n");
    }

    for (int i = 0; i < user_count; i++)
    {
        fprintf(file, "%s %s\n", users[i].user_name, users[i].password);
    }
    fclose(file);
}

void register_user()
{
    if (user_count >= MAX_USERS)
    {
        printf("___________________________________________________\n");
        printf("\n");
        printf("  User limit reached. Cannot register new users.   \n");
        printf("___________________________________________________\n");
        printf("\n");
    }
    printf("__________________________________________\n");
    printf("\n");
    printf("           Register a new user            \n");
    printf("\n");
    printf("__________________________________________\n");
    printf("\n   Enter username: ");
    scanf("%s", users[user_count].user_name);
    printf("\n   Enter Password: ");
    scanf("%s", users[user_count].password);
    printf("__________________________________________\n");
    printf("\n");
    printf("      User registered successfully!       \n");
    printf("\n");
    printf("__________________________________________\n");
    user_count++;

    save_users();
}


int main()
{
    figlet();
    char input_user_name[50], input_password[50];
    int otp, entered_otp, attempts = 0;
    time_t otp_generated_time;

    load_users();

    printf("\nWelcome! Are you a registered user? (yes/no): ");
    char choice[10];
    scanf("%s", choice);

    if (strcmp(choice, "no") == 0)
    {
        register_user();
        return 0; 
    }
    
    if (strcmp(choice, "yes") != 0) 
    {
        printf("__________________________________________\n");
        printf("\n");
        printf("       Invalid choice. Exiting...         \n");
        printf("__________________________________________\n");
        printf("\n");
        return 1;
    }
    printf("__________________________________________\n");
    printf("Enter your user_name:");
    scanf("%s", input_user_name);

    int user_index = is_registered(input_user_name);

    if (user_index == -1)
    {
        printf("_________________________________________________________\n");
        printf("\n");
        printf("  User not found. Please register first and Try again..  \n");
        printf("_________________________________________________________\n");
        printf("\n");
        return 1;
    }

    printf("Enter your Password: ");
    scanf("%s", input_password, "\n");
    printf("__________________________________________\n");

    if (strcmp(users[user_index].password, input_password) != 0)
    {
        printf("__________________________________________\n");
        printf("\n");
        printf("       Invalid password. Exiting...       \n");
        printf("__________________________________________\n");
        printf("\n");
        return 1;
    }
   
    otp = generate_otp();
    otp_generated_time = time(NULL);

    printf("Your one-time password (OTP) is: %d\n", otp);
    printf("__________________________________________\n");

    while (attempts < MAX_ATTEMPTS)
    {
        printf("__________________________________________\n");
        printf("    Enter OTP:");
        scanf("%d", &entered_otp);
        printf("__________________________________________\n");
        
        if (difftime(time(NULL), otp_generated_time) > OTP_VALIDITY)
        {
            printf("OTP expired. Please request a new one.\n");
            return 1;
        }

        if (entered_otp == otp)
        {
            printf("Authentication successful!\n");
            printf("Login successful!\n");
            printf("__________________________________________\n");
            return 0;
        }
        else 
        {
            printf("__________________________________________\n");
            printf("\n");
            printf("        Incorrect OTP. Try again.         \n");
            printf("__________________________________________\n");
            printf("\n");
            attempts++;
        }
    }

    printf("__________________________________________\n");
    printf("\n");
    printf("   Maximum attempts reached. Exiting...   \n");
    printf("__________________________________________\n");
    printf("\n");
    return 1;
}
