/* ============================================================
   PERSONAL HABIT TRACKER AND MOTIVATION ASSISTANT
   Multi-User Edition
   Language  : C
   Data Structure: Array of Structures (Students + Habits)
   Description: Enhanced with smart file handling — persistence,
                daily reset, streak tracking, history log,
                auto-backup, CSV export, safe recovery, and
                MULTI-USER SUPPORT for classrooms/groups.
   ============================================================ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

/* POSIX unistd/usleep/sleep are not available on native Windows MSVC.
   Provide a lightweight shim so the same source compiles on Windows (MinGW/MSYS or MSVC). */
#ifdef _WIN32
  #include <windows.h>
  /* Sleep expects milliseconds; provide POSIX-like aliases used in code */
  static void usleep(unsigned int usec) { Sleep((usec) / 1000); } /* micro -> ms */
  static unsigned int sleep(unsigned int seconds) { Sleep(seconds * 1000u); return 0; }
#else
  #include <unistd.h>
#endif

/* ============================================================
   ANSI COLOR CODES
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

/* ============================================================
   FILE PATHS — MULTI-USER VERSION
   ============================================================ */
#define STUDENTS_FILE     "students.dat"       /* Student registry + count  */
#define HABITS_DIR        "habits"             /* Directory: habits/student_N.dat */
#define BACKUP_DIR        "backup"             /* Directory: backup/student_N.bak */
#define LOG_DIR           "logs"               /* Directory: logs/student_N.log */
#define STUDENTS_CONFIG   "students_config.txt"/* Readable student list */
#define EXPORT_DIR        "exports"            /* Directory for CSV exports */

/* ============================================================
   FILE VALIDATION CONSTANTS
   ============================================================ */
#define FILE_MAGIC    0xABCD1234U
#define FILE_VERSION  2

/* ============================================================
   OTHER CONSTANTS
   ============================================================ */
#define MAX_STUDENTS    20
#define MAX_HABITS      100
#define NAME_LEN        100
#define DATE_LEN        12    /* "YYYY-MM-DD\0" */
#define LOG_LINES       50    /* Max lines held in memory for log view */

/* ============================================================
   ENHANCED HABIT STRUCT (unchanged)
   ============================================================ */
struct Habit {
    char name[NAME_LEN];
    int  completed;
    int  archived;
    int  streak;
    int  longest_streak;
    int  total_completions;
    char last_completed[DATE_LEN];
};

/* ============================================================
   STUDENT STRUCT (NEW)
   ============================================================ */
struct Student {
    char name[NAME_LEN];
    int  id;
    char created_date[DATE_LEN];
};

/* ============================================================
   FILE HEADER (unchanged)
   ============================================================ */
struct FileHeader {
    unsigned int magic;
    int          version;
    int          habitCount;
};

/* ============================================================
   GLOBAL STATE — MULTI-USER VERSION
   ============================================================ */
struct Student students[MAX_STUDENTS];
int studentCount = 0;
int currentStudentIndex = -1;  /* Currently selected student */

struct Habit habits[MAX_HABITS];
int habitCount = 0;

/* ============================================================
   MOTIVATIONAL QUOTES
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
   FUNCTION PROTOTYPES — File Handling (Multi-User)
   ============================================================ */
void initializeDirectories();
void loadStudents();
void saveStudents();
void addStudent(const char *name);
void deleteStudent(int index);
void selectStudent();
void listAllStudents();
void loadStudentHabits(int studentIndex);
void saveStudentHabits(int studentIndex);
void backupStudentHabits(int studentIndex);
void logAction(const char *action, const char *detail);
void viewHistoryLog();
void exportToCSV();
void checkDailyReset();
void saveLastRunDate();
void getTodayDate(char *buf);

/* ============================================================
   FUNCTION PROTOTYPES — Core Features
   ============================================================ */
void loadingScreen();
void showMotivationalQuote();
void displayMenu();
void addHabit();
void viewHabits();
void completeHabit();
void archiveHabit();
void viewArchivedHabits();
void deleteHabit();
void showStatistics();
void printSeparator(char c, int len);
void clearInputBuffer();
void pressEnterToContinue();

/* ============================================================
   MAIN
   ============================================================ */
