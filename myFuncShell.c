#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>


void welcome();
void getLocation();
char **splitArgument(char *str);
void move(char **args);
void delete(char *str);
void logout(char *str);
void cd(char **args);
void cp(char **args);

int main()
{
    welcome();
    getLocation();

    char input[256];

    printf("Enter a command: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; // הסרת '\n' בסוף

    char **arguments = splitArgument(input);
    if (!arguments) // אם קרתה שגיאה בפיצול
    {
        printf("Error splitting arguments.\n");
        return 1;
        // free(arguments);

        return 1;
    }

    printf("Parsed Arguments:\n");
    for (int i = 0; arguments[i] != NULL; i++)
    printf("%s\n", arguments[i]);

    // free(arguments); // שחרור המערך הראשי


    if (strcmp(arguments[0], "delete") == 0) {
        delete(arguments[1]); // מפעיל את הפונקציה delete
    }
    // free(arguments); // שחרור המערך הראשי
    else if (strcmp(arguments[0], "move") == 0) {
        move(arguments); // קריאה לפונקציה move
    } else if (strcmp(arguments[0], "exit") == 0) {
        printf("Exiting shell...\n");
        free(arguments);
        return 0;
    } else {
        printf("Unknown command: %s\n", arguments[0]);
    }

    free(arguments);
    return 0;
}


void welcome()
{
    printf("\n\033[1;31m"); // צבע אדום
    
    printf(" ***************************************************************\n");
    printf("----------------------------------------------------------------\n");
    printf("  *       *  *****  *       *****   *****  *     *  *****  \n");
    printf("  *       *  *      *       *       *   *  * * * *  *      \n");
    printf("  *   *   *  ***    *       *       *   *  *  *  *  ***    \n");
    printf("  *  * *  *  *      *       *       *   *  *     *  *      \n");
    printf("  * *   * *  *****  *****   *****   *****  *     *  *****  \n");
    printf("----------------------------------------------------------------\n");
    printf(" ***************************************************************\n");
    
    printf("\033[0m\n"); // איפוס צבע
    printf("Welcome to myShell \n\n");
}

void getLocation()
{
    char cwd[PATH_MAX];  // הנתיב הנוכחי
    char hostname[1024]; // אחסון שם המחשב // 1024 מספר מקובל למגבלת
    // getenv() היא פונקציה מספריית <stdlib.h>, שמקבלת שם של משתנה סביבה (כמו "USER", "HOME", "PATH") ומחזירה מצביע (char *) למחרוזת עם הערך שלו.
    char *username = getenv("USER"); // לקבל ממערכת ההפעלה את שם המשתמש // getenv מספריית  <stdio.h>

    // שם המשתמש
    if (username == NULL)
    {
        username = "no know user";
    }
    // שם המחשב
    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        snprintf(hostname, sizeof(hostname), "unknown");
    }

    // הנתיב הנוכחי
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        snprintf(cwd, sizeof(cwd), "no know path");
    }

    printf("\033[1;32m%s@%s\033[0m", username, hostname);
    printf("\033[1;34m%s\033[0m", cwd);
}

char **splitArgument(char *str)
{
    if (str == NULL || *str == '\0') return NULL; // בדיקה אם הקלט ריק

    int count = 0;
    int length = strlen(str);

    // מקצים מקום למערך מצביעים (נניח עד 20 מילים, אפשר להגדיל אם צריך)
    char **words = malloc(20 * sizeof(char *));
    if (!words)
    {
        perror("Memory failed");
        return NULL;
    }

    for (int i = 0; i < length; i++)
    {
        // דילוג על רווחים בתחילת מילה
        while (str[i] == ' ') i++;
        if (str[i] == '\0') break; // אם הגענו לסוף המחרוזת, יוצאים

        // שמירת כתובת תחילת המילה
        words[count] = &str[i];
        count++;

        // מעבר עד סוף המילה
        while (str[i] != ' ' && str[i] != '\0') i++;

        if (str[i] == ' ') // החלפת רווח ב-'\0' כדי לסיים את המילה
        {
            str[i] = '\0';
        }
    }

    words[count] = NULL; // מסמנים את סוף הרשימה

    return words;
}

void logout(char *str)
{
    while (*str == ' ') // כל עוד המצביע לתו מסוים במחרוזת הוא ריק
        str++;          // מעבירה את המצביע לתא הבא במחרוזת

    if (*str == '\0') // אם המחרוזת ריקה
        return;       // לא יקרה כלום

    // בדיקה אם המחרוזת מתחילה ב- exit.
    if (strncmp(str, "exit", 4) == 0)
    {
        str += 4; // אם המחרוזת מתחילה במילה בת 4 תווים , המצביע יידלג 4 תוים קדימה כך שהוא יצביע ישירות על מה שמגיע אחרי

        while (*str == ' ')
            str++;
        if (*str == '\0')
        {
            printf("logout\n");
            exit(0);
        }
    }
}


// כניסה לתיקיות
void cd(char **args) {
    if (args[1] == NULL) { // אם המשתמש לא הכניס נתיב
        fprintf(stderr, "cd: missing argument\n");
        return;
    }

    // אם יש יותר מדי ארגומנטים (ללא גרשיים)
    if (args[2] != NULL) {
        fprintf(stderr, "cd: too many arguments (use quotes for spaces)\n");
        return;
    }

    // שינוי הנתיב
    if (chdir(args[1]) != 0) {
        perror("cd failed");
    }
}

