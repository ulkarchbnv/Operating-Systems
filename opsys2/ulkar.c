#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>

#define NAME_LENGTH 100
#define POEM_LENGTH 256
#define BUNNY_FILE "ulkar.dat"
#define MAX_BUNNIES 100

typedef struct {
    char bunny_name[NAME_LENGTH];
    char bunny_poem[POEM_LENGTH];
    int collected_eggs;
} BunnyContestant;

typedef struct {
    int index;
    int eggs;
} BunnyResult;

void add_new_bunny();
void update_bunny_info();
void remove_bunny();
void show_all_bunnies();
void clear_my_screen();
int check_if_bunny_exists(const char* name);
void start_competition();
void handle_signal(int sig);

int signals_received = 0;

int main() {
    int user_choice;
    do {
        clear_my_screen();
        printf("\n🐰 Welcome to Easter Bunny Contest 🥚\n");
        printf("1. Add new bunny\n");
        printf("2. Update bunny\n");
        printf("3. Remove bunny\n");
        printf("4. Show all\n");
        printf("5. Start the competition\n");
        printf("0. Exit\n");
        printf("\nWhat to do: ");
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
            case 5:
                start_competition();
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
        printf("\nBunny is already in! 🐰\n");
        printf("Enter to go back...");
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
            printf("\nCurrent info for %s:\n", current_bunny.bunny_name);
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
        printf("\nCouldn't find the bunny in the contest! 🤔\n");
    } else {
        printf("\nInfo updated! 🎉\n");
    }

    printf("Enter to continue...");
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
        printf("\nCouldn't find the bunny in\n");
    } else {
        printf("\n%s has been removed 👋\n", bunny_to_remove);
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
        printf("No contestants have joined yet! 🐰\n");
    } else {
        printf("\nTotal contestants: %d 🐰\n", bunny_count);
    }

    printf("\nenter to continue...");
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

void handle_signal(int sig) {
    if (sig == SIGUSR1) {
        signals_received++;
    }
}

void start_competition() {
    FILE* my_file = fopen(BUNNY_FILE, "rb");
    if (!my_file) {
        printf("No bunnies yet 🐇\nenter to continue");
        getchar();
        return;
    }

    signal(SIGUSR1, handle_signal);
    srand(time(NULL));

    BunnyContestant bunnies[MAX_BUNNIES];
    int count = 0;
    while (count < MAX_BUNNIES && fread(&bunnies[count], sizeof(BunnyContestant), 1, my_file) == 1) {
        count++;
    }
    fclose(my_file);

    if (count == 0) {
        printf("No bunnies registered yet!\n");
        printf("enter to continue");
        getchar();
        return;
    }

    int result_pipe[2];
    int sync_pipe[2];
    
    if (pipe(result_pipe) == -1 || pipe(sync_pipe) == -1) {
        perror("failed");
        return;
    }

    signals_received = 0;
    
    clear_my_screen();
    printf("\n🐰 STARTING THE COMPETITION! 🥚\n");
    printf("Chief Bunny is sending all %d bunnies to water!\n\n", count);

    pid_t bunny_pids[MAX_BUNNIES];
    BunnyResult results[MAX_BUNNIES];
    
    for (int i = 0; i < count; i++) {
        bunny_pids[i] = fork();
        
        if (bunny_pids[i] < 0) {
            perror("failed");
            exit(1);
        }
        
        if (bunny_pids[i] == 0) { 
            close(result_pipe[0]); 
            close(sync_pipe[1]);   
            
            srand(time(NULL) ^ (getpid() << 16)); 
            
            char signal_byte;
            read(sync_pipe[0], &signal_byte, 1);
            close(sync_pipe[0]);
            
            kill(getppid(), SIGUSR1);
            
            usleep(100000);
            
            int eggs = (rand() % 20) + 1;
            
            BunnyResult result;
            result.index = i;
            result.eggs = eggs;
            
            write(result_pipe[1], &result, sizeof(BunnyResult));
            close(result_pipe[1]);
            
            exit(0); 
        }
    }
    
    close(result_pipe[1]); 
    close(sync_pipe[0]);  
    
    
    for (int i = 0; i < count; i++) {
        char signal_byte = 1;
        write(sync_pipe[1], &signal_byte, 1); 
        
        int old_signals = signals_received;
        while (signals_received == old_signals) {
            usleep(10000); 
        }
        
        printf("🐰 Bunny '%s' is watering...\n", bunnies[i].bunny_name);
        printf("🐰 Chief Bunny: Received watering signal from a bunny! (%d/%d)\n", 
               signals_received, count);
        
        printf("📜 Poem: \"%s\"\n", bunnies[i].bunny_poem);
        
        BunnyResult result;
        if (read(result_pipe[0], &result, sizeof(BunnyResult)) == sizeof(BunnyResult)) {
            
            results[i] = result;
            bunnies[result.index].collected_eggs = result.eggs;
            
            printf("🥚 Bunny '%s' received %d red eggs!\n\n", 
                   bunnies[result.index].bunny_name, result.eggs);
        }
        
        usleep(500000);
    }
    
    close(sync_pipe[1]);
    close(result_pipe[0]);
    
    for (int i = 0; i < count; i++) {
        waitpid(bunny_pids[i], NULL, 0);
    }
    
    printf("👑 Chief Bunny: Everyone finished watering! Signals received: %d\n, Congratulations!", signals_received);
    
    my_file = fopen(BUNNY_FILE, "wb");
    if (my_file) {
        for (int i = 0; i < count; i++) {
            fwrite(&bunnies[i], sizeof(BunnyContestant), 1, my_file);
        }
        fclose(my_file);
    }
    
    int winner_index = 0;
    for (int i = 1; i < count; i++) {
        if (bunnies[i].collected_eggs > bunnies[winner_index].collected_eggs) {
            winner_index = i;
        }
    }
    
    printf("\n👑 The Easter Bunny King is %s with %d eggs! 👑\n",
           bunnies[winner_index].bunny_name,
           bunnies[winner_index].collected_eggs);
    
    printf("\nEnter to continue...");
    getchar();
}