int main() {
    int choice;
    system("chcp 65001 > nul");

    srand((unsigned int)time(NULL));

    loadingScreen();

    /* Initialize directories and load student registry */
    initializeDirectories();
    loadStudents();

    /* Multi-user startup: ask for number of students if first run */
    if (studentCount == 0) {
        printf(CYAN BOLD "\n  WELCOME TO MULTI-USER HABIT TRACKER!\n" RESET);
        printf(WHITE "  How many students will use this program? (1-%d): " RESET, MAX_STUDENTS);
        int numStudents;
        if (scanf("%d", &numStudents) != 1 || numStudents < 1 || numStudents > MAX_STUDENTS) {
            printf(RED "\n  [!] Invalid input. Starting with 1 student.\n" RESET);
            numStudents = 1;
        }
        clearInputBuffer();

        printf(CYAN "\n  Setting up %d student(s)...\n" RESET, numStudents);
        sleep(1);

        for (int i = 0; i < numStudents; i++) {
            char studentName[NAME_LEN];
            printf(WHITE "\n  Enter name for student %d: " RESET, i + 1);
            fgets(studentName, NAME_LEN, stdin);
            if (strchr(studentName, '\n') == NULL) clearInputBuffer();
            studentName[strcspn(studentName, "\n")] = '\0';

            if (strlen(studentName) > 0) {
                addStudent(studentName);
            } else {
                printf(YELLOW "  [!] Empty name. Skipping.\n" RESET);
                i--;
            }
        }

        saveStudents();
        printf(GREEN "\n  [✔] %d student(s) registered!\n" RESET, studentCount);
        sleep(2);
    }

    /* Let user select which student to track */
    selectStudent();

    if (currentStudentIndex == -1) {
        printf(RED "\n  [!] No student selected. Exiting.\n" RESET);
        return 1;
    }

    /* Load habits for the selected student */
    loadStudentHabits(currentStudentIndex);
    checkDailyReset();
    showMotivationalQuote();

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
            case 1:  addHabit();           break;
            case 2:  viewHabits();         break;
            case 3:  completeHabit();      break;
            case 4:  archiveHabit();       break;
            case 5:  viewArchivedHabits(); break;
            case 6:  showStatistics();     break;
            case 7:  exportToCSV();        break;
            case 8:  viewHistoryLog();     break;
            case 9:  deleteHabit();        break;
            case 10: { /* NEW: Switch student */
                    saveStudentHabits(currentStudentIndex);

                    selectStudent();

                    if (currentStudentIndex != -1) {
                        loadStudentHabits(currentStudentIndex);
                        checkDailyReset();
                    }

                    break;
                }  

            case 11: {                             /* NEW: Add student */
                char newName[NAME_LEN];
                printf(WHITE "\n  Enter new student name: " RESET);
                fgets(newName, NAME_LEN, stdin);
                if (strchr(newName, '\n') == NULL) clearInputBuffer();
                newName[strcspn(newName, "\n")] = '\0';
                if (strlen(newName) > 0) {
                    addStudent(newName);
                    saveStudents();
                    printf(GREEN "\n  [✔] Student \"%s\" added!\n" RESET, newName);
                } else {
                    printf(RED "\n  [!] Name cannot be empty.\n" RESET);
                }
                pressEnterToContinue();
                break;
            }
            case 12: {                             /* NEW: Delete student */
                listAllStudents();
                printf(RED "\n  Enter student number to DELETE (0 to cancel): " RESET);
                int delIdx;
                if (scanf("%d", &delIdx) != 1) {
                    clearInputBuffer();
                    printf(RED "\n  [!] Invalid input.\n" RESET);
                    pressEnterToContinue();
                    break;
                }
                clearInputBuffer();

                if (delIdx == 0) {
                    printf(YELLOW "  Cancelled.\n" RESET);
                    pressEnterToContinue();
                    break;
                }

                if (delIdx < 1 || delIdx > studentCount) {
                    printf(RED "\n  [!] Invalid student number.\n" RESET);
                    pressEnterToContinue();
                    break;
                }

                int realIdx = delIdx - 1;
                printf(RED "\n  [!] Permanently delete \"%s\"? This cannot be undone. (y/n): " RESET,
                       students[realIdx].name);
                char confirm;
                scanf(" %c", &confirm);
                clearInputBuffer();

                if (confirm == 'y' || confirm == 'Y') {
                    char delName[NAME_LEN];
                    strncpy(delName, students[realIdx].name, NAME_LEN - 1);
                    delName[NAME_LEN - 1] = '\0';
                    deleteStudent(realIdx);
                    saveStudents();

                    if (currentStudentIndex == realIdx) {
                        currentStudentIndex = -1;
                        if (studentCount > 0) {
                            selectStudent();
                            loadStudentHabits(currentStudentIndex);
                        }
                    } else if (currentStudentIndex > realIdx) {
                        currentStudentIndex--;
                    }

                    printf(GREEN "\n  [✔] \"%s\" deleted permanently.\n" RESET, delName);
                } else {
                    printf(YELLOW "  Deletion cancelled.\n" RESET);
                }
                pressEnterToContinue();
                break;
            }
            case 0:
                printf("\n");
                printSeparator('=', 60);
                printf(MAGENTA BOLD
                    "  \"The journey of a thousand miles begins with one step.\"\n"
                    "                                         — Lao Tzu\n"
                    RESET);
                printSeparator('=', 60);
                printf(GREEN BOLD
                    "\n  Thank you for using Habit Tracker! Keep going! \n\n"
                    RESET);
                logAction("SESSION", "User exited program");
                saveStudentHabits(currentStudentIndex);
                exit(0);
            default:
                printf(RED "\n  [!] Invalid choice. Please select 0–12.\n" RESET);
                pressEnterToContinue();
        }
    }

    return 0;
}

/* ============================================================
   INITIALIZE DIRECTORIES
   Creates necessary directories if they don't exist.
   ============================================================ */
void initializeDirectories() {
    #ifdef _WIN32
        // Windows version (no -p needed, works fine)
        system("mkdir habits 2>nul");
        system("mkdir backup 2>nul");
        system("mkdir logs 2>nul");
        system("mkdir exports 2>nul");
    #else
        // Linux/Mac version (uses -p)
        system("mkdir -p " HABITS_DIR);
        system("mkdir -p " BACKUP_DIR);
        system("mkdir -p " LOG_DIR);
        system("mkdir -p " EXPORT_DIR);
    #endif
}

/* ============================================================
   LOAD STUDENTS
   Reads student registry from students.dat.
   ============================================================ */
