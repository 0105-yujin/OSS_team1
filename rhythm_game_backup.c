#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>

#define PERFECT_WINDOW 75
#define GOOD_WINDOW 150
#define MISS_WINDOW 200
#define LOOKAHEAD_MS 3000

#define GAME_HEIGHT 20
#define JUDGE_LINE_Y GAME_HEIGHT
#define LANE_0_X 10
#define LANE_WIDTH 5

typedef struct {
    unsigned long time_ms;
    int line;
    bool judged;
} Note;

int score = 0;
int combo = 0;
int maxCombo = 0;
unsigned long startTime;

unsigned long get_ms_time() {
    return GetTickCount();
}

void gotoxy(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

unsigned long getCurrentTime_ms() {
    return get_ms_time() - startTime;
}

void printJudgment(char* text) {
    gotoxy(40, 6);
    printf("                          ");
    gotoxy(40, 6);
    printf("%s", text);
}

void judgeMiss(Note* note) {
    if (note->judged) return;
    note->judged = true;
    combo = 0;
    
    char tempText[50];
    sprintf(tempText, "[%05dms] Line %d: MISS", note->time_ms, note->line);
    printJudgment(tempText);
}

void judgeHit(unsigned long hitTime_ms, int line, Note* pattern, int noteCount) {
    int bestNoteIndex = -1;
    unsigned long minDiff = 10000;

    for (int i = 0; i < noteCount; i++) {
        if (pattern[i].line == line && !pattern[i].judged) {
            unsigned long diff_check = (hitTime_ms > pattern[i].time_ms) ?
                                       (hitTime_ms - pattern[i].time_ms) :
                                       (pattern[i].time_ms - hitTime_ms);
            
            if (diff_check <= MISS_WINDOW && diff_check < minDiff) {
                minDiff = diff_check;
                bestNoteIndex = i;
            }
        }
    }

    char tempText[50];

    if (bestNoteIndex == -1) {
        sprintf(tempText, "[%05dms] Line %d: (Bad Hit)", hitTime_ms, line);
        printJudgment(tempText);
        return;
    }

    Note* targetNote = &pattern[bestNoteIndex];
    unsigned long diff = minDiff;

    if (diff <= PERFECT_WINDOW) {
        sprintf(tempText, "[%05dms] Line %d: PERFECT! (%dms)", hitTime_ms, line, (int)diff);
        score += 100;
        combo++;
    } else if (diff <= GOOD_WINDOW) {
        sprintf(tempText, "[%05dms] Line %d: Good. (%dms)", hitTime_ms, line, (int)diff);
        score += 50;
        combo++;
    } else {
        sprintf(tempText, "[%05dms] Line %d: Miss. (%dms)", hitTime_ms, line, (int)diff);
        combo = 0;
    }
    
    printJudgment(tempText);
    
    targetNote->judged = true;
    if (combo > maxCombo) {
        maxCombo = combo;
    }
}

void draw_board() {
    gotoxy(LANE_0_X - 2, JUDGE_LINE_Y); printf("[d]");
    gotoxy(LANE_0_X + LANE_WIDTH - 2, JUDGE_LINE_Y); printf("[f]");
    gotoxy(LANE_0_X + LANE_WIDTH * 2 - 2, JUDGE_LINE_Y); printf("[j]");
    gotoxy(LANE_0_X + LANE_WIDTH * 3 - 2, JUDGE_LINE_Y); printf("[k]");
    
    for (int x = 0; x < 40; x++) {
        gotoxy(x, JUDGE_LINE_Y + 1); printf("-");
    }

    for (int y = 0; y < JUDGE_LINE_Y; y++) {
        gotoxy(LANE_0_X - 3, y); printf("|");
        gotoxy(LANE_0_X + LANE_WIDTH * 4 - 3, y); printf("|");
    }

    gotoxy(40, 2); printf("Score: %d", score);
    gotoxy(40, 3); printf("Combo: %d", combo);
    
    gotoxy(40, 5); printf("Last Judgment:");
    
    gotoxy(40, 8); printf("Time: %.1f s", (float)getCurrentTime_ms() / 1000.0);
}

void draw_notes(Note* pattern, int noteCount, unsigned long currentTime_ms) {
    for (int i = 0; i < noteCount; i++) {
        if (pattern[i].judged) continue;

        long time_diff = (long)pattern[i].time_ms - (long)currentTime_ms;

        if (time_diff < LOOKAHEAD_MS && time_diff > -MISS_WINDOW) {
            
            double y_percent = 1.0 - ((double)time_diff / LOOKAHEAD_MS);
            int y = (int)(y_percent * JUDGE_LINE_Y);
            
            int x = LANE_0_X + pattern[i].line * LANE_WIDTH - 2;

            if (y >= 0 && y <= JUDGE_LINE_Y + 1) {
                gotoxy(x, y); printf(" O ");
            }
        }
    }
}

int main() {
    
    Note pattern[] = {
        {2000, 0, false},
        {3000, 1, false},
        {4000, 2, false},
        {5000, 3, false},
        {6000, 0, false},
        {6500, 1, false},
        {7000, 0, false},
    };
    int noteCount = sizeof(pattern) / sizeof(Note);

    hideCursor();

    printf("--- 리듬 게임 시작 ---\n");
    printf("키: [d] [f] [j] [k] (종료: q)\n");
    printf("3초 후 시작합니다...\n");
    Sleep(3000);

    startTime = get_ms_time();
    unsigned long currentTime_ms = 0;
    
    bool gameRunning = true;

    while (gameRunning) {
        system("cls");
        
        currentTime_ms = getCurrentTime_ms();

        if (_kbhit()) {
            int ch = _getch();
            
            int line = -1;
            if (ch == 'd') line = 0;
            else if (ch == 'f') line = 1;
            else if (ch == 'j') line = 2;
            else if (ch == 'k') line = 3;
            else if (ch == 'q') {
                gameRunning = false;
                break;
            }

            if (line != -1) {
                judgeHit(currentTime_ms, line, pattern, noteCount);
            }
        }

        bool allJudged = true;
        
        for (int i = 0; i < noteCount; i++) {
            if (!pattern[i].judged) {
                allJudged = false;
                
                if (currentTime_ms > pattern[i].time_ms + MISS_WINDOW) {
                    judgeMiss(&pattern[i]);
                }
            }
        }

        draw_board();
        draw_notes(pattern, noteCount, currentTime_ms);

        if (allJudged) {
            gameRunning = false;
            Sleep(2000);
        }

        Sleep(50);
    }

    system("cls");
    printf("\n--- 게임 종료 ---\n");
    if (combo > maxCombo) {
        maxCombo = combo;
    }
    
    bool isPass = (score >= 200);

    printf("최종 점수: %d\n", score);
    printf("최대 콤보: %d\n", maxCombo);
    printf("통과 여부: %s\n", isPass ? "Pass" : "Fail");
    
    printf("\n종료하려면 아무 키나 누르세요...");
    _getch();

    return 0;
}
