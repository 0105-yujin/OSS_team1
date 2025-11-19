#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_YELLOW 6
#define COLOR_WHITE 7

void InitUI();
void Gotoxy(int x, int y);
void SetColor(int textColor, int bgColor);
void DrawLayout(char* title, char* subtitle);
void UpdateStatusBar(char* leftMsg, char* rightMsg);
void ShowPopup(char* title, char* message);
void PrintCenter(int y, char* text);

int PlayRhythmGame();
int PlaySequenceGame();

int main() {
    InitUI();

    while (1) {
        DrawLayout("NOVA APERIO", "Escape Room Project");
        PrintCenter(10, "1. GAME START");
        PrintCenter(12, "2. EXIT");
        UpdateStatusBar("Select Number", "Team Nova");

        char choice = _getch();

        if (choice == '2') {
            break;
        }
        else if (choice == '1') {
            DrawLayout("PROLOGUE", "Press Any Key...");
            PrintCenter(10, "You wake up in a locked room...");
            PrintCenter(12, "Find the clues to escape.");
            _getch();

            int result2 = PlayRhythmGame();
            if (result2 == 0) {
                ShowPopup("FAILED", "You failed the Rhythm Game.");
                continue;
            }
            ShowPopup("STAGE CLEAR", "Clue Found: [ 3 ]");

            int result3 = PlaySequenceGame();
            if (result3 == 0) {
                ShowPopup("FAILED", "You ran out of tries.");
                continue;
            }
            ShowPopup("STAGE CLEAR", "Clue Found: [ 9 ]");

            DrawLayout("THE END", "More stages coming soon...");
            _getch();
        }
    }

    SetColor(COLOR_WHITE, COLOR_BLACK);
    system("cls");
    return 0;
}

void InitUI() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    SetConsoleTitle("NOVA APERIO: Escape Room");
    
    system("mode con: cols=80 lines=25");
}