void loadStudents() {
    FILE *fp = fopen(STUDENTS_FILE, "rb");

    if (!fp) {
        printf(CYAN "\n  [i] No student registry found. Starting fresh.\n" RESET);
        sleep(1);
        studentCount = 0;
        return;
    }

    if (fread(&studentCount, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        printf(YELLOW "\n  [!] Student registry unreadable. Starting fresh.\n" RESET);
        studentCount = 0;
        return;
    }

    if (studentCount < 0 || studentCount > MAX_STUDENTS) {
        fclose(fp);
        printf(YELLOW "\n  [!] Invalid student count. Starting fresh.\n" RESET);
        studentCount = 0;
        return;
    }

    int n = (int)fread(students, sizeof(struct Student), studentCount, fp);
    fclose(fp);

    if (n == studentCount && studentCount > 0) {
        printf(GREEN "\n  [✔] Loaded %d student(s).\n" RESET, studentCount);
    }

    sleep(1);
}

/* ============================================================
   SAVE STUDENTS
   Writes student registry to students.dat.
   ============================================================ */
void saveStudents() {
    FILE *fp = fopen(STUDENTS_FILE, "wb");

    if (!fp) {
        printf(RED "\n  [!] ERROR: Could not save student registry.\n" RESET);
        return;
    }

    if (fwrite(&studentCount, sizeof(int), 1, fp) != 1) {
        printf(RED "\n  [!] ERROR: Failed to write student count.\n" RESET);
        fclose(fp);
        return;
    }

    if (studentCount > 0) {
        size_t written = fwrite(students, sizeof(struct Student), studentCount, fp);
        if ((int)written != studentCount) {
            printf(RED "\n  [!] WARNING: Only %zu of %d students saved.\n" RESET,
                   written, studentCount);
        }
    }

    fclose(fp);

    /* Also write a readable config file */
    fp = fopen(STUDENTS_CONFIG, "w");
    if (fp) {
        fprintf(fp, "Student Registry (%d total)\n", studentCount);
        fprintf(fp, "================================\n");
        for (int i = 0; i < studentCount; i++) {
            fprintf(fp, "%d. %s (ID: %d, Created: %s)\n",
                    i + 1, students[i].name, students[i].id, students[i].created_date);
        }
        fclose(fp);
    }
}

/* ============================================================
   ADD STUDENT
   Adds a new student to the registry.
   ============================================================ */
void addStudent(const char *name) {
    if (studentCount >= MAX_STUDENTS) {
        printf(RED "\n  [!] Maximum student limit (%d) reached!\n" RESET, MAX_STUDENTS);
        return;
    }

    strncpy(students[studentCount].name, name, NAME_LEN - 1);
    students[studentCount].name[NAME_LEN - 1] = '\0';
    students[studentCount].id = studentCount + 1;
    getTodayDate(students[studentCount].created_date);

    studentCount++;
}

/* ============================================================
   DELETE STUDENT
   Removes a student from the registry by shifting array.
   ============================================================ */
void deleteStudent(int index) {
    if (index < 0 || index >= studentCount) return;

    char path[256];
    /* Remove from the files*/
    snprintf(path, sizeof(path), "%s/student_%d.dat",
            HABITS_DIR, students[index].id);
    remove(path);

    snprintf(path, sizeof(path), "%s/student_%d.log",
            LOG_DIR, students[index].id);
    remove(path);

    snprintf(path, sizeof(path), "%s/student_%d.bak",
            BACKUP_DIR, students[index].id);
    remove(path);

    snprintf(path, sizeof(path), "%s/student_%d.lastrun",
            LOG_DIR, students[index].id);
    remove(path);

    /* Shift array left */
    for (int i = index; i < studentCount - 1; i++) {
        students[i] = students[i + 1];
    }
    studentCount--;
}

/* ============================================================
   SELECT STUDENT
   Interactive menu to choose which student to work with.
   ============================================================ */
void selectStudent() {
    printf("\n");
    printSeparator('=', 60);
    printf(CYAN BOLD "    SELECT A STUDENT\n" RESET);
    printSeparator('=', 60);

    listAllStudents();

    printf(WHITE "\n   Enter student number to enter (0 to cancel): " RESET);
    int choice;
    if (scanf("%d", &choice) != 1) {
        clearInputBuffer();
        printf(RED "\n  [!] Invalid input.\n" RESET);
        currentStudentIndex = -1;
        return;
    }
    clearInputBuffer();

    if (choice == 0) {
        currentStudentIndex = -1;
        return;
    }

    if (choice < 1 || choice > studentCount) {
        printf(RED "\n  [!] Invalid student number.\n" RESET);
        currentStudentIndex = -1;
        return;
    }

    currentStudentIndex = choice - 1;
    printf(GREEN "\n  [✔] Selected: %s\n" RESET, students[currentStudentIndex].name);
    sleep(1);
}

/* ============================================================
   LIST ALL STUDENTS
   Displays all registered students.
   ============================================================ */
void listAllStudents() {
    if (studentCount == 0) {
        printf(YELLOW "  No students registered yet.\n" RESET);
        return;
    }

    printf(BOLD "  %-4s %-30s %-12s\n" RESET, "No.", "Name", "Created");
    printSeparator('-', 60);

    for (int i = 0; i < studentCount; i++) {
        printf("  %-4d %-30s %-12s\n", i + 1, students[i].name, students[i].created_date);
    }
}

/* ============================================================
   LOAD STUDENT HABITS
   Loads habits for a specific student.
   ============================================================ */
void loadStudentHabits(int studentIndex) {
    if (studentIndex < 0 || studentIndex >= studentCount) {
        habitCount = 0;
        return;
    }

    char filePath[256];
    snprintf(filePath, sizeof(filePath), "%s/student_%d.dat", HABITS_DIR, students[studentIndex].id);

    FILE *fp = fopen(filePath, "rb");

    if (!fp) {
        habitCount = 0;
        printf(CYAN "\n  [i] No habits found for %s. Starting fresh.\n" RESET,
               students[studentIndex].name);
        sleep(1);
        return;
    }

    struct FileHeader header;
    if (fread(&header, sizeof(struct FileHeader), 1, fp) != 1) {
        fclose(fp);
        printf(YELLOW "\n  [!] Habits file unreadable for %s.\n" RESET,
               students[studentIndex].name);
        habitCount = 0;
        return;
    }

    if (header.magic != FILE_MAGIC || header.version != FILE_VERSION) {
        fclose(fp);
        printf(YELLOW "\n  [!] Habits file format mismatch for %s.\n" RESET,
               students[studentIndex].name);
        habitCount = 0;
        return;
    }

    if (header.habitCount < 0 || header.habitCount > MAX_HABITS) {
        fclose(fp);
        printf(YELLOW "\n  [!] Invalid habit count for %s.\n" RESET,
               students[studentIndex].name);
        habitCount = 0;
        return;
    }

    int n = (int)fread(habits, sizeof(struct Habit), header.habitCount, fp);
    habitCount = n;
    fclose(fp);

    if (habitCount > 0) {
        printf(GREEN "\n  [✔] Loaded %d habit(s) for %s.\n" RESET,
               habitCount, students[studentIndex].name);
    }

    sleep(1);
}

/* ============================================================
   SAVE STUDENT HABITS
   Saves habits for a specific student.
   ============================================================ */
void saveStudentHabits(int studentIndex) {
    if (studentIndex < 0 || studentIndex >= studentCount) return;

    backupStudentHabits(studentIndex);

    char filePath[256];
    snprintf(filePath, sizeof(filePath), "%s/student_%d.dat", HABITS_DIR, students[studentIndex].id);

    FILE *fp = fopen(filePath, "wb");
    if (!fp) {
        printf(RED "\n  [!] ERROR: Could not save habits.\n" RESET);
        return;
    }

    struct FileHeader header;
    header.magic      = FILE_MAGIC;
    header.version    = FILE_VERSION;
    header.habitCount = habitCount;

    if (fwrite(&header, sizeof(struct FileHeader), 1, fp) != 1) {
        printf(RED "\n  [!] ERROR: Failed to write file header.\n" RESET);
        fclose(fp);
        return;
    }

    if (habitCount > 0) {
        size_t written = fwrite(habits, sizeof(struct Habit), habitCount, fp);
        if ((int)written != habitCount) {
            printf(RED "\n  [!] WARNING: Only %zu of %d habits saved.\n" RESET,
                   written, habitCount);
        }
    }

    fclose(fp);
}

/* ============================================================
   BACKUP STUDENT HABITS
   Creates backup of student's habits file.
   ============================================================ */
void backupStudentHabits(int studentIndex) {
    if (studentIndex < 0 || studentIndex >= studentCount) return;

    char srcPath[256], dstPath[256];
    snprintf(srcPath, sizeof(srcPath), "%s/student_%d.dat", HABITS_DIR, students[studentIndex].id);
    snprintf(dstPath, sizeof(dstPath), "%s/student_%d.bak", BACKUP_DIR, students[studentIndex].id);

    FILE *src = fopen(srcPath, "rb");
    if (!src) return;

    FILE *dst = fopen(dstPath, "wb");
    if (!dst) {
        fclose(src);
        return;
    }

    char buf[512];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), src)) > 0) {
        fwrite(buf, 1, n, dst);
    }

    fclose(src);
    fclose(dst);
}

