// ...existing code...

/* Confirmation helper: returns 1 for yes, 0 for no */
int confirmAction(const char *prompt) {
    char buf[8];
    while (1) {
        printf("%s (y/n): ", prompt);
        if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;
        if (buf[0] == 'y' || buf[0] == 'Y') return 1;
        if (buf[0] == 'n' || buf[0] == 'N') return 0;
        printf("Please answer y or n.\n");
    }
}

/* ── Mark Habit as Completed (improved UX) ───────────────── */
void completeHabit() {
    int i, displayNum = 0, choice;
    int indexMap[MAX_HABITS];

    printf("\n");
    printf("====== MARK HABIT AS COMPLETED ======\n");
    printf("\n");

    /* build and display list of active habits with numbers */
    for (i = 0; i < habitCount; i++) {
        if (habits[i].archived == 0) {
            displayNum++;
            indexMap[displayNum] = i;
            printf("[%d] %s  %s\n", displayNum, habits[i].name,
                   habits[i].completed ? "(Completed)" : "(Not Done)");
        }
    }

    if (displayNum == 0) {
        printf("No active habits to complete.\n");
        printf("\n===================================================\n");
        return;
    }

    while (1) {
        printf("Enter Habit Number to mark complete: ");
        if (!readInt(&choice)) {
            printf("Invalid input. Please enter a valid habit number (1-%d).\n", displayNum);
            continue;
        }
        if (choice < 1 || choice > displayNum) {
            printf("Invalid habit number. Please enter a number between 1 and %d.\n", displayNum);
            continue;
        }

        int realIndex = indexMap[choice];

        if (habits[realIndex].completed == 1) {
            printf("That habit is already marked as completed. Choose a different one.\n");
            continue;
        }

        /* confirmation */
        char prompt[256];
        snprintf(prompt, sizeof(prompt), "Mark '%s' as completed?", habits[realIndex].name);
        if (!confirmAction(prompt)) {
            printf("Operation cancelled.\n");
            printf("\n===================================================\n");
            return;
        }

        habits[realIndex].completed = 1;
        habits[realIndex].streak++;
        break;
    }

    printf("\nHabit marked as COMPLETED successfully!\n");
    showStatistics();
    printf("Keep going! Small progress is still progress.\n");
    printf("\n===================================================\n");
}

/* ── Archive Habit (improved UX) ───────────────────────── */
void archiveHabit() {
    int i, displayNum = 0, choice;
    int indexMap[MAX_HABITS];

    printf("\n");
    printf("====== ARCHIVE HABIT ======\n");
    printf("\n");

    /* build and display list of active habits with numbers */
    for (i = 0; i < habitCount; i++) {
        if (habits[i].archived == 0) {
            displayNum++;
            indexMap[displayNum] = i;
            printf("[%d] %s  %s\n", displayNum, habits[i].name,
                   habits[i].completed ? "(Completed)" : "(Not Done)");
        }
    }

    if (displayNum == 0) {
        printf("No active habits to archive.\n");
        printf("\n===================================================\n");
        return;
    }

    while (1) {
        printf("Enter Habit Number to archive : ");
        if (!readInt(&choice)) {
            printf("Invalid input. Please enter a valid habit number (1-%d).\n", displayNum);
            continue;
        }
        if (choice < 1 || choice > displayNum) {
            printf("Invalid habit number. Please enter a number between 1 and %d.\n", displayNum);
            continue;
        }
        break;
    }

    /* confirmation */
    {
        int realIndex = indexMap[choice];
        char prompt[256];
        snprintf(prompt, sizeof(prompt), "Archive '%s'? This cannot be undone easily.", habits[realIndex].name);
        if (!confirmAction(prompt)) {
            printf("Archive cancelled.\n");
            printf("\n===================================================\n");
            return;
        }
        habits[realIndex].archived = 1;
    }

    printf("\nHabit archived successfully!\n");
    printf("\n===================================================\n");
}

// ...existing code...