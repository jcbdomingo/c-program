/* ============================================================
   PERSONAL HABIT TRACKER AND MOTIVATION ASSISTANT
   Language  : C
   Data Structure: Array of Structures
   Author    : Habit Tracker Program
   Description: A command-line habit tracking application
                that helps users build and maintain good habits.
   ============================================================ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* ============================================================
   ANSI COLOR CODES FOR TERMINAL STYLING
   ============================================================ */
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"
#define BG_BLUE     "\033[44m"
#define BG_GREEN    "\033[42m"
#define BG_YELLOW   "\033[43m"

/* ============================================================
   CONSTANTS
   ============================================================ */
#define MAX_HABITS  100
#define NAME_LEN    100

/* ============================================================
   STRUCT DEFINITION
   The core data structure of this program.
   Each habit is stored as a Habit struct.
   ============================================================ */
struct Habit {
    char name[NAME_LEN];   /* Name/description of the habit      */
    int  completed;        /* 0 = pending, 1 = completed          */
    int  archived;         /* 0 = active,  1 = archived           */
    int  streak;           /* Streak counter (for future use)     */
};

/* ============================================================
   GLOBAL ARRAY OF STRUCTURES
   All habits are stored here. Max 100 habits.
   ============================================================ */
struct Habit habits[MAX_HABITS];
int habitCount = 0;  /* Tracks how many habits have been added */

/* ============================================================
   MOTIVATIONAL QUOTES ARRAY
   10+ quotes displayed randomly at startup.
   ============================================================ */
const char *quotes[] = {
    "Small daily improvements are the key to long-term success.",
    "Motivation gets you started. Habit keeps you going. — Jim Ryun",
    "You don't rise to the level of your goals, you fall to the level of your systems.",
    "The secret of your future is hidden in your daily routine.",
    "Discipline is choosing between what you want now and what you want most.",
    "Success is the sum of small efforts, repeated day in and day out.",
    "Your habits today determine your success tomorrow.",
    "We are what we repeatedly do. Excellence is not an act, but a habit. — Aristotle",
    "The chains of habit are too light to be felt until they are too heavy to be broken.",
    "Every action you take is a vote for the type of person you wish to become.",
    "It does not matter how slowly you go, as long as you do not stop. — Confucius",
    "Consistency is the hallmark of the undefeated.",
    "Make each day your masterpiece. — John Wooden"
};
#define QUOTE_COUNT (int)(sizeof(quotes) / sizeof(quotes[0]))

/* ============================================================
   FUNCTION PROTOTYPES
   ============================================================ */
void loadingScreen();
void showMotivationalQuote();
void displayMenu();
void addHabit();
void viewHabits();
void completeHabit();
void archiveHabit();
void viewArchivedHabits();
void showStatistics();
void printSeparator(char c, int len);
void clearInputBuffer();
void pressEnterToContinue();

/* ============================================================
   MAIN FUNCTION
   Entry point: Shows loading screen, quote, then main loop.
   ============================================================ */
int main() { 
    system("chcp 65001 > nul"); 
    int choice;

    /* Seed random number generator for quotes */
    srand((unsigned int)time(NULL));

    /* Show animated loading screen */
    loadingScreen();

    /* Show a random motivational quote */
    showMotivationalQuote();

    /* Main program loop */
    while (1) {
        displayMenu();

        printf(CYAN BOLD "  Enter your choice: " RESET);
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf(RED "\n  [!] Invalid input. Please enter a number.\n" RESET);
            pressEnterToContinue();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                addHabit();
                break;
            case 2:
                viewHabits();
                break;
            case 3:
                completeHabit();
                break;
            case 4:
                archiveHabit();
                break;
            case 5:
                viewArchivedHabits();
                break;
            case 6:
                /* Exit with farewell quote */
                printf("\n");
                printSeparator('=', 60);
                printf(MAGENTA BOLD
                    "  \"The journey of a thousand miles begins with one step.\"\n"
                    "                                         — Lao Tzu\n"
                    RESET);
                printSeparator('=', 60);
                printf(GREEN BOLD
                    "\n  Thank you for using Habit Tracker! Keep going! 💪\n\n"
                    RESET);
                exit(0);
            default:
                printf(RED "\n  [!] Invalid choice. Please select 1–6.\n" RESET);
                pressEnterToContinue();
        }
    }

    return 0;
}