/* ============================================================
   FILE HANDLING: GET TODAY'S DATE
   ============================================================ */
void getTodayDate(char *buf) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buf, DATE_LEN, "%Y-%m-%d", tm_info);
}

/* ============================================================
   FILE HANDLING: LOG ACTION
   ============================================================ */
void logAction(const char *action, const char *detail) {
    if (currentStudentIndex < 0 || currentStudentIndex >= studentCount) return;

    char logPath[256];
    snprintf(logPath, sizeof(logPath), "%s/student_%d.log", LOG_DIR, students[currentStudentIndex].id);

    FILE *fp = fopen(logPath, "a");
    if (!fp) return;

    char dateStr[DATE_LEN];
    getTodayDate(dateStr);

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char timeStr[10];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", tm_info);

    fprintf(fp, "[%s %s] %-14s : %s\n", dateStr, timeStr, action, detail);
    fclose(fp);
}

/* ============================================================
   FILE HANDLING: VIEW HISTORY LOG
   ============================================================ */
void viewHistoryLog() {
    if (currentStudentIndex < 0) {
        printf(RED "\n  [!] No student selected.\n" RESET);
        pressEnterToContinue();
        return;
    }

    printf("\n");
    printSeparator('=', 60);
    printf(BLUE BOLD "    HISTORY LOG (last 20 entries)\n" RESET);
    printSeparator('=', 60);

    char logPath[256];
    snprintf(logPath, sizeof(logPath), "%s/student_%d.log", LOG_DIR, students[currentStudentIndex].id);

    FILE *fp = fopen(logPath, "r");
    if (!fp) {
        printf(YELLOW "  No history log found yet.\n" RESET);
        printSeparator('=', 60);
        pressEnterToContinue();
        return;
    }

    char lines[20][256];
    int total = 0;

    while (fgets(lines[total % 20], 256, fp)) {
        total++;
    }

    fclose(fp);

    int start = (total > 20) ? total - 20 : 0;

    if (total > 20) {
        printf(YELLOW
            "  (Showing last 20 of %d recorded entries)\n\n"
            RESET,
            total);
    }

    for (int i = start; i < total; i++) {

        char *line = lines[i % 20];

        if      (strstr(line, "COMPLETED"))
            printf(GREEN   "  %s" RESET, line);

        else if (strstr(line, "ADDED"))
            printf(CYAN    "  %s" RESET, line);

        else if (strstr(line, "ARCHIVED"))
            printf(MAGENTA "  %s" RESET, line);

        else if (strstr(line, "DELETED"))
            printf(RED     "  %s" RESET, line);

        else if (strstr(line, "DAY_RESET"))
            printf(YELLOW  "  %s" RESET, line);

        else
            printf(WHITE   "  %s" RESET, line);
    }
    if (total == 0) {
        printf(YELLOW "  No entries yet.\n" RESET);
    }

    printSeparator('=', 60);
    pressEnterToContinue();
}

/* ============================================================
   FILE HANDLING: EXPORT TO CSV
   ============================================================ */
