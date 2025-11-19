#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

// --- 색상 상수 ---
#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_YELLOW 6
#define COLOR_WHITE 7

// --- 함수 선언 ---
void InitUI();
void Gotoxy(int x, int y);
void SetColor(int textColor, int bgColor);
void DrawLayout(char* title, char* subtitle);
void UpdateStatusBar(char* leftMsg, char* rightMsg);
void ShowPopup(char* title, char* message);
void PrintCenter(int y, char* text);

int PlayRhythmGame();
int PlaySequenceGame();

// --- 메인 함수 ---
int main() {
    // chcp 명령어 삭제 (기본 설정 유지)

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

            // 리듬 게임 실행
            int result2 = PlayRhythmGame();

            // 리듬 게임 종료 후 화면 정리
            InitUI();

            if (result2 == 0) {
                ShowPopup("FAILED", "You failed the Rhythm Game.");
                continue;
            }
            ShowPopup("STAGE CLEAR", "Clue Found: [ 3 ]");

            // 순서 게임 실행
            int result3 = PlaySequenceGame();

            InitUI(); // 게임 후 화면 정리

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

// --- 기본 UI 함수 ---

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
    for (int i = 1; i < 79; i++) printf("-");

    SetColor(COLOR_WHITE, COLOR_BLACK);
}