/* ============================================================
   LOADING SCREEN
   Displays an animated startup banner.
   ============================================================ */
void loadingScreen() {
    int i;
    printf("\033[2J\033[H"); /* Clear screen */

    printf("\n\n");
    printf(CYAN BOLD);
    printf("  ██╗  ██╗ █████╗ ██████╗ ██╗████████╗    ████████╗██████╗  █████╗  ██████╗██╗  ██╗███████╗██████╗ \n");
    printf("  ██║  ██║██╔══██╗██╔══██╗██║╚══██╔══╝    ╚══██╔══╝██╔══██╗██╔══██╗██╔════╝██║ ██╔╝██╔════╝██╔══██╗\n");
    printf("  ███████║███████║██████╔╝██║   ██║          ██║   ██████╔╝███████║██║     █████╔╝ █████╗  ██████╔╝\n");
    printf("  ██╔══██║██╔══██║██╔══██╗██║   ██║          ██║   ██╔══██╗██╔══██║██║     ██╔═██╗ ██╔══╝  ██╔══██╗\n");
    printf("  ██║  ██║██║  ██║██████╔╝██║   ██║          ██║   ██║  ██║██║  ██║╚██████╗██║  ██╗███████╗██║  ██║\n");
    printf("  ╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝ ╚═╝   ╚═╝          ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝\n");
    printf(RESET);

    printf(YELLOW BOLD "\n            Personal Habit Tracker & Motivation Assistant\n" RESET);
    printf(WHITE   "                  Build Better Habits. One Day at a Time.\n\n" RESET);

    /* Loading animation */
    printf(GREEN "  Loading ");
    fflush(stdout);
    for (i = 0; i < 20; i++) {
        printf("█");
        fflush(stdout);

        /* Simulate delay with a busy loop (portable) */
        volatile long j;
        for (j = 0; j < 8000000L; j++);
    }
    printf(" Done!\n\n" RESET);

    /* Brief pause */
    volatile long k;
    for (k = 0; k < 60000000L; k++);
}

/* ============================================================
   SHOW MOTIVATIONAL QUOTE
   Picks a random quote and displays it in a styled box.
   ============================================================ */
void showMotivationalQuote() {
    int index = rand() % QUOTE_COUNT;

    printf("\033[2J\033[H"); /* Clear screen */
    printf("\n");
    printSeparator('*', 60);
    printf(YELLOW BOLD "    QUOTE OF THE SESSION\n" RESET);
    printSeparator('*', 60);
    printf(CYAN "\n  \"%s\"\n\n" RESET, quotes[index]);
    printSeparator('*', 60);
    printf("\n");
    pressEnterToContinue();
}

/* ============================================================
   DISPLAY MENU
   Shows the main navigation menu.
   ============================================================ */
void displayMenu() {
    printf("\033[2J\033[H"); /* Clear screen each time */
    printf("\n");
    printSeparator('=', 60);
    printf(BLUE BOLD "         PERSONAL HABIT TRACKER — MAIN MENU\n" RESET);
    printSeparator('=', 60);
    printf(GREEN  "  [1]" RESET WHITE "  Add New Habit\n"           RESET);
    printf(CYAN   "  [2]" RESET WHITE "  View All Habits\n"         RESET);
    printf(YELLOW "  [3]" RESET WHITE "  Mark Habit as Completed\n" RESET);
    printf(MAGENTA"  [4]" RESET WHITE "  Archive Habit\n"           RESET);
    printf(BLUE   "  [5]" RESET WHITE "  View Archived Habits\n"    RESET);
    printf(RED    "  [6]" RESET WHITE "  Exit Program\n"            RESET);
    printSeparator('=', 60);
    printf("\n");
}