void exportToCSV() {
    if (currentStudentIndex < 0) {
        printf(RED "\n  [!] No student selected.\n" RESET);
        pressEnterToContinue();
        return;
    }

    printf("\n");
    printSeparator('=', 60);
    printf(CYAN BOLD "    EXPORT TO CSV\n" RESET);
    printSeparator('=', 60);

    if (habitCount == 0) {
        printf(YELLOW "  No habits to export.\n" RESET);
        pressEnterToContinue();
        return;
    }

    char exportPath[256];
    snprintf(exportPath, sizeof(exportPath), "%s/%s_habits.csv",
             EXPORT_DIR, students[currentStudentIndex].name);

    FILE *fp = fopen(exportPath, "w");
    if (!fp) {
        printf(RED "\n  [!] ERROR: Could not create CSV file.\n" RESET);
        pressEnterToContinue();
        return;
    }

    fprintf(fp, "Name,Status,Archived,Streak,Longest Streak,"
                "Total Completions,Last Completed\n");

    for (int i = 0; i < habitCount; i++) {
        const char *status   = habits[i].completed ? "Completed" : "Pending";
        const char *archived = habits[i].archived  ? "Yes"       : "No";
        const char *lastDate = habits[i].last_completed[0]
                                   ? habits[i].last_completed
                                   : "Never";

        fprintf(fp, "\"%s\",%s,%s,%d,%d,%d,%s\n",
            habits[i].name,
            status,
            archived,
            habits[i].streak,
            habits[i].longest_streak,
            habits[i].total_completions,
            lastDate
        );
    }

    fclose(fp);
    printf(GREEN "\n  [✔] Exported %d habit(s) to \"%s\"!\n" RESET,
           habitCount, exportPath);
    pressEnterToContinue();
}

/* ============================================================
   FILE HANDLING: DAILY RESET
   ============================================================ */
void checkDailyReset() {
    if (currentStudentIndex < 0) return;

    char today[DATE_LEN];
    char lastRun[DATE_LEN] = "";
    getTodayDate(today);

    char runDatePath[256];
    snprintf(runDatePath, sizeof(runDatePath), "%s/student_%d.lastrun", LOG_DIR,
             students[currentStudentIndex].id);

    FILE *fp = fopen(runDatePath, "r");
    if (fp) {
        if (fgets(lastRun, DATE_LEN, fp)) {
            lastRun[strcspn(lastRun, "\n")] = '\0';
        }
        fclose(fp);
    }

    if (strcmp(today, lastRun) == 0) {
        saveLastRunDate();
        return;
    }

    if (lastRun[0] == '\0') {
        saveLastRunDate();
        return;
    }

    printf(CYAN "\n  [✦] New day! Updating streaks for %s...\n" RESET, today);
    sleep(1);

    int done = 0, total = 0;
    for (int i = 0; i < habitCount; i++) {
        if (habits[i].archived) continue;
        total++;
        if (habits[i].completed) {
            done++;
            habits[i].streak++;
            if (habits[i].streak > habits[i].longest_streak) {
                habits[i].longest_streak = habits[i].streak;
            }
        } else {
            habits[i].streak = 0;
        }
        habits[i].completed = 0;
    }

    char summary[80];
    snprintf(summary, sizeof(summary), "Day summary %s — %d/%d completed", lastRun, done, total);
    logAction("DAY_RESET", summary);

    saveStudentHabits(currentStudentIndex);
    saveLastRunDate();

    printf(GREEN "  [✔] Streaks updated!\n" RESET);
    sleep(1);
}

/* ============================================================
   FILE HANDLING: SAVE LAST RUN DATE
   ============================================================ */
void saveLastRunDate() {
    if (currentStudentIndex < 0) return;

    char today[DATE_LEN];
    getTodayDate(today);

    char runDatePath[256];
    snprintf(runDatePath, sizeof(runDatePath), "%s/student_%d.lastrun", LOG_DIR,
             students[currentStudentIndex].id);

    FILE *fp = fopen(runDatePath, "w");
    if (fp) {
        fprintf(fp, "%s\n", today);
        fclose(fp);
    }
}

/* ============================================================
   LOADING SCREEN
   ============================================================ */
void loadingScreen() {
    int i;
    printf("\033[2J\033[H");
    printf("\n\n");
    printf(CYAN BOLD);
    printf("  ██╗  ██╗ █████╗ ██████╗ ██╗████████╗    ████████╗██████╗  █████╗  ██████╗██╗  ██╗███████╗██████╗ \n");
    printf("  ██║  ██║██╔══██╗██╔══██╗██║╚══██╔══╝    ╚══██╔══╝██╔══██╗██╔══██╗██╔════╝██║ ██╔╝██╔════╝██╔══██╗\n");
    printf("  ███████║███████║██████╔╝██║   ██║          ██║   ██████╔╝███████║██║     █████╔╝ █████╗  ██████╔╝\n");
    printf("  ██╔══██║██╔══██║██╔══██╗██║   ██║          ██║   ██╔══██╗██╔══██║██║     ██╔═██╗ ██╔══╝  ██╔══██╗\n");
    printf("  ██║  ██║██║  ██║██████╔╝██║   ██║          ██║   ██║  ██║██║  ██║╚██████╗██║  ██╗███████╗██║  ██║\n");
    printf("  ╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝ ╚═╝   ╚═╝          ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝\n");
    printf(RESET);
    printf(YELLOW BOLD "\n       Personal Habit Tracker & Motivation Assistant — MULTI-USER\n" RESET);
    printf(WHITE       "                  Build Better Habits. One Day at a Time.\n\n" RESET);

    printf(GREEN "  Loading ");
    fflush(stdout);
    for (i = 0; i < 20; i++) {
        printf("█");
        fflush(stdout);
        usleep(50000);
    }
    printf(" Done!\n\n" RESET);
    sleep(1);
}

/* ============================================================
   MOTIVATIONAL QUOTE
   ============================================================ */