void Gotoxy(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void SetColor(int textColor, int bgColor) {
    int color = textColor + (bgColor * 16);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void DrawLayout(char* title, char* subtitle) {
    system("cls");

    SetColor(COLOR_GREEN, COLOR_BLACK);
    
    for (int x = 0; x < 80; x++) {
        Gotoxy(x, 0); printf("=");
        Gotoxy(x, 24); printf("=");
    }
    
    for (int y = 0; y < 25; y++) {
        Gotoxy(0, y); printf("|");
        Gotoxy(79, y); printf("|");
    }

    SetColor(COLOR_WHITE, COLOR_BLACK);
    Gotoxy(2, 1); 
    printf("[ NOVA APERIO System ]");
    
    PrintCenter(2, title);

    if (subtitle != NULL) {
        SetColor(COLOR_YELLOW, COLOR_BLACK);
        PrintCenter(3, subtitle);
    }

    SetColor(COLOR_GREEN, COLOR_BLACK);
    Gotoxy(1, 4);
    for(int i=1; i<79; i++) printf("-");

    SetColor(COLOR_WHITE, COLOR_BLACK);
}

void UpdateStatusBar(char* leftMsg, char* rightMsg) {
    SetColor(COLOR_WHITE, COLOR_BLACK);
    
    Gotoxy(2, 23);
    printf("                                                                            ");

    if (leftMsg != NULL) {
        Gotoxy(2, 23);
        printf("%s", leftMsg);
    }

    if (rightMsg != NULL) {
        int len = 0;
        while (rightMsg[len] != '\0') len++;
        Gotoxy(78 - len, 23);
        printf("%s", rightMsg);
    }
}

void ShowPopup(char* title, char* message) {
    int width = 40;
    int height = 10;
    int startX = (80 - width) / 2;
    int startY = (25 - height) / 2;

    SetColor(COLOR_WHITE, COLOR_BLUE);

    for (int y = startY; y < startY + height; y++) {
        Gotoxy(startX, y);
        for (int x = 0; x < width; x++) {
            printf(" ");
        }
    }

    for (int x = startX; x < startX + width; x++) {
        Gotoxy(x, startY); printf("-");
        Gotoxy(x, startY + height - 1); printf("-");
    }

    int titleLen = 0;
    while (title[titleLen] != '\0') titleLen++;
    Gotoxy(startX + (width - titleLen) / 2, startY + 1);
    printf("[%s]", title);

    int msgLen = 0;
    while (message[msgLen] != '\0') msgLen++;
    Gotoxy(startX + (width - msgLen) / 2, startY + 4);
    printf("%s", message);

    Gotoxy(startX + (width - 20) / 2, startY + 8);
    printf("Press Any Key...");

    _getch();

    SetColor(COLOR_WHITE, COLOR_BLACK);
    system("cls"); 
}

void PrintCenter(int y, char* text) {
    int len = 0;
    while (text[len] != '\0') len++;
    int x = (80 - len) / 2;
    Gotoxy(x, y);
    printf("%s", text);
}

int PlayRhythmGame() {
    DrawLayout("STAGE 2: RHYTHM GAME", "Press Space when '>' meets '<'");
    UpdateStatusBar("Score: 0", "Space: HIT | Q: Quit");

    int score = 0;
    int combo = 0;
    
    for(int i=3; i>0; i--) {
        char countStr[10];
        sprintf_s(countStr, sizeof(countStr), "%d", i);
        PrintCenter(10, countStr);
        Sleep(1000);
    }
    PrintCenter(10, "START!");
    Sleep(500);
    PrintCenter(10, "      "); 

    for (int i = 0; i < 10; i++) {
        for (int x = 10; x < 40; x += 2) {
            Gotoxy(x, 12); printf(">");      
            Gotoxy(80-x, 12); printf("<");   
            Gotoxy(40, 12); printf("[ ]");   
            Sleep(50);
            Gotoxy(x, 12); printf(" ");
            Gotoxy(80-x, 12); printf(" ");
        }
        Gotoxy(40, 12); printf("[O]"); 
        
        int hit = 0;
        for(int w=0; w<20; w++) {
            if (_kbhit()) {
                char key = _getch();
                if (key == ' ') { hit = 1; break; } 
                if (key == 'q') return 0; 
            }
            Sleep(10);
        }

        if (hit) {
            score += 100;
            combo++;
            UpdateStatusBar("HIT!", "NICE TIMING!");
        } else {
            combo = 0;
            UpdateStatusBar("MISS...", "Focus!");
        }

        char scoreText[30];
        sprintf_s(scoreText, sizeof(scoreText), "Score: %d | Combo: %d", score, combo);
        UpdateStatusBar(scoreText, "Space: HIT");
        Sleep(500);
        Gotoxy(40, 12); printf("   "); 
    }

    if (score >= 500) return 1;
    else return 0;
}

int PlaySequenceGame() {
    srand((unsigned int)time(NULL));

    int base_seq[5] = { 1, 4, 3, 5, 2 };
    int map[6]; 
    for (int i = 1; i <= 5; i++) map[i] = i;
    
    for (int i = 5; i >= 2; i--) {
        int j = rand() % i + 1;
        int tmp = map[i]; map[i] = map[j]; map[j] = tmp;
    }

    int answer[5];
    for (int i = 0; i < 5; i++) {
        answer[i] = map[base_seq[i]];
    }

    int order[5] = { 0, 1, 2, 3, 4 };
    for (int i = 4; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = order[i]; order[i] = order[j]; order[j] = tmp;
    }

    DrawLayout("STAGE 3: LOGIC PUZZLE", "Analyze hints and find the order.");
    UpdateStatusBar("Input: 1 2 3 4 5", "Focus on logic");

    int startY = 7;
    Gotoxy(5, startY - 2); printf("[ HINTS ]");
    
    for (int k = 0; k < 5; k++) {
        Gotoxy(5, startY + (k * 2)); 
        printf("%d. ", k + 1);
        switch (order[k]) {
        case 0: printf("Number %d is at the 1st position.", map[1]); break;
        case 1: printf("Number %d is immediately before %d.", map[4], map[3]); break;
        case 2: printf("Number %d comes before %d.", map[3], map[5]); break;
        case 3: printf("Number %d is immediately before %d.", map[5], map[2]); break;
        case 4: printf("Number %d comes before %d.", map[1], map[5]); break;
        }
    }

    int input[5];
    int tries = 3; 

    while (tries > 0) {
        char msg[30];
        sprintf_s(msg, sizeof(msg), "Tries Left: %d", tries);
        UpdateStatusBar(msg, "Format: 1 2 3 4 5");

        Gotoxy(15, 20);
        printf("Enter Code (e.g. 4 2 3 1 5):                   "); 
        Gotoxy(44, 20); 

        if (scanf_s("%d %d %d %d %d", &input[0], &input[1], &input[2], &input[3], &input[4]) != 5) {
            while (getchar() != '\n');
            ShowPopup("ERROR", "Invalid Format!");
            DrawLayout("STAGE 3: LOGIC PUZZLE", "Analyze hints and find the order.");
            
            Gotoxy(5, startY - 2); printf("[ HINTS ]");
            for (int k = 0; k < 5; k++) {
                Gotoxy(5, startY + (k * 2)); 
                printf("%d. ", k + 1);
                switch (order[k]) {
                case 0: printf("Number %d is at the 1st position.", map[1]); break;
                case 1: printf("Number %d is immediately before %d.", map[4], map[3]); break;
                case 2: printf("Number %d comes before %d.", map[3], map[5]); break;
                case 3: printf("Number %d is immediately before %d.", map[5], map[2]); break;
                case 4: printf("Number %d comes before %d.", map[1], map[5]); break;
                }
            }
            continue;
        }
        while (getchar() != '\n'); 

        int correct = 1;
        for (int i = 0; i < 5; i++) {
            if (input[i] != answer[i]) { correct = 0; break; }
        }

        if (correct) return 1;
        else {
            tries--;
            ShowPopup("WRONG", "Incorrect sequence.");
            DrawLayout("STAGE 3: LOGIC PUZZLE", "Analyze hints and find the order.");
            Gotoxy(5, startY - 2); printf("[ HINTS ]");
            for (int k = 0; k < 5; k++) {
                Gotoxy(5, startY + (k * 2)); 
                printf("%d. ", k + 1);
                switch (order[k]) {
                case 0: printf("Number %d is at the 1st position.", map[1]); break;
                case 1: printf("Number %d is immediately before %d.", map[4], map[3]); break;
                case 2: printf("Number %d comes before %d.", map[3], map[5]); break;
                case 3: printf("Number %d is immediately before %d.", map[5], map[2]); break;
                case 4: printf("Number %d comes before %d.", map[1], map[5]); break;
                }
            }
        }
    }
    return 0;
}