#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <string.h>

#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_YELLOW 6
#define COLOR_WHITE 7

// 함수 원형 선언
void InitUI();
void Gotoxy(int x, int y);
void SetColor(int textColor, int bgColor);
void DrawLayout(char* title, char* subtitle);
void UpdateStatusBar(char* leftMsg, char* rightMsg);
void ShowPopup(char* title, char* message);
void PrintCenter(int y, char* text);

int PlayCardGame();
int PlayRhythmGame();
int PlaySequenceGame();
int PlayMemoryGame();
int PlayBossGame();

long GetTick() { return (long)clock(); }

int main() {
    
    system("cls");
    InitUI();
    srand((unsigned int)time(NULL));

    while (1) {
        DrawLayout(" ESC_ TEAM ", "방탈출 프로젝트");
        PrintCenter(10, "1. 게임 시작");

        PrintCenter(12, "2. 종료");
        UpdateStatusBar("번호를 선택하세요", "팀 ESC");

        char choice = _getch();

        if (choice == '2') {
            break;
        }
        else if (choice == '1') {

            DrawLayout("프롤로그", "아무 키나 누르세요...");
            PrintCenter(10, "당신은 잠긴 방에서 깨어납니다...");
            PrintCenter(12, "단서를 찾아 탈출하세요.");
            _getch();

            if (PlayCardGame() == 0) {
                ShowPopup("실패", "게임 오버 (1단계)");
                continue;
            }
            ShowPopup("스테이지 클리어", "첫 번째 단서 획득!");

            if (PlayRhythmGame() == 0) {
                ShowPopup("실패", "게임 오버 (2단계)");
                continue;
            }
            ShowPopup("스테이지 클리어", "두 번째 단서 획득!");

            if (PlaySequenceGame() == 0) {
                ShowPopup("실패", "게임 오버 (3단계)");
                continue;
            }
            ShowPopup("스테이지 클리어", "세 번째 단서 획득!");

            if (PlayMemoryGame() == 0) {
                ShowPopup("실패", "게임 오버 (4단계)");
                continue;
            }
            ShowPopup("스테이지 클리어", "네 번째 단서 획득!");


            DrawLayout("최종 스테이지", "보스를 피해 아이템을 모으세요 (q: 중단)");
            PrintCenter(10, "Stage 5: 추격전 시작");
            _getch();

            if (PlayBossGame() == 0) {
                ShowPopup("실패", "게임 오버 (5단계)");
                continue;
            }
            ShowPopup("스테이지 클리어", "보스를 격퇴했습니다!");

            DrawLayout("축하합니다!", "모든 스테이지를 완료했습니다.");
            PrintCenter(10, "To Be Continued...");
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

    SetConsoleTitle("ESC: 방탈출 게임");
    system("mode con: cols=80 lines=25");
}

void Gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void SetColor(int textColor, int bgColor) {
    int color = textColor + (bgColor * 16);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void DrawLayout(char* title, char* subtitle) {
    system("cls");
    SetColor(COLOR_GREEN, COLOR_BLACK);
    for (int x = 0; x < 80; x++) { Gotoxy(x, 0); printf("="); Gotoxy(x, 24); printf("="); }
    for (int y = 0; y < 25; y++) { Gotoxy(0, y); printf("|"); Gotoxy(79, y); printf("|"); }
    SetColor(COLOR_WHITE, COLOR_BLACK);
    Gotoxy(2, 1); printf("[ ESC 시스템 ]");
    PrintCenter(2, title);
    if (subtitle != NULL) { SetColor(COLOR_YELLOW, COLOR_BLACK); PrintCenter(3, subtitle); }
    SetColor(COLOR_GREEN, COLOR_BLACK);
    Gotoxy(1, 4); for (int i = 1; i < 79; i++) printf("-");
    SetColor(COLOR_WHITE, COLOR_BLACK);
}

void UpdateStatusBar(char* leftMsg, char* rightMsg) {
    SetColor(COLOR_WHITE, COLOR_BLACK);
    Gotoxy(2, 23); printf("                                                                            ");
    if (leftMsg != NULL) { Gotoxy(2, 23); printf("%s", leftMsg); }
    if (rightMsg != NULL) {
        int len = 0; while (rightMsg[len] != '\0') len++;
        Gotoxy(78 - len, 23); printf("%s", rightMsg);
    }
}

void ShowPopup(char* title, char* message) {
    int w = 40, h = 10, sx = (80 - w) / 2, sy = (25 - h) / 2;
    SetColor(COLOR_WHITE, COLOR_BLUE);
    for (int y = sy; y < sy + h; y++) { Gotoxy(sx, y); for (int x = 0; x < w; x++) printf(" "); }
    for (int x = sx; x < sx + w; x++) { Gotoxy(x, sy); printf("-"); Gotoxy(x, sy + h - 1); printf("-"); }
    int tLen = 0; while (title[tLen]) tLen++; Gotoxy(sx + (w - tLen) / 2, sy + 1); printf("[%s]", title);
    int mLen = 0; while (message[mLen]) mLen++; Gotoxy(sx + (w - mLen) / 2, sy + 4); printf("%s", message);
    Gotoxy(sx + (w - 18) / 2, sy + 8); printf("아무 키나 누르세요...");
    _getch();
    SetColor(COLOR_WHITE, COLOR_BLACK);
    system("cls");
}

void PrintCenter(int y, char* text) {
    int len = 0; while (text[len] != '\0') len++;
    int x = (80 - len) / 2;
    Gotoxy(x, y); printf("%s", text);
}

int PlayCardGame() {
    DrawLayout("스테이지 1: 카드 짝 맞추기", "같은 숫자의 카드를 3쌍 찾으세요.");

    int cards[10] = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
    int revealed[10] = { 0 };
    int matches = 0;
    int tries = 10;

    for (int i = 0; i < 20; i++) {
        int a = rand() % 10; int b = rand() % 10;
        int t = cards[a]; cards[a] = cards[b]; cards[b] = t;
    }

    while (tries > 0 && matches < 3) {
        char buf[50]; sprintf(buf, "남은 기회: %d | 찾은 쌍: %d/3", tries, matches);
        UpdateStatusBar(buf, "번호 2개를 입력하세요 (0-9)");

        for (int i = 0; i < 10; i++) {
            Gotoxy(15 + (i * 5), 10);
            if (revealed[i]) { SetColor(COLOR_CYAN, COLOR_BLACK); printf("[%d]", cards[i]); }
            else { SetColor(COLOR_WHITE, COLOR_BLACK); printf("[?]"); }
            Gotoxy(15 + (i * 5), 11); printf(" %d ", i);
        }
        SetColor(COLOR_WHITE, COLOR_BLACK);

        Gotoxy(20, 15); printf("첫 번째 카드 (0-9): ");
        int first = _getch() - '0';
        if (first < 0 || first > 9 || revealed[first]) continue;
        Gotoxy(15 + (first * 5), 10); printf("[%d]", cards[first]);

        Gotoxy(20, 16); printf("두 번째 카드 (0-9): ");
        int second = _getch() - '0';
        if (second < 0 || second > 9 || first == second || revealed[second]) continue;
        Gotoxy(15 + (second * 5), 10); printf("[%d]", cards[second]);

        Sleep(800);

        if (cards[first] == cards[second]) {
            revealed[first] = 1; revealed[second] = 1;
            matches++;
            ShowPopup("성공!", "짝을 찾았습니다.");
        }
        else {
            tries--;
        }
        DrawLayout("스테이지 1: 카드 짝 맞추기", "같은 숫자의 카드를 3쌍 찾으세요.");
    }
    return (matches >= 3) ? 1 : 0;
}

#define R_LANE_START_X 10
#define R_LANE_WIDTH 6
#define R_JUDGE_LINE_Y 20
#define R_SPEED_DIVIDER 100

typedef struct {
    long targetTime;
    int line;
    int judged;
    int prevY;
} R_Note;

int PlayRhythmGame() {
    system("cls");
    R_Note notes[] = {
        {2000, 0, 0, -1}, {3000, 1, 0, -1}, {4000, 2, 0, -1}, {5000, 3, 0, -1},
        {6000, 0, 0, -1}, {6500, 1, 0, -1}, {7000, 0, 0, -1}, {7500, 3, 0, -1}
    };
    int noteCount = sizeof(notes) / sizeof(R_Note);

    DrawLayout("스테이지 2: 리듬 게임", "타이밍에 맞춰 키를 누르세요!");
    SetColor(COLOR_CYAN, COLOR_BLACK);
    PrintCenter(4, "키: [D] [F] [J] [K]");

    for (int y = 5; y <= 22; y++) {
        for (int i = 0; i < 4; i++) {
            Gotoxy(R_LANE_START_X + (i * R_LANE_WIDTH), y); printf("|");
            Gotoxy(R_LANE_START_X + (i * R_LANE_WIDTH) + R_LANE_WIDTH, y); printf("|");
        }
    }
    SetColor(COLOR_YELLOW, COLOR_BLACK);
    Gotoxy(R_LANE_START_X, R_JUDGE_LINE_Y); printf("-------------------------");
    SetColor(COLOR_WHITE, COLOR_BLACK);
    Gotoxy(R_LANE_START_X + 2, R_JUDGE_LINE_Y + 1); printf("D");
    Gotoxy(R_LANE_START_X + 2 + R_LANE_WIDTH, R_JUDGE_LINE_Y + 1); printf("F");
    Gotoxy(R_LANE_START_X + 2 + R_LANE_WIDTH * 2, R_JUDGE_LINE_Y + 1); printf("J");
    Gotoxy(R_LANE_START_X + 2 + R_LANE_WIDTH * 3, R_JUDGE_LINE_Y + 1); printf("K");

    long startTime = GetTick();
    int score = 0, combo = 0, maxCombo = 0;
    int gameRunning = 1;
    char msg[50] = "준비";

    while (gameRunning) {
        long currentTime = GetTick() - startTime;
        int allFinished = 1;

        if (_kbhit()) {
            int key = _getch();
            int line = -1;
            if (key == 'd' || key == 'D') line = 0; else if (key == 'f' || key == 'F') line = 1;
            else if (key == 'j' || key == 'J') line = 2; else if (key == 'k' || key == 'K') line = 3;
            else if (key == 'q' || key == 'Q') { gameRunning = 0; break; }

            if (line != -1) {
                int hitIdx = -1; long minDiff = 9999;
                for (int i = 0; i < noteCount; i++) {
                    if (notes[i].line == line && !notes[i].judged) {
                        long diff = abs(currentTime - notes[i].targetTime);
                        if (diff < 400) {
                            if (diff < minDiff) { minDiff = diff; hitIdx = i; }
                        }
                    }
                }
                if (hitIdx != -1) {
                    notes[hitIdx].judged = 1;
                    if (notes[hitIdx].prevY > 0 && notes[hitIdx].prevY < 24) {
                        Gotoxy(R_LANE_START_X + (notes[hitIdx].line * R_LANE_WIDTH) + 2, notes[hitIdx].prevY); printf("  ");
                    }
                    if (minDiff < 100) { score += 100; combo++; strcpy(msg, "완벽해요!"); }
                    else if (minDiff < 250) { score += 50; combo++; strcpy(msg, "좋아요  "); }
                    else { combo = 0; strcpy(msg, "나빠요  "); }
                    if (combo > maxCombo) maxCombo = combo;
                }
            }
        }

        for (int i = 0; i < noteCount; i++) {
            if (notes[i].judged) continue;
            allFinished = 0;
            long diff = notes[i].targetTime - currentTime;

            if (diff < -300) {
                notes[i].judged = 1; combo = 0; strcpy(msg, "놓침    ");
                if (notes[i].prevY > 0) {
                    Gotoxy(R_LANE_START_X + (notes[i].line * R_LANE_WIDTH) + 2, notes[i].prevY); printf("  ");
                }
                continue;
            }
            int y = R_JUDGE_LINE_Y - (int)(diff / R_SPEED_DIVIDER);
            if (y != notes[i].prevY) {
                if (notes[i].prevY >= 5 && notes[i].prevY < 24 && notes[i].prevY != R_JUDGE_LINE_Y) {
                    Gotoxy(R_LANE_START_X + (notes[i].line * R_LANE_WIDTH) + 2, notes[i].prevY); printf("  ");
                }
                if (y >= 5 && y < 24) {
                    Gotoxy(R_LANE_START_X + (notes[i].line * R_LANE_WIDTH) + 2, y);
                    SetColor(COLOR_CYAN, COLOR_BLACK); printf("●");
                }
                notes[i].prevY = y;
            }
        }
        SetColor(COLOR_WHITE, COLOR_BLACK);
        Gotoxy(50, 10); printf("점수 : %d    ", score);
        Gotoxy(50, 12); printf("콤보 : %d    ", combo);
        Gotoxy(50, 14); printf("%s        ", msg);

        if (allFinished) { Sleep(1000); break; }
        Sleep(30);
    }
    return (score >= 300) ? 1 : 0;
}

int PlaySequenceGame() {
    int base_seq[5] = { 1, 4, 3, 5, 2 };
    int map[6]; for (int i = 1; i <= 5; i++) map[i] = i;
    for (int i = 5; i >= 2; i--) { int j = rand() % i + 1; int t = map[i]; map[i] = map[j]; map[j] = t; }
    int answer[5]; for (int i = 0; i < 5; i++) answer[i] = map[base_seq[i]];
    int order[5] = { 0, 1, 2, 3, 4 };
    for (int i = 4; i > 0; i--) { int j = rand() % (i + 1); int t = order[i]; order[i] = order[j]; order[j] = t; }

    DrawLayout("스테이지 3: 논리 퍼즐", "힌트를 분석하여 순서를 찾으세요.");
    UpdateStatusBar("입력: 1 2 3 4 5", "논리에 집중하세요");
    int startY = 7; Gotoxy(5, startY - 2); printf("[ 힌트 ]");

    for (int k = 0; k < 5; k++) {
        Gotoxy(5, startY + (k * 2)); printf("%d. ", k + 1);
        switch (order[k]) {
        case 0: printf("숫자 %d은(는) 첫 번째 위치에 있습니다.", map[1]); break;
        case 1: printf("숫자 %d은(는) %d 바로 앞에 있습니다.", map[4], map[3]); break;
        case 2: printf("숫자 %d은(는) %d 앞에 옵니다.", map[3], map[5]); break;
        case 3: printf("숫자 %d은(는) %d 바로 앞에 있습니다.", map[5], map[2]); break;
        case 4: printf("숫자 %d은(는) %d 앞에 옵니다.", map[1], map[5]); break;
        }
    }

    int input[5], tries = 3;
    while (tries > 0) {
        char msg[30]; sprintf(msg, "남은 시도: %d", tries); UpdateStatusBar(msg, "형식: 1 2 3 4 5");
        Gotoxy(15, 20); printf("코드 입력 (예: 4 2 3 1 5):                        "); Gotoxy(44, 20);
        if (scanf_s("%d %d %d %d %d", &input[0], &input[1], &input[2], &input[3], &input[4]) != 5) {
            while (getchar() != '\n');
            ShowPopup("오류", "잘못된 형식입니다!");
            continue;
        }
        while (getchar() != '\n');
        int correct = 1; for (int i = 0; i < 5; i++) if (input[i] != answer[i]) { correct = 0; break; }
        if (correct) return 1;
        else {
            tries--; ShowPopup("틀림", "잘못된 순서입니다.");
        }
    }
    return 0;
}

int PlayMemoryGame() {
    int round_lengths[] = { 3, 5, 7 };
    int total_rounds = 3;
    int sequence[20];
    char input[100];

    for (int round = 0; round < total_rounds; round++) {
        int length = round_lengths[round];
        for (int i = 0; i < length; i++) sequence[i] = rand() % 10;

        char title[50]; sprintf(title, "스테이지 4: 기억력 (라운드 %d)", round + 1);

        for (int i = 0; i < length; i++) {
            DrawLayout(title, "숫자를 기억하세요 (2초 간격으로 나타납니다)");
            char numStr[20]; sprintf(numStr, "[ %d ]", sequence[i]);

            SetColor(COLOR_CYAN, COLOR_BLACK);
            PrintCenter(12, numStr);

            Sleep(2000);

            DrawLayout(title, "...");
            Sleep(200);
        }
        SetColor(COLOR_WHITE, COLOR_BLACK);

        DrawLayout(title, "기억한 숫자를 순서대로 입력하세요.");
        UpdateStatusBar("입력 완료 후 엔터", "띄어쓰기 없이 입력 (예: 123)");

        Gotoxy(25, 12); printf("정답 입력: ");

        if (scanf_s("%s", input, 100) != 1) {
            while (getchar() != '\n'); return 0;
        }
        while (getchar() != '\n');

        int correct = 1;
        if (strlen(input) != length) correct = 0;
        else {
            for (int i = 0; i < length; i++) {
                if (input[i] != sequence[i] + '0') {
                    correct = 0; break;
                }
            }
        }

        if (correct) {
            ShowPopup("정답!", "다음 라운드로 진행합니다.");
        }
        else {
            char answerStr[50] = "정답: ";
            for (int i = 0; i < length; i++) {
                char temp[2]; sprintf(temp, "%d", sequence[i]);
                strcat(answerStr, temp);
            }
            ShowPopup("실패", answerStr);
            return 0;
        }
    }
    return 1;
}

/* =========================
   Stage 5: Boss Chase Game (오류 수정됨)
   ========================= */

   // 구조체 및 상수 전역 정의
#define BOSS_WIDTH 80
#define BOSS_HEIGHT 25
#define BOSS_WALL_CHAR "#"
#define BOSS_PLAYER_CHAR "o"
#define BOSS_ENEMY_CHAR "X"
#define BOSS_ITEM_CHAR "$"

typedef struct { int x, y; } BWall;
typedef struct { int x, y; int active; } BItem;

// 보조 함수들을 전역으로 이동
void b_set_cursor(int x, int y) { Gotoxy(x, y); }
void b_set_color(int color) { SetColor(color, COLOR_BLACK); }

int b_is_wall(int x, int y, BWall walls[], int count) {
    for (int i = 0; i < count; i++) if (x == walls[i].x && y == walls[i].y) return 1;
    return 0;
}

void b_draw_walls(BWall walls[], int count) {
    b_set_color(8);
    for (int i = 0; i < count; i++) {
        b_set_cursor(walls[i].x, walls[i].y);
        printf("%s", BOSS_WALL_CHAR);
    }
    b_set_color(COLOR_WHITE);
}

void b_draw_items(BItem items[], int count) {
    b_set_color(COLOR_YELLOW);
    for (int i = 0; i < count; i++) {
        if (items[i].active) {
            b_set_cursor(items[i].x, items[i].y);
            printf("%s", BOSS_ITEM_CHAR);
        }
    }
    b_set_color(COLOR_WHITE);
}

void b_reset_items(BItem items[], int count, BWall walls[], int wallCount, int px, int py, int ex, int ey) {
    for (int i = 0; i < count; i++) {
        int x, y, valid;
        do {
            valid = 1;
            x = rand() % BOSS_WIDTH;
            y = rand() % BOSS_HEIGHT;
            for (int w = 0; w < wallCount; w++) {
                if (walls[w].x == x && walls[w].y == y) { valid = 0; break; }
            }
            if ((x == px && y == py) || (x == ex && y == ey)) valid = 0;
        } while (!valid);
        items[i].x = x; items[i].y = y; items[i].active = 1;
    }
}

int PlayBossGame() {
    // 게임 상태 변수
    int px, py, ex, ey;
    int ch, round = 1;
    DWORD lastMoveTime;
    int baseSpeed = 300;
    int score = 0;
    const int rounds_to_win = 2; // 라운드 2회 성공하면 스테이지 클리어

    // walls
    BWall walls[] = {
        {10,3},{15,5},{20,8},{35,6},{40,10},{45,15},{60,7},{62,8},{64,9},
        {25,18},{30,20},{50,22},{12,12},{18,14},{22,16},{70,5},{72,9},{74,12}
    };
    int wallCount = sizeof(walls) / sizeof(walls[0]);

    BItem items[3];

    // 숨김 커서
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci;
    GetConsoleCursorInfo(out, &ci);
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(out, &ci);

restart_round:
    system("cls");

    // 초기 위치
    px = 5; py = 5;
    ex = 70; ey = 20;
    score = 0;

    b_reset_items(items, 3, walls, wallCount, px, py, ex, ey);
    b_draw_walls(walls, wallCount);
    b_draw_items(items, 3);

    lastMoveTime = GetTickCount();
    b_set_cursor(px, py); printf("%s", BOSS_PLAYER_CHAR);
    b_set_cursor(ex, ey); printf("%s", BOSS_ENEMY_CHAR);

    b_set_cursor(0, BOSS_HEIGHT - 1);
    printf("Round %d 시작! 보스 속도: %.2fx\n", round, (1000.0 / baseSpeed));

    while (1) {
        if (_kbhit()) {
            ch = _getch();
            if (ch == 0 || ch == 224) ch = _getch();
            int nx = px, ny = py;
            if (ch == 'w' || ch == 'W' || ch == 72) ny--;
            if (ch == 's' || ch == 'S' || ch == 80) ny++;
            if (ch == 'a' || ch == 'A' || ch == 75) nx--;
            if (ch == 'd' || ch == 'D' || ch == 77) nx++;
            if (ch == 'q' || ch == 'Q') {
                // 중단: 실패로 처리
                ci.bVisible = TRUE;
                SetConsoleCursorInfo(out, &ci);
                return 0;
            }

            if (nx < 0 || ny < 0 || nx >= BOSS_WIDTH || ny >= BOSS_HEIGHT) continue;
            if (b_is_wall(nx, ny, walls, wallCount)) continue;

            b_set_cursor(px, py); printf(" ");
            px = nx; py = ny;
            b_set_cursor(px, py); printf("%s", BOSS_PLAYER_CHAR);
        }

        // 아이템 획득
        for (int i = 0; i < 3; i++) {
            if (items[i].active && px == items[i].x && py == items[i].y) {
                items[i].active = 0;
                b_set_cursor(items[i].x, items[i].y); printf(" ");
                score++;
                b_set_cursor(0, BOSS_HEIGHT - 1);
                printf("?? 아이템 획득! (%d / 3)\n", score);
            }
        }


        if (GetTickCount() - lastMoveTime > baseSpeed) {
            int dx = 0, dy = 0;
            if (ex < px) dx = 1;
            if (ex > px) dx = -1;
            if (ey < py) dy = 1;
            if (ey > py) dy = -1;

            int nex = ex + dx;
            int ney = ey + dy;

            int onItem = 0;
            for (int i = 0; i < 3; i++) if (items[i].active && items[i].x == nex && items[i].y == ney) onItem = 1;

            if (!b_is_wall(nex, ney, walls, wallCount) && !onItem) {
                b_set_cursor(ex, ey); printf(" ");
                ex = nex; ey = ney;
                b_set_cursor(ex, ey); printf("%s", BOSS_ENEMY_CHAR);
            }

            lastMoveTime = GetTickCount();
        }


        if (abs(ex - px) < 1 && abs(ey - py) < 1) {
            b_set_cursor(0, BOSS_HEIGHT - 1);
            printf("\n?? 추격자에게 잡혔습니다! 실패...\n");
            Sleep(1200);
            ci.bVisible = TRUE;
            SetConsoleCursorInfo(out, &ci);
            return 0;
        }


        if (score == 3) {
            round++;
            if (round > rounds_to_win) {
                b_set_cursor(0, BOSS_HEIGHT - 1);
                printf("\n?? 보스전 클리어! 축하합니다.\n");
                Sleep(1000);
                ci.bVisible = TRUE;
                SetConsoleCursorInfo(out, &ci);
                return 1;
            }
            baseSpeed = (int)(baseSpeed * 0.6);
            b_set_cursor(0, BOSS_HEIGHT - 1);
            printf("\n?? 아이템 모두 획득! 다음 라운드 시작...\n");
            Sleep(1200);
            goto restart_round;
        }

        Sleep(10);
    }

    // unreachable
    ci.bVisible = TRUE;
    SetConsoleCursorInfo(out, &ci);
    return 0;
}