void showMotivationalQuote() {
    int index = rand() % QUOTE_COUNT;
    printf("\033[2J\033[H");
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
   DISPLAY MENU — UPDATED WITH NEW OPTIONS
   ============================================================ */
void displayMenu() {
    printf("DEBUG: displayMenu() compiled version — expecting 12 options\n"); // debug marker
    printf("\033[2J\033[H");
    printf("\n");
    printSeparator('=', 60);
    printf(BLUE BOLD "         PERSONAL HABIT TRACKER — MAIN MENU\n" RESET);
    printf(WHITE "         Currently tracking: " RESET);
    if (currentStudentIndex >= 0) {
        printf(GREEN "%s\n" RESET, students[currentStudentIndex].name);
    } else {
        printf(RED "None\n" RESET);
    }
    printSeparator('=', 60);
    printf(GREEN   "  [1]" RESET WHITE "  Add New Habit\n"              RESET);
    printf(CYAN    "  [2]" RESET WHITE "  View All Habits\n"            RESET);
    printf(YELLOW  "  [3]" RESET WHITE "  Mark Habit as Completed\n"   RESET);
    printf(MAGENTA "  [4]" RESET WHITE "  Archive Habit\n"             RESET);
    printf(BLUE    "  [5]" RESET WHITE "  View Archived Habits\n"      RESET);
    printf(CYAN    "  [6]" RESET WHITE "  Show Statistics\n"           RESET);
    printf(GREEN   "  [7]" RESET WHITE "  Export Habits (Save to Excel/Google Sheets)\n"    RESET);
    printf(YELLOW  "  [8]" RESET WHITE "  View History Log\n"          RESET);
    printf(RED     "  [9]" RESET WHITE "  Delete Habit (Permanent)\n"  RESET);
    printf(MAGENTA "  [10]" RESET WHITE " Switch Student\n"            RESET);
    printf(CYAN    "  [11]" RESET WHITE " Add New Student\n"           RESET);
    printf(RED     "  [12]" RESET WHITE " Delete Student (Permanent)\n" RESET);
    printf(WHITE   "  [0]" RESET WHITE "  Exit\n"                      RESET);
    printSeparator('=', 60);
    printf("\n");
}

/* ============================================================
   ADD NEW HABIT
   ============================================================ */
void addHabit() {
    if (currentStudentIndex < 0) {
        printf(RED "\n  [!] No student selected.\n" RESET);
        pressEnterToContinue();
        return;
    }

    char newName[NAME_LEN];
    int i;

    printf("\n");
    printSeparator('-', 60);
    printf(GREEN BOLD "    ADD NEW HABIT\n" RESET);
    printSeparator('-', 60);

    if (habitCount >= MAX_HABITS) {
        printf(RED "\n  [!] Maximum habit limit (%d) reached!\n" RESET, MAX_HABITS);
        pressEnterToContinue();
        return;
    }

    printf(WHITE "  Enter habit name: " RESET);
    fgets(newName, NAME_LEN, stdin);

    if (strchr(newName, '\n') == NULL) clearInputBuffer();
    newName[strcspn(newName, "\n")] = '\0';

    if (strlen(newName) == 0) {
        printf(RED "\n  [!] Habit name cannot be empty!\n" RESET);
        pressEnterToContinue();
        return;
    }

    /* Duplicate check */
    for (i = 0; i < habitCount; i++) {
        if (habits[i].archived == 1) continue;

        char existing[NAME_LEN], input[NAME_LEN];
        int k;
        strncpy(existing, habits[i].name, NAME_LEN - 1);
        existing[NAME_LEN - 1] = '\0';
        strncpy(input, newName, NAME_LEN - 1);
        input[NAME_LEN - 1] = '\0';
        for (k = 0; existing[k]; k++) existing[k] = tolower((unsigned char)existing[k]);
        for (k = 0; input[k];    k++) input[k]    = tolower((unsigned char)input[k]);

        if (strcmp(existing, input) == 0) {
            printf(YELLOW "\n  [!] Duplicate: \"%s\" already exists.\n" RESET, habits[i].name);
            pressEnterToContinue();
            return;
        }
    }

    strncpy(habits[habitCount].name, newName, NAME_LEN - 1);
    habits[habitCount].name[NAME_LEN - 1]      = '\0';
    habits[habitCount].completed               = 0;
    habits[habitCount].archived                = 0;
    habits[habitCount].streak                  = 0;
    habits[habitCount].longest_streak          = 0;
    habits[habitCount].total_completions       = 0;
    habits[habitCount].last_completed[0]       = '\0';
    habitCount++;

    logAction("ADDED", newName);
    saveStudentHabits(currentStudentIndex);

    printf(GREEN "\n  [✔] \"%s\" added successfully!\n" RESET, newName);
    printf(WHITE "  Total habits tracked: %d / %d\n" RESET, habitCount, MAX_HABITS);
    pressEnterToContinue();
}

/* ============================================================
   VIEW HABITS
   ============================================================ */
void viewHabits() {
    if (currentStudentIndex < 0) {
        printf(RED "\n  [!] No student selected.\n" RESET);
        pressEnterToContinue();
        return;
    }

    int i, displayNum = 0;

    printf("\n");
    printSeparator('=', 60);
    printf(CYAN BOLD "   ACTIVE HABITS\n" RESET);
    printSeparator('=', 60);
    printf(BOLD "  %-4s %-28s %-11s %-6s\n" RESET, "No.", "Habit Name", "Status", "Streak");
    printSeparator('-', 60);

    for (i = 0; i < habitCount; i++) {
        if (habits[i].archived == 0) {
            displayNum++;
            const char *statusColor = habits[i].completed ? GREEN  : YELLOW;
            const char *statusLabel = habits[i].completed ? "✔ Done" : "○ Pending";

            printf("  %-4d %-28s %s%-11s%s %s%d day(s)%s\n",
                displayNum,
                habits[i].name,
                statusColor, statusLabel, RESET,
                habits[i].streak > 0 ? GREEN : WHITE,
                habits[i].streak,
                RESET);
        }
    }

    if (displayNum == 0) {
        printf(YELLOW "  No active habits. Add some first!\n" RESET);
    }

    printSeparator('=', 60);
    printf(WHITE "  Active: %d\n" RESET, displayNum);
    pressEnterToContinue();
}

/* ============================================================
   COMPLETE HABIT
   ============================================================ */
void completeHabit() {
    if (currentStudentIndex < 0) {
        printf(RED "\n  [!] No student selected.\n" RESET);
        pressEnterToContinue();
        return;
    }

    int i, displayNum = 0, choice;
    int indexMap[MAX_HABITS];

    printf("\n");
    printSeparator('=', 60);
    printf(YELLOW BOLD "    MARK HABIT AS COMPLETED\n" RESET);
    printSeparator('=', 60);
    printf(BOLD "  %-4s %-28s %-11s\n" RESET, "No.", "Habit Name", "Status");
    printSeparator('-', 60);

    for (i = 0; i < habitCount; i++) {
        if (habits[i].archived == 0) {
            displayNum++;
            indexMap[displayNum] = i;
            const char *statusColor = habits[i].completed ? GREEN  : YELLOW;
            const char *statusLabel = habits[i].completed ? " Completed" : "○ Pending";
            printf("  %-4d %-28s %s%-11s%s\n",
                displayNum, habits[i].name, statusColor, statusLabel, RESET);
        }
    }

    if (displayNum == 0) {
        printf(YELLOW "\n  No active habits!\n" RESET);
        pressEnterToContinue();
        return;
    }

    printSeparator('-', 60);
    printf(WHITE "  Enter habit number to complete (0 to cancel): " RESET);
    if (scanf("%d", &choice) != 1) {
        clearInputBuffer();
        printf(RED "\n  [!] Invalid input.\n" RESET);
        pressEnterToContinue();
        return;
    }
    clearInputBuffer();

    if (choice == 0) {
        printf(YELLOW "  Cancelled.\n" RESET);
        pressEnterToContinue();
        return;
    }

    if (choice < 1 || choice > displayNum) {
        printf(RED "\n  [!] Invalid habit number.\n" RESET);
        pressEnterToContinue();
        return;
    }

    int realIndex = indexMap[choice];

    if (habits[realIndex].completed == 1) {
        printf(YELLOW "\n  [!] \"%s\" is already completed today!\n" RESET,
               habits[realIndex].name);
        pressEnterToContinue();
        return;
    }

    habits[realIndex].completed = 1;
    habits[realIndex].total_completions++;
    getTodayDate(habits[realIndex].last_completed);

    logAction("COMPLETED", habits[realIndex].name);
    saveStudentHabits(currentStudentIndex);

    printf(GREEN "\n  [✔] \"%s\" marked as COMPLETED! Great job!\n" RESET,
           habits[realIndex].name);
    printf(CYAN  "  Total completions for this habit: %d\n" RESET,
           habits[realIndex].total_completions);

    showStatistics();
}

/* ============================================================
   ARCHIVE HABIT
   ============================================================ */
void archiveHabit() {
    if (currentStudentIndex < 0) {
        printf(RED "\n  [!] No student selected.\n" RESET);
        pressEnterToContinue();
        return;
    }

    int i, displayNum = 0, choice;
    int indexMap[MAX_HABITS];
    char confirm;

    printf("\n");
    printSeparator('=', 60);
    printf(MAGENTA BOLD "    ARCHIVE HABIT\n" RESET);
    printSeparator('=', 60);
    printf(BOLD "  %-4s %-28s %-11s\n" RESET, "No.", "Habit Name", "Status");
    printSeparator('-', 60);

    for (i = 0; i < habitCount; i++) {
        if (habits[i].archived == 0) {
            displayNum++;
            indexMap[displayNum] = i;
            const char *statusColor = habits[i].completed ? GREEN  : YELLOW;
            const char *statusLabel = habits[i].completed ? " Completed" : " Pending";
            printf("  %-4d %-28s %s%-11s%s\n",
                displayNum, habits[i].name, statusColor, statusLabel, RESET);
        }
    }

    if (displayNum == 0) {
        printf(YELLOW "\n  No active habits to archive.\n" RESET);
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
        printf(YELLOW "  Cancelled.\n" RESET);
        pressEnterToContinue();
        return;
    }

    if (choice < 1 || choice > displayNum) {
        printf(RED "\n  [!] Invalid habit number.\n" RESET);
        pressEnterToContinue();
        return;
    }

    int realIndex = indexMap[choice];

    printf(YELLOW "\n  Archive \"%s\"? (y/n): " RESET, habits[realIndex].name);
    scanf(" %c", &confirm);
    clearInputBuffer();

    if (confirm == 'y' || confirm == 'Y') {
        habits[realIndex].archived = 1;
        logAction("ARCHIVED", habits[realIndex].name);
        saveStudentHabits(currentStudentIndex);
        printf(GREEN "\n  [✔] \"%s\" archived successfully.\n" RESET, habits[realIndex].name);
    } else {
        printf(YELLOW "  Cancelled.\n" RESET);
    }

    pressEnterToContinue();
}

/* ============================================================
   VIEW ARCHIVED HABITS
   ============================================================ */
void viewArchivedHabits() {
    if (currentStudentIndex < 0) {
        printf(RED "\n  [!] No student selected.\n" RESET);
        pressEnterToContinue();
        return;
    }

    int i, displayNum = 0;

    printf("\n");
    printSeparator('=', 60);
    printf(BLUE BOLD "    ARCHIVED HABITS\n" RESET);
    printSeparator('=', 60);
    printf(BOLD "  %-4s %-25s %-6s %-7s %-8s\n" RESET,
           "No.", "Habit Name", "Streak", "Best", "Total");
    printSeparator('-', 60);

    for (i = 0; i < habitCount; i++) {
        if (habits[i].archived == 1) {
            displayNum++;
            printf("  %-4d %-25s %-6d %-7d %-8d\n",
                displayNum,
                habits[i].name,
                habits[i].streak,
                habits[i].longest_streak,
                habits[i].total_completions);
        }
    }

    if (displayNum == 0) {
        printf(YELLOW "  No archived habits yet.\n" RESET);
    }

    printSeparator('=', 60);
    printf(WHITE "  Total archived: %d\n" RESET, displayNum);
    pressEnterToContinue();
}

/* ============================================================
   DELETE HABIT (PERMANENT)
   ============================================================ */
void deleteHabit() {
    if (currentStudentIndex < 0) {
        printf(RED "\n  [!] No student selected.\n" RESET);
        pressEnterToContinue();
        return;
    }

    int i, displayNum = 0, choice;
    int indexMap[MAX_HABITS];
    char confirm;

    printf("\n");
    printSeparator('=', 60);
    printf(RED BOLD "    DELETE HABIT (PERMANENT)\n" RESET);
    printSeparator('=', 60);
    printf(YELLOW "  Note: Only archived habits can be permanently deleted.\n"
                  "  Use option [4] to archive an active habit first.\n" RESET);
    printSeparator('-', 60);
    printf(BOLD "  %-4s %-28s %-8s\n" RESET, "No.", "Habit Name", "Total");
    printSeparator('-', 60);

    for (i = 0; i < habitCount; i++) {
        if (habits[i].archived == 1) {
            displayNum++;
            indexMap[displayNum] = i;
            printf("  %-4d %-28s %-8d\n",
                displayNum,
                habits[i].name,
                habits[i].total_completions);
        }
    }

    if (displayNum == 0) {
        printf(YELLOW "\n  No archived habits to delete.\n" RESET);
        pressEnterToContinue();
        return;
    }

    printSeparator('-', 60);
    printf(WHITE "  Enter habit number to DELETE (0 to cancel): " RESET);
    if (scanf("%d", &choice) != 1) {
        clearInputBuffer();
        printf(RED "\n  [!] Invalid input.\n" RESET);
        pressEnterToContinue();
        return;
    }
    clearInputBuffer();

    if (choice == 0) {
        printf(YELLOW "  Cancelled.\n" RESET);
        pressEnterToContinue();
        return;
    }

    if (choice < 1 || choice > displayNum) {
        printf(RED "\n  [!] Invalid habit number.\n" RESET);
        pressEnterToContinue();
        return;
    }

    int realIndex = indexMap[choice];
    char deletedName[NAME_LEN];
    strncpy(deletedName, habits[realIndex].name, NAME_LEN - 1);
    deletedName[NAME_LEN - 1] = '\0';

    printf(RED "\n  [!] Permanently delete \"%s\"? This cannot be undone. (y/n): " RESET,
           deletedName);
    scanf(" %c", &confirm);
    clearInputBuffer();

    if (confirm == 'y' || confirm == 'Y') {
        for (i = realIndex; i < habitCount - 1; i++) {
            habits[i] = habits[i + 1];
        }
        habitCount--;

        logAction("DELETED", deletedName);
        saveStudentHabits(currentStudentIndex);

        printf(GREEN "\n  [✔] \"%s\" permanently deleted.\n" RESET, deletedName);
    } else {
        printf(YELLOW "  Deletion cancelled.\n" RESET);
    }

    pressEnterToContinue();
}

/* ============================================================
   SHOW STATISTICS
   ============================================================ */
void showStatistics() {
    if (currentStudentIndex < 0) {
        printf(RED "\n  [!] No student selected.\n" RESET);
        pressEnterToContinue();
        return;
    }

    int i, total = 0, completed = 0;
    int best_streak = 0;
    char best_habit[NAME_LEN] = "none";
    float percentage = 0.0f;

    for (i = 0; i < habitCount; i++) {
        if (habits[i].archived == 0) {
            total++;
            if (habits[i].completed) completed++;
            if (habits[i].longest_streak > best_streak) {
                best_streak = habits[i].longest_streak;
                strncpy(best_habit, habits[i].name, NAME_LEN - 1);
                best_habit[NAME_LEN - 1] = '\0';
            }
        }
    }

    if (total > 0) percentage = ((float)completed / total) * 100.0f;

    printf("\n");
    printSeparator('-', 60);
    printf(CYAN BOLD "    HABIT STATISTICS\n" RESET);
    printSeparator('-', 60);
    printf(WHITE  "  Total Active Habits : %d\n"  RESET, total);
    printf(GREEN  "  Completed Today     : %d\n"  RESET, completed);
    printf(YELLOW "  Remaining           : %d\n"  RESET, total - completed);
    printf(MAGENTA"  Best Streak Habit   : %s (%d days)\n" RESET, best_habit, best_streak);

    printf(CYAN   "  Today's Progress    : [");
    int bars = (int)(percentage / 5);
    int j;
    for (j = 0; j < 20; j++) {
        if (j < bars) { printf(GREEN "█" RESET CYAN); usleep(40000); }
        else          { printf("░");                  usleep(40000); }
    }
    printf(CYAN "] %.1f%%\n" RESET, percentage);
    printSeparator('-', 60);
    pressEnterToContinue();
}

/* ============================================================
   UTILITY FUNCTIONS
   ============================================================ */
void printSeparator(char c, int len) {
    int i;
    printf("  ");
    for (i = 0; i < len; i++) printf("%c", c);
    printf("\n");
}

void clearInputBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void pressEnterToContinue() {
    usleep(25000);
    printf(WHITE "\n  Press " CYAN "[Enter]" WHITE " to continue..." RESET);
    getchar();
}