void cp (char ** args) {
    // בודקים כמה נתונים המשתמש הכניס לפקודה
    int size = 0 ;
    while(args[size] != NULL) {size++;}
    if (size != 3) {
        printf("too nuch or too few argument\n");
        return;
    }

    FILE *fileOrginal;
    // FILE *fileOrginal -> 1;
    FILE *fileCopy;
    // FILE *fileCopy -> 2;

    int c; // מאחסן תו אחד בכל פעם // להעתיק תווים מהמקור אל המועתק.
    fileOrginal = fopen(args[1],"rb"); //  פתיחת הקובץ המקורי לקריאה
    fileCopy = fopen(args[2], "wb"); // פתיחת הקובץ המעותק להעתקה 

    while((c = fgetc(fileOrginal) != EOF)) {fputc(c,fileCopy);} // מעתיקה כל תו שבקובץ המקורי לחדש, כל עוד הלולאה לא מגיעה לסוף הקובץ
    fclose(fileOrginal);
    fclose(fileCopy);
    printf("file copy!!\n");
}

void delete(char *str) {
    if (str == NULL || *str == '\0') {
        printf("delete\n");
        return;
    }

    // הסרת גרשיים אם קיימים בתוך אותה פונקציה
    size_t len = strlen(str);
    if (len > 1 && str[0] == '"' && str[len - 1] == '"') {
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }

    // ניסיון למחוק את הקובץ
    if (unlink(str) == 0) {
        printf("File %s deleted\n", str);
    } else {
        perror("delete error"); // מדפיס שגיאה אם המחיקה נכשלה
    }
}

// void mypipe(char ** argv1, char ** argv2) {
//     int j = 0;
//     bool foundPipe= false;
//     for (int i = 0 ; argv1[i] != NULL ; i ++){
//         if(strcmp(argv[i], "|") == 0){
//             foundPipe = true;
//             argv1[i]= NULL;
//         }

//         if(foundPipe){
//             argv2[j] = argv1[i];
//             j++;
//         }
//     }
//     argv2[j] = NULL;
// }
// public static void myPipe(string argv1 ,string argv2){
//     for(int i = 0; i <argv1.length() ; i++){  
//         while (argv1[i] != "|"){
//             continue;
//             if (argv1[i] == "|")
//                 argv2[i] = argv1[i]                
//         }
//         argv2[argv2.length()-1] = NULL;
        
//     }
// }

void move(char **args) {
    int i = 1;
    while (args[i] != NULL) {i++;}
    if (i < 3) {
        printf(stderr,"Usage: move <file1> <file2> ... <destination>\n");
        return;
    }
    char *destination = args[i-1];
    for (int j = 1 ;j < i -1 ; j++){
        char new_path[512];
        snprintf(new_path, sizeof(new_path), "%s/%s", destination, strrchr(args[j], '/') ? strrchr(args[j], '/') + 1 : args[j]);
        printf("Moved %s -> %s\n", args[j], new_path);

    }
}

void echoppend(char ** args) {
    if (args[1] == NULL || args[2] == NULL) {
        printf("Usage: echo <string> >> <pathFile>\n");
        return;
    }

    char *filename = args[2];

    // פתיחת הקובץ במצב Append (כתיבה בסוף הקובץ, יצירה אם לא קיים)
    FILE *file = fopen(filename, "a");
    if (!file) {
        perror("Error opening file");
        return;
    }

    // כתיבת התוכן לקובץ
    fprintf(file, "%s\n", args[1]);

    fclose(file);
    printf("Appended \"%s\" to %s\n", args[1], filename);
}

// void echowrite (char ** args) {
//     #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

void echowrite(char **args) {
    if (args[1] == NULL || args[2] == NULL) {
        printf("Usage: echo <string> > <pathFile>\n");
        return;
    }

    char *filename = args[2];

    // פתיחת הקובץ במצב כתיבה (אם הקובץ קיים – הוא יימחק)
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        return;
    }

    // כתיבת התוכן לקובץ
    fprintf(file, "%s\n", args[1]);

    fclose(file);
    printf("Written \"%s\" to %s\n", args[1], filename);
}

// }

void readFile (char ** args) {
    if (args[1] == NULL) {
        printf("Usage: read <pathFile>\n");
        return;
    }

    char *filename = args[1];

    // פתיחת הקובץ לקריאה
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    // קריאת הקובץ שורה-שורה והדפסתו למסך
    char line[1024]; // הגבלת אורך השורה
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}

void wordCount(char ** args) {
    if (args[1] == NULL || args[2] == NULL) {
        printf("Usage: wc <option> <pathFile>\n");
        return;
    }

    char *option = args[1];
    char *filename = args[2];

    // פתיחת הקובץ לקריאה
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    int lineCount = 0, wordCount = 0;
    char line[1024];

    // קריאת הקובץ שורה-שורה
    while (fgets(line, sizeof(line), file)) {
        lineCount++;

        // ספירת מילים בשורה
        int inWord = 0;
        for (int i = 0; line[i] != '\0'; i++) {
            if (isspace(line[i])) {
                inWord = 0;
            } else if (!inWord) {
                wordCount++;
                inWord = 1;
            }
        }
    }

    fclose(file);

    // הדפסת התוצאה בהתאם לאופציה שנבחרה
    if (strcmp(option, "-l") == 0) {
        printf("%d %s\n", lineCount, filename);
    } else if (strcmp(option, "-w") == 0) {
        printf("%d %s\n", wordCount, filename);
    } else {
        printf("Invalid option: %s\n", option);
        printf("Usage: wc -l <file>  (line count)\n");
        printf("       wc -w <file>  (word count)\n");
    }
}