/* ============================================================
   ADD NEW HABIT
   Prompts user for a habit name, validates it, checks for
   duplicates, and saves it to the habits array.
   ============================================================ */
void addHabit() {
    char newName[NAME_LEN];
    int i;

    printf("\n");
    printSeparator('-', 60);
    printf(GREEN BOLD "    ADD NEW HABIT\n" RESET);
    printSeparator('-', 60);

    /* Check if max capacity is reached */
    if (habitCount >= MAX_HABITS) {
        printf(RED "\n  [!] Maximum habit limit (%d) reached!\n" RESET, MAX_HABITS);
        pressEnterToContinue();
        return;
    }

    printf(WHITE "  Enter habit name: " RESET);
    fgets(newName, NAME_LEN, stdin);

    /* Strip trailing newline */
    newName[strcspn(newName, "\n")] = '\0';

    /* Validate: empty input */
    if (strlen(newName) == 0) {
        printf(RED "\n  [!] Habit name cannot be empty!\n" RESET);
        pressEnterToContinue();
        return;
    }

    /* Check for duplicate (case-insensitive) */
    for (i = 0; i < habitCount; i++) {
        /* Simple case check: compare lowercased versions */
        char existing[NAME_LEN], input[NAME_LEN];
        int k;
        strcpy(existing, habits[i].name);
        strcpy(input, newName);
        for (k = 0; existing[k]; k++) {
            if (existing[k] >= 'A' && existing[k] <= 'Z')
                existing[k] += 32;
        }
        for (k = 0; input[k]; k++) {
            if (input[k] >= 'A' && input[k] <= 'Z')
                input[k] += 32;
        }
        if (strcmp(existing, input) == 0) {
            printf(YELLOW "\n  [!] Duplicate detected! \"%s\" already exists.\n" RESET, habits[i].name);
            pressEnterToContinue();
            return;
        }
    }

    /* Save to array with default values */
    strcpy(habits[habitCount].name, newName);
    habits[habitCount].completed = 0;
    habits[habitCount].archived  = 0;
    habits[habitCount].streak    = 0;
    habitCount++;

    printf(GREEN "\n    Habit \"%s\" added successfully!\n" RESET, newName);
    printf(WHITE "  Total habits tracked: %d / %d\n" RESET, habitCount, MAX_HABITS);
    pressEnterToContinue();
}

/* ============================================================
   VIEW ALL HABITS
   Displays all active (non-archived) habits in a table.
   ============================================================ */
void viewHabits() {
    int i, displayNum = 0;

    printf("\n");
    printSeparator('=', 60);
    printf(CYAN BOLD "   ACTIVE HABITS\n" RESET);
    printSeparator('=', 60);

    /* Table header */
    printf(BOLD "  %-5s %-35s %-12s\n" RESET, "No.", "Habit Name", "Status");
    printSeparator('-', 60);

    /* Loop through habits array and display only active ones */
    for (i = 0; i < habitCount; i++) {
        if (habits[i].archived == 0) {
            displayNum++;
            const char *statusColor = habits[i].completed ? GREEN : YELLOW;
            const char *statusLabel = habits[i].completed ? "✔ Completed" : "○ Pending";

            printf("  %-5d %-35s %s%-12s%s\n",
                displayNum,
                habits[i].name,
                statusColor, statusLabel, RESET);
        }
    }

    /* If no active habits */
    if (displayNum == 0) {
        printf(YELLOW "  No active habits found. Add some habits first!\n" RESET);
    }

    printSeparator('=', 60);
    printf(WHITE "  Total active habits: %d\n" RESET, displayNum);

    pressEnterToContinue();
}

/* ============================================================
   COMPLETE HABIT
   Lets the user mark an active habit as completed, then
   displays updated statistics.
   ============================================================ */
