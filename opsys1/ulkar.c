#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 100
#define POEM_LENGTH 256
#define BUNNY_FILE "ulkar.dat"

typedef struct {
    char bunny_name[NAME_LENGTH];
    char bunny_poem[POEM_LENGTH];
    int collected_eggs;
} BunnyContestant;

void add_new_bunny();
void update_bunny_info();
void remove_bunny();
void show_all_bunnies();
void clear_my_screen();
int check_if_bunny_exists(const char* name);

int main() {
    int user_choice;
    do {
        clear_my_screen();
        printf("\n🐰 Welcome to Easter Bunny Contest 🥚\n");
        printf("1. Add new bunny\n");
        printf("2. Update bunny\n");
        printf("3. Remove bunny\n");
        printf("4. Show all\n");
        printf("0. Exit\n");
        printf("\nWhat to do");
        scanf("%d", &user_choice);
        getchar(); 

        switch (user_choice) {
            case 1:
                add_new_bunny();
                break;
            case 2:
                update_bunny_info();
                break;
            case 3:
                remove_bunny();
                break;
            case 4:
                show_all_bunnies();
                break;
            case 0:
                printf("\nHappy Easter! Bye 🐰\n");
                break;
            default:
                printf("try again");
                getchar();
        }
    } while (user_choice != 0);

    return 0;
}

void clear_my_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void add_new_bunny() {
    BunnyContestant new_contestant;
    FILE* my_file;

    clear_my_screen();
    printf("\n🐰 Add New Bunny🐰\n");
    
    printf("Enter bunny name: ");
    fgets(new_contestant.bunny_name, NAME_LENGTH, stdin);
    new_contestant.bunny_name[strcspn(new_contestant.bunny_name, "\n")] = 0;
    
    if (check_if_bunny_exists(new_contestant.bunny_name) != -1) {
        printf("\n bunny is already in! 🐰\n");
        printf("enter to go back...");
        getchar();
        return;
    }

    printf("Enter bunny poem: ");
    fgets(new_contestant.bunny_poem, POEM_LENGTH, stdin);
    new_contestant.bunny_poem[strcspn(new_contestant.bunny_poem, "\n")] = 0;
    
    new_contestant.collected_eggs = 0;

    my_file = fopen(BUNNY_FILE, "ab");
    if (my_file == NULL) {
        printf("Couldn't open the file! 😢\n");
        return;
    }

    fwrite(&new_contestant, sizeof(BunnyContestant), 1, my_file);
    fclose(my_file);

    printf("\n%s now in! 🎉\n", new_contestant.bunny_name);
    printf("Enter to continue");
    getchar();
}

void update_bunny_info() {
    char bunny_to_find[NAME_LENGTH];
    BunnyContestant current_bunny;
    FILE* my_file;
    int found = 0;
    long file_position;

    clear_my_screen();
    printf("\n🔄 Update Info 🔄\n");
    
    
    printf("Which bunny to update? ");
    fgets(bunny_to_find, NAME_LENGTH, stdin);
    bunny_to_find[strcspn(bunny_to_find, "\n")] = 0;

    my_file = fopen(BUNNY_FILE, "rb+");
    if (my_file == NULL) {
        printf("Couldn't open the file! 😢\n");
        return;
    }

    while (fread(&current_bunny, sizeof(BunnyContestant), 1, my_file) == 1) {
        if (strcmp(current_bunny.bunny_name, bunny_to_find) == 0) {
            printf("\nCurrent info  %s:\n", current_bunny.bunny_name);
            printf("Poem: %s\n", current_bunny.bunny_poem);
            printf("Collected eggs: %d\n\n", current_bunny.collected_eggs);

            printf("Enter new poem (or press Enter to keep the current one): ");
            char new_poem[POEM_LENGTH];
            fgets(new_poem, POEM_LENGTH, stdin);
            new_poem[strcspn(new_poem, "\n")] = 0;
            
            if (strlen(new_poem) > 0) {
                strcpy(current_bunny.bunny_poem, new_poem);
            }

            printf("How many eggs has %s collected?: ", current_bunny.bunny_name);
            int egg_count;
            scanf("%d", &egg_count);
            getchar();
            
            if (egg_count >= 0) {
                current_bunny.collected_eggs = egg_count;
            }

            file_position = ftell(my_file) - sizeof(BunnyContestant);
            fseek(my_file, file_position, SEEK_SET);
            fwrite(&current_bunny, sizeof(BunnyContestant), 1, my_file);
            found = 1;
            break;
        }
    }

    fclose(my_file);

    if (!found) {
        printf("\n couldn't find the bunny in the contest! 🤔\n");
    } else {
        printf("\ninfo updated! 🎉\n");
    }

    printf("enter to continue...");
    getchar();
}

