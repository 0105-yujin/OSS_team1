#define _CRT_SECURE_NO_WARNINGS
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
    system("chcp 65001"); 
    system("cls"); 

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
            
            InitUI(); 

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

#define R_PERFECT 150
#define R_GOOD 250
#define R_MISS 350
#define R_LOOKAHEAD 3000
#define R_JUDGE_Y 20
#define R_LANE_0_X 10
#define R_LANE_WIDTH 5
#define R_SCREEN_W 80
#define R_SCREEN_H 25

typedef struct {
    unsigned long time_ms;
    int line;
    int judged;
} R_Note;

HANDLE hRhythmBuf[2];
int nRhythmIdx = 0;

void InitRhythmScreen() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;

    hRhythmBuf[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    hRhythmBuf[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    COORD size = { 80, 25 };
    SMALL_RECT rect = { 0, 0, 79, 24 };

    SetConsoleScreenBufferSize(hRhythmBuf[0], size);
    SetConsoleWindowInfo(hRhythmBuf[0], TRUE, &rect);
    SetConsoleScreenBufferSize(hRhythmBuf[1], size);
    SetConsoleWindowInfo(hRhythmBuf[1], TRUE, &rect);

    SetConsoleCursorInfo(hRhythmBuf[0], &cursorInfo);
    SetConsoleCursorInfo(hRhythmBuf[1], &cursorInfo);
}

void FlipRhythmScreen() {
    SetConsoleActiveScreenBuffer(hRhythmBuf[nRhythmIdx]);
    nRhythmIdx = !nRhythmIdx;
}

void ClearRhythmScreen() {
    COORD coor = { 0, 0 };
    DWORD dw;
    FillConsoleOutputCharacter(hRhythmBuf[nRhythmIdx], ' ', R_SCREEN_W * R_SCREEN_H, coor, &dw);
}

void WriteRhythmStr(int x, int y, const char* str) {
    COORD pos = { x, y };
    DWORD dw;
    SetConsoleCursorPosition(hRhythmBuf[nRhythmIdx], pos);
    WriteConsole(hRhythmBuf[nRhythmIdx], str, strlen(str), &dw, NULL);
}

void WriteRhythmFmt(int x, int y, const char* format, ...) {
    char buf[256];
    va_list args;
    va_start(args, format);
    vsprintf(buf, format, args);
    va_end(args);
    WriteRhythmStr(x, y, buf);
}

void ReleaseRhythmScreen() {
    CloseHandle(hRhythmBuf[0]);
    CloseHandle(hRhythmBuf[1]);
    SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
}

unsigned long GetTimeMs(unsigned long start) {
    return (unsigned long)clock() - start;
}

int PlayRhythmGame() {
    InitRhythmScreen();

    R_Note notes[] = {
        {2000, 0, 0}, {3000, 1, 0}, {4000, 2, 0}, {5000, 3, 0},
        {6000, 0, 0}, {6500, 1, 0}, {7000, 0, 0}, {7500, 3, 0}
    };
    int noteCount = sizeof(notes) / sizeof(R_Note);

    int score = 0;
    int combo = 0;
    int maxCombo = 0;
    char lastJudge[50] = "Ready...";

    WriteRhythmStr(5, 5, "--- STAGE 2: RHYTHM ---");
    WriteRhythmStr(5, 6, "KEYS: [d] [f] [j] [k]");
    WriteRhythmStr(5, 8, "STARTING IN 3 SEC...");
    FlipRhythmScreen();
    Sleep(3000);

    unsigned long startTime = (unsigned long)clock();
    int gameRunning = 1;

    while (gameRunning) {
        ClearRhythmScreen();
        unsigned long current = GetTimeMs(startTime);

        if (_kbhit()) {
            int ch = _getch();
            int line = -1;
            if (ch == 'd' || ch == 'D') line = 0;
            else if (ch == 'f' || ch == 'F') line = 1;
            else if (ch == 'j' || ch == 'J') line = 2;
            else if (ch == 'k' || ch == 'K') line = 3;
            else if (ch == 'q' || ch == 'Q') { gameRunning = 0; break; }

            if (line != -1) {
                int bestIdx = -1;
                unsigned long minDiff = 10000;

                for (int i = 0; i < noteCount; i++) {
                    if (notes[i].line == line && !notes[i].judged) {
                        unsigned long diff = (current > notes[i].time_ms) ? (current - notes[i].time_ms) : (notes[i].time_ms - current);
                        if (diff <= R_MISS && diff < minDiff) {
                            minDiff = diff;
                            bestIdx = i;
                        }
                    }
                }

                if (bestIdx != -1) {
                    notes[bestIdx].judged = 1;
                    if (minDiff <= R_PERFECT) {
                        score += 100; combo++;
                        sprintf(lastJudge, "PERFECT! (%dms)", (int)minDiff);
                    } else if (minDiff <= R_GOOD) {
                        score += 50; combo++;
                        sprintf(lastJudge, "GOOD (%dms)", (int)minDiff);
                    } else {
                        combo = 0;
                        sprintf(lastJudge, "MISS (%dms)", (int)minDiff);
                    }
                    if (combo > maxCombo) maxCombo = combo;
                }
            }
        }

        int unjudgedCount = 0;
        for (int i = 0; i < noteCount; i++) {
            if (!notes[i].judged) {
                unjudgedCount++;
                if (current > notes[i].time_ms + R_MISS) {
                    notes[i].judged = 1;
                    combo = 0;
                    sprintf(lastJudge, "MISS (Timeout)");
                }
            }
        }

        WriteRhythmStr(R_LANE_0_X - 2, R_JUDGE_Y, "[d]");
        WriteRhythmStr(R_LANE_0_X + R_LANE_WIDTH - 2, R_JUDGE_Y, "[f]");
        WriteRhythmStr(R_LANE_0_X + R_LANE_WIDTH * 2 - 2, R_JUDGE_Y, "[j]");
        WriteRhythmStr(R_LANE_0_X + R_LANE_WIDTH * 3 - 2, R_JUDGE_Y, "[k]");
        
        for (int x = 0; x < 40; x++) WriteRhythmStr(x, R_JUDGE_Y + 1, "-");

        for (int i = 0; i < noteCount; i++) {
            if (notes[i].judged) continue;
            long diff = (long)notes[i].time_ms - (long)current;
            if (diff < R_LOOKAHEAD && diff > -R_MISS) {
                double percent = 1.0 - ((double)diff / R_LOOKAHEAD);
                int y = (int)(percent * R_JUDGE_Y);
                int x = R_LANE_0_X + notes[i].line * R_LANE_WIDTH - 2;
                if (y >= 0 && y <= R_JUDGE_Y) WriteRhythmStr(x, y, " O ");
            }
        }

        WriteRhythmFmt(40, 2, "Score: %d", score);
        WriteRhythmFmt(40, 3, "Combo: %d (Max: %d)", combo, maxCombo);
        WriteRhythmStr(40, 5, "Judgment:");
        WriteRhythmStr(40, 6, lastJudge);
        
        FlipRhythmScreen(); 

        if (unjudgedCount == 0) {
            Sleep(1500);
            gameRunning = 0;
        }
        Sleep(10); 
    }

    ReleaseRhythmScreen(); 

    return (score >= 200) ? 1 : 0;
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

        if (correct) {
            return 1; 
        } else {
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