void completeHabit() {
    int i, displayNum = 0, choice;
    int indexMap[MAX_HABITS]; /* Maps display number -> habits[] index */

    printf("\n");
    printSeparator('=', 60);
    printf(YELLOW BOLD "    MARK HABIT AS COMPLETED\n" RESET);
    printSeparator('=', 60);

    /* Show active habits */
    printf(BOLD "  %-5s %-35s %-12s\n" RESET, "No.", "Habit Name", "Status");
    printSeparator('-', 60);

    for (i = 0; i < habitCount; i++) {
        if (habits[i].archived == 0) {
            displayNum++;
            indexMap[displayNum] = i;
            const char *statusColor = habits[i].completed ? GREEN : YELLOW;
            const char *statusLabel = habits[i].completed ? " Completed" : "○ Pending";
            printf("  %-5d %-35s %s%-12s%s\n",
                displayNum, habits[i].name,
                statusColor, statusLabel, RESET);
        }
    }

    if (displayNum == 0) {
        printf(YELLOW "\n  No active habits to complete!\n" RESET);
        pressEnterToContinue();
        return;
    }

    printSeparator('-', 60);
    printf(WHITE "  Enter habit number to mark complete (0 to cancel): " RESET);

    if (scanf("%d", &choice) != 1) {
        clearInputBuffer();
        printf(RED "\n  [!] Invalid input.\n" RESET);
        pressEnterToContinue();
        return;
    }
    clearInputBuffer();

    /* Cancel */
    if (choice == 0) {
        printf(YELLOW "  Operation cancelled.\n" RESET);
        pressEnterToContinue();
        return;
    }

    /* Validate range */
    if (choice < 1 || choice > displayNum) {
        printf(RED "\n  [!] Invalid habit number!\n" RESET);
        pressEnterToContinue();
        return;
    }

    int realIndex = indexMap[choice];

    /* Already completed? */
    if (habits[realIndex].completed == 1) {
        printf(YELLOW "\n  [!] \"%s\" is already marked as completed!\n" RESET,
               habits[realIndex].name);
        pressEnterToContinue();
        return;
    }

    /* Mark as completed */
    habits[realIndex].completed = 1;
    habits[realIndex].streak++;

    printf(GREEN "\n    \"%s\" marked as COMPLETED! Great job!\n" RESET,
           habits[realIndex].name);

    /* Show statistics after completing */
    showStatistics();
    pressEnterToContinue();
}

/* ============================================================
   ARCHIVE HABIT
   Moves a habit to the archive (archived = 1).
   Asks for confirmation before archiving.
   ============================================================ */
void archiveHabit() {
    int i, displayNum = 0, choice;
    int indexMap[MAX_HABITS];
    char confirm;

    printf("\n");
    printSeparator('=', 60);
    printf(MAGENTA BOLD "    ARCHIVE HABIT\n" RESET);
    printSeparator('=', 60);

    printf(BOLD "  %-5s %-35s %-12s\n" RESET, "No.", "Habit Name", "Status");
    printSeparator('-', 60);

    for (i = 0; i < habitCount; i++) {
        if (habits[i].archived == 0) {
            displayNum++;
            indexMap[displayNum] = i;
            const char *statusColor = habits[i].completed ? GREEN : YELLOW;
            const char *statusLabel = habits[i].completed ? " Completed" : " Pending";
            printf("  %-5d %-35s %s%-12s%s\n",
                displayNum, habits[i].name,
                statusColor, statusLabel, RESET);
        }
    }

    if (displayNum == 0) {
        printf(YELLOW "\n  No active habits to archive!\n" RESET);
        pressEnterToContinue();
        return;
    }

    printSeparator('-', 60);
    printf(WHITE "  Enter habit number to archive (0 to cancel): " RESET);

    if (scanf("%d", &choice) != 1) {
        clearInputBuffer();
        printf(RED "\n  [!] Invalid input.\n" RESET);
        pressEnterToContinue();
        return;
    }
    clearInputBuffer();

    if (choice == 0) {
        printf(YELLOW "  Operation cancelled.\n" RESET);
        pressEnterToContinue();
        return;
    }

    if (choice < 1 || choice > displayNum) {
        printf(RED "\n  [!] Invalid habit number!\n" RESET);
        pressEnterToContinue();
        return;
    }

    int realIndex = indexMap[choice];

    /* Confirmation prompt */
    printf(YELLOW "\n  Are you sure you want to archive \"%s\"? (y/n): " RESET,
           habits[realIndex].name);
    scanf(" %c", &confirm);
    clearInputBuffer();

    if (confirm == 'y' || confirm == 'Y') {
        habits[realIndex].archived = 1;
        printf(GREEN "\n    \"%s\" has been archived successfully!\n" RESET,
               habits[realIndex].name);
    } else {
        printf(YELLOW "  Archiving cancelled.\n" RESET);
    }

    pressEnterToContinue();
}