void remove_bunny() {
    char bunny_to_remove[NAME_LENGTH];
    BunnyContestant current_bunny;
    FILE *my_file, *temp_file;
    int found = 0;

    clear_my_screen();
    printf("\n❌ Remove Bunny ❌\n");
    
    
    printf("Which bunny to remove? ");
    fgets(bunny_to_remove, NAME_LENGTH, stdin);
    bunny_to_remove[strcspn(bunny_to_remove, "\n")] = 0;

    my_file = fopen(BUNNY_FILE, "rb");
    if (my_file == NULL) {
        printf("Couldn't open the file! 😢\n");
        return;
    }

    temp_file = fopen("wq1c71_temp.dat", "wb");
    if (temp_file == NULL) {
        printf("Something went wrong!\n");
        fclose(my_file);
        return;
    }

    while (fread(&current_bunny, sizeof(BunnyContestant), 1, my_file) == 1) {
        if (strcmp(current_bunny.bunny_name, bunny_to_remove) == 0) {
            found = 1;
            continue;
        }
        fwrite(&current_bunny, sizeof(BunnyContestant), 1, temp_file);
    }

    fclose(my_file);
    fclose(temp_file);

    remove(BUNNY_FILE);
    rename("wq1c71_temp.dat", BUNNY_FILE);

    if (!found) {
        printf("\ncouldn't find the bunny in\n");
    } else {
        printf("\n %s has been removed 👋\n", bunny_to_remove);
    }

    printf("Enter to continue...");
    getchar();
}

void show_all_bunnies() {
    BunnyContestant current_bunny;
    FILE* my_file;
    int bunny_count = 0;

    clear_my_screen();
    printf("\n📋 Current Contestants 📋\n");
   

    my_file = fopen(BUNNY_FILE, "rb");
    if (my_file == NULL) {
        printf("No contestants in yet! 🐰\n");
        printf("Enter to continue...");
        getchar();
        return;
    }

    printf("%-25s %-45s %s\n", "Bunny Name", "Watering Poem", "Eggs");
 
    while (fread(&current_bunny, sizeof(BunnyContestant), 1, my_file) == 1) {
        printf("%-25s %-45s %d\n", 
               current_bunny.bunny_name, 
               current_bunny.bunny_poem, 
               current_bunny.collected_eggs);
        bunny_count++;
    }

    fclose(my_file);

    if (bunny_count == 0) {
        printf("No contestants have joined  yet! 🐰\n");
    } else {
        printf("\nTotal contestants: %d 🐰\n", bunny_count);
    }

    printf("\nEnter to continue...");
    getchar();
}

int check_if_bunny_exists(const char* name) {
    BunnyContestant current_bunny;
    FILE* my_file;
    int position = -1;
    int count = 0;

    my_file = fopen(BUNNY_FILE, "rb");
    if (my_file == NULL) {
        return -1;
    }

    while (fread(&current_bunny, sizeof(BunnyContestant), 1, my_file) == 1) {
        if (strcmp(current_bunny.bunny_name, name) == 0) {
            position = count;
            break;
        }
        count++;
    }

    fclose(my_file);
    return position;
}