void UpdateStatusBar(char* leftMsg, char* rightMsg) {
    SetColor(COLOR_WHITE, COLOR_BLACK);

    Gotoxy(2, 23);
    printf("                                                                               ");

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

// ============================================================================
// [새로 작성된 리듬 게임] - 복잡한 버퍼링 제거, Gotoxy 방식 사용
// ============================================================================

#define R_LANE_START_X 10
#define R_LANE_WIDTH 6
#define R_JUDGE_LINE_Y 20
#define R_SPEED_DIVIDER 100 // 떨어지는 속도 조절

typedef struct {
    long targetTime; // 판정선에 닿아야 하는 시간
    int line;        // 0:D, 1:F, 2:J, 3:K
    int judged;      // 판정 여부
    int prevY;       // 이전 프레임의 Y 위치 (지우기 용도)
} Note;

// 시간 측정 함수
long GetTick() {
    return (long)clock();
}

// 리듬 게임 메인
int PlayRhythmGame() {
    system("cls"); // 화면 깨끗하게

    // 1. 노트 데이터 설정 (시간, 라인)
    Note notes[] = {
        {2000, 0, 0, -1}, {3000, 1, 0, -1}, {4000, 2, 0, -1}, {5000, 3, 0, -1},
        {6000, 0, 0, -1}, {6500, 1, 0, -1}, {7000, 0, 0, -1}, {7500, 3, 0, -1}
    };
    int noteCount = sizeof(notes) / sizeof(Note);

    // 2. UI 그리기 (정적인 부분)
    SetColor(COLOR_CYAN, COLOR_BLACK);
    PrintCenter(2, "--- STAGE 2: RHYTHM ---");
    PrintCenter(4, "KEYS: [D] [F] [J] [K]");

    // 레인 그리기
    for (int y = 5; y <= 22; y++) {
        for (int i = 0; i < 4; i++) {
            Gotoxy(R_LANE_START_X + (i * R_LANE_WIDTH), y); printf("|");
            Gotoxy(R_LANE_START_X + (i * R_LANE_WIDTH) + R_LANE_WIDTH, y); printf("|");
        }
    }

    // 판정선 그리기
    SetColor(COLOR_YELLOW, COLOR_BLACK);
    Gotoxy(R_LANE_START_X, R_JUDGE_LINE_Y);
    printf("-------------------------");

    SetColor(COLOR_WHITE, COLOR_BLACK);
    Gotoxy(R_LANE_START_X + 2, R_JUDGE_LINE_Y + 1); printf("D");
    Gotoxy(R_LANE_START_X + 2 + R_LANE_WIDTH, R_JUDGE_LINE_Y + 1); printf("F");
    Gotoxy(R_LANE_START_X + 2 + R_LANE_WIDTH * 2, R_JUDGE_LINE_Y + 1); printf("J");
    Gotoxy(R_LANE_START_X + 2 + R_LANE_WIDTH * 3, R_JUDGE_LINE_Y + 1); printf("K");

    // 카운트다운
    for (int i = 3; i > 0; i--) {
        char buf[10]; sprintf(buf, "%d...", i);
        PrintCenter(10, buf);
        Sleep(1000);
    }
    PrintCenter(10, "      "); // 지우기

    // 3. 게임 루프
    long startTime = GetTick();
    int score = 0;
    int combo = 0;
    int maxCombo = 0;
    int gameRunning = 1;
    char msg[50] = "Ready";

    while (gameRunning) {
        long currentTime = GetTick() - startTime;
        int allFinished = 1;

        // 입력 처리
        if (_kbhit()) {
            int key = _getch();
            int line = -1;
            if (key == 'd' || key == 'D') line = 0;
            else if (key == 'f' || key == 'F') line = 1;
            else if (key == 'j' || key == 'J') line = 2;
            else if (key == 'k' || key == 'K') line = 3;
            else if (key == 'q' || key == 'Q') { gameRunning = 0; break; }

            if (line != -1) {
                // 판정 로직
                int hitIdx = -1;
                long minDiff = 9999;

                for (int i = 0; i < noteCount; i++) {
                    if (notes[i].line == line && !notes[i].judged) {
                        long diff = abs(currentTime - notes[i].targetTime);
                        if (diff < 400) { // 0.4초 이내면 판정 범위
                            if (diff < minDiff) {
                                minDiff = diff;
                                hitIdx = i;
                            }
                        }
                    }
                }

                if (hitIdx != -1) {
                    notes[hitIdx].judged = 1;

                    // 노트 지우기 (잔상 제거)
                    if (notes[hitIdx].prevY > 0 && notes[hitIdx].prevY < 24) {
                        Gotoxy(R_LANE_START_X + (notes[hitIdx].line * R_LANE_WIDTH) + 2, notes[hitIdx].prevY);
                        printf("  ");
                    }

                    if (minDiff < 100) {
                        score += 100; combo++; strcpy(msg, "PERFECT!");
                    }
                    else if (minDiff < 250) {
                        score += 50; combo++; strcpy(msg, "GOOD    ");
                    }
                    else {
                        combo = 0; strcpy(msg, "BAD     ");
                    }
                    if (combo > maxCombo) maxCombo = combo;
                }
            }
        }

        // 노트 이동 및 그리기
        for (int i = 0; i < noteCount; i++) {
            if (notes[i].judged) continue;

            allFinished = 0;
            long diff = notes[i].targetTime - currentTime;

            // 화면 밖(아래)으로 나갔는지 확인 (Miss)
            if (diff < -300) {
                notes[i].judged = 1;
                combo = 0;
                strcpy(msg, "MISS    ");

                // 마지막 위치 지우기
                if (notes[i].prevY > 0) {
                    Gotoxy(R_LANE_START_X + (notes[i].line * R_LANE_WIDTH) + 2, notes[i].prevY);
                    printf("  ");
                }
                continue;
            }

            // Y 좌표 계산 (판정선 Y - 남은시간비례)
            int y = R_JUDGE_LINE_Y - (int)(diff / R_SPEED_DIVIDER);

            // 화면 그리기 (이전 위치 지우고 새 위치 그림)
            if (y != notes[i].prevY) {
                // 이전 위치 지우기
                if (notes[i].prevY >= 5 && notes[i].prevY < 24 && notes[i].prevY != R_JUDGE_LINE_Y) {
                    Gotoxy(R_LANE_START_X + (notes[i].line * R_LANE_WIDTH) + 2, notes[i].prevY);
                    printf("  ");
                }

                // 새 위치 그리기 (화면 범위 내일 때만)
                if (y >= 5 && y < 24) {
                    Gotoxy(R_LANE_START_X + (notes[i].line * R_LANE_WIDTH) + 2, y);
                    SetColor(COLOR_CYAN, COLOR_BLACK);
                    printf("●"); // 노트 모양
                }
                notes[i].prevY = y;
            }
        }

        // 점수판 업데이트
        SetColor(COLOR_WHITE, COLOR_BLACK);
        Gotoxy(50, 10); printf("SCORE : %d  ", score);
        Gotoxy(50, 12); printf("COMBO : %d  ", combo);
        Gotoxy(50, 14); printf("%s      ", msg);

        if (allFinished) {
            Sleep(1000);
            break;
        }

        Sleep(30); // 프레임 조절
    }

    return (score >= 300) ? 1 : 0;
}

// ============================================================================
// [기존 순서 게임] - 변경 없음
// ============================================================================
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
        printf("Enter Code (e.g. 4 2 3 1 5):                       ");
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
        }
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