/* ============================================================
   VIEW ARCHIVED HABITS
   Displays all habits that have been archived.
   ============================================================ */
void viewArchivedHabits() {
    int i, displayNum = 0;

    printf("\n");
    printSeparator('=', 60);
    printf(BLUE BOLD "    ARCHIVED HABITS\n" RESET);
    printSeparator('=', 60);

    printf(BOLD "  %-5s %-35s %-12s\n" RESET, "No.", "Habit Name", "Status");
    printSeparator('-', 60);

    for (i = 0; i < habitCount; i++) {
        if (habits[i].archived == 1) {
            displayNum++;
            const char *statusColor = habits[i].completed ? GREEN : YELLOW;
            const char *statusLabel = habits[i].completed ? "✔ Completed" : "○ Pending";
            printf("  %-5d %-35s %s%-12s%s\n",
                displayNum, habits[i].name,
                statusColor, statusLabel, RESET);
        }
    }

    if (displayNum == 0) {
        printf(YELLOW "  No archived habits yet.\n" RESET);
    }

    printSeparator('=', 60);
    printf(WHITE "  Total archived habits: %d\n" RESET, displayNum);

    pressEnterToContinue();
}

/* ============================================================
   SHOW STATISTICS
   Displays completion stats for all active habits.
   Called automatically after completing a habit.
   ============================================================ */
void showStatistics() {
    int i, total = 0, completed = 0, remaining = 0;
    float percentage = 0.0f;

    for (i = 0; i < habitCount; i++) {
        if (habits[i].archived == 0) {
            total++;
            if (habits[i].completed == 1) completed++;
        }
    }
    remaining = total - completed;
    if (total > 0) percentage = ((float)completed / total) * 100.0f;

    printf("\n");
    printSeparator('-', 60);
    printf(CYAN BOLD "    HABIT STATISTICS\n" RESET);
    printSeparator('-', 60);
    printf(WHITE  "  Total Active Habits : %d\n" RESET, total);
    printf(GREEN  "  Completed           : %d\n" RESET, completed);
    printf(YELLOW "  Remaining           : %d\n" RESET, remaining);

    /* Progress bar */
    printf(CYAN "  Completion          : [");
    int bars = (int)(percentage / 5); /* 20-char bar */
    int j;
    for (j = 0; j < 20; j++) {
        if (j < bars) printf(GREEN "█" RESET CYAN);
        else printf("░");
    }
    printf(CYAN "] %.1f%%\n" RESET, percentage);
    printSeparator('-', 60);
}

/* ============================================================
   UTILITY: Print a separator line of given character and length
   ============================================================ */
void printSeparator(char c, int len) {
    int i;
    printf("  ");
    for (i = 0; i < len; i++) printf("%c", c);
    printf("\n");
}

/* ============================================================
   UTILITY: Clear leftover characters from input buffer
   ============================================================ */
void clearInputBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

/* ============================================================
   UTILITY: Wait for user to press Enter before continuing
   ============================================================ */
void pressEnterToContinue() {
    printf(WHITE "\n  Press " CYAN "[Enter]" WHITE " to continue..." RESET);
    getchar();
}