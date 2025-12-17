#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <stdbool.h> 

#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_YELLOW 6
#define COLOR_WHITE 7

#define R 2 
#define C 5 
#define PAIRS 5 
#define MAX_ATTEMPTS 10

#define ANSI_ITALIC_ON  "\x1b[3m"
#define ANSI_RESET      "\x1b[0m"
#define ANSI_BOLD_ON    "\x1b[1m"
#define ANSI_BLINK_ON   "\x1b[5m"
#define ANSI_CYAN_ON    "\x1b[36m"
#define ANSI_RED_ON     "\x1b[31m"
#define ANSI_YELLOW_ON  "\x1b[33m"
#define ANSI_LIGHT_GRAY_ON "\x1b[90m"

#define PANEL_X 22
#define PANEL_Y 6
#define PATTERN_Y 12

int FINAL_CODE[5];

void InitUI();
void Gotoxy(int x, int y);
void SetColor(int textColor, int bgColor);
void DrawLayout(char* title, char* subtitle);
void UpdateStatusBar(char* leftMsg, char* rightMsg);
void ShowPopup(char* title, char* message);
void PrintCenter(int y, char* text);

void PrintTypewriter(int y, char* message, int delay_ms);
void DrawLockPanel_Stage1(int x, int y, char* status_msg, bool active);
void BlinkScenePanel(int y, char* text, int cycles, int speed_ms);
void BlinkLoop(int y, char* text);
void FadeInOutText(int y, char* text, int step_delay_ms);
void DrawSparkEffect(int count, int duration_ms);

long GetTick() { return (long)clock(); }

void clear_input_buffer(void);
void init_cards(char cards[R][C], bool matched[R][C]);
void draw_board(char cards[R][C], bool matched[R][C], int attempts, int score);
void get_selection(char cards[R][C], bool matched[R][C], int* r, int* c);
void wait_for_enter(void);

void PrologSequence();
void StartSequence_Stage1();
void StartSequence_Stage2();
void StartSequence_Stage3();
void StartSequence_Stage4();
void StartSequence_Stage5();

void ClearSequence_Stage1();
void ClearSequence_Stage2();
void ClearSequence_Stage3();
void ClearSequence_Stage4();
void Epilogue();

int PlayCardGame();
int PlayRhythmGame();
int PlaySequenceGame();
int PlayMemoryGame();
int PlayBossGame(int current_round); // 매개변수 추가 (원래 코드 구조에 맞춤)
int ESCape();
int main() {

    system("cls");
    InitUI();
    srand((unsigned int)time(NULL));

    for (int i = 0; i < 5; i++) {
        FINAL_CODE[i] = rand() % 10;
    }

    char msgBuf[100];

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

            PrologSequence();
            StartSequence_Stage1();

            if (PlaySequenceGame() == 0) {
                ShowPopup("실패", "게임 오버 (1단계)");
                continue;
            }

            ClearSequence_Stage1();

            sprintf(msgBuf, "첫 번째 단서 획득: [ %d ]", FINAL_CODE[0]);
            ShowPopup("스테이지 클리어", msgBuf);

            StartSequence_Stage2();

            if (PlayMemoryGame() == 0) {
                ShowPopup("실패", "게임 오버 (2단계)");
                continue;
            }

            ClearSequence_Stage2();

            sprintf(msgBuf, "두 번째 단서 획득: [ %d ]", FINAL_CODE[1]);
            ShowPopup("스테이지 클리어", msgBuf);

            StartSequence_Stage3();

            if (PlayRhythmGame() == 0) {
                ShowPopup("실패", "게임 오버 (3단계)");
                continue;
            }

            ClearSequence_Stage3();

            sprintf(msgBuf, "세 번째 단서 획득: [ %d ]", FINAL_CODE[2]);
            ShowPopup("스테이지 클리어", msgBuf);

            StartSequence_Stage4();

            if (PlayCardGame() == 0) {
                ShowPopup("실패", "게임 오버 (4단계)");
                continue;
            }

            ClearSequence_Stage4();

            sprintf(msgBuf, "네 번째 단서 획득: [ %d ]", FINAL_CODE[3]);
            ShowPopup("스테이지 클리어", msgBuf);

            StartSequence_Stage5();

            // PlayBossGame은 3라운드 클리어 시 성공으로 간주
            int boss_success = 1;
            for (int round = 1; round <= 3; round++) {
                if (PlayBossGame(round) == 0) {
                    boss_success = 0;
                    break;
                }
            }

            if (boss_success == 0) {
                ShowPopup("실패", "게임 오버 (5단계)");
                continue;
            }

            sprintf(msgBuf, "마지막 단서 획득: [ %d ]", FINAL_CODE[4]);
            ShowPopup("스테이지 클리어", msgBuf);
            Sleep(1500);

            DrawLayout("최종 관문", "도어락 비밀번호를 입력하세요");
            PrintCenter(8, "획득한 단서를 순서대로 입력하세요.");
            UpdateStatusBar("5자리 숫자 입력", "엔터 키로 입력");

            Gotoxy(35, 12);
            printf("비밀번호: ");

            int inputCode;
            if (scanf_s("%d", &inputCode) == 1) {
                int correctCode = FINAL_CODE[0] * 10000 +
                    FINAL_CODE[1] * 1000 +
                    FINAL_CODE[2] * 100 +
                    FINAL_CODE[3] * 10 +
                    FINAL_CODE[4];

                if (inputCode == correctCode) {
                    ShowPopup("잠금 해제", "철컥! 문이 열립니다...");
                    ESCape();
                    DrawLayout("탈출 성공!", "축하합니다!");
                    SetColor(COLOR_YELLOW, COLOR_BLACK);
                    PrintCenter(6, " #   #  ###     ##     ##    ###     ##    #  #  ");
                    PrintCenter(7, " ## ##   #     #  #   #  #    #     #  #   ## #  ");
                    PrintCenter(8, " # # #   #      #      #      #     #  #   # ##  ");
                    PrintCenter(9, " # # #   #       #      #     #     #  #   #  #  ");
                    PrintCenter(10, " #   #   #     #  #   #  #    #     #  #   #  #  ");
                    PrintCenter(11, " #   #  ###     ##     ##    ###     ##    #  #  ");

                    PrintCenter(13, "  ##     ##    #   #  ###    #      ####   #####  ####  ");
                    PrintCenter(14, " #  #   #  #   ## ##  #  #   #      #        #    #     ");
                    PrintCenter(15, " #      #  #   # # #  #  #   #      ###      #    ###   ");
                    PrintCenter(16, " #      #  #   # # #  ###    #      #        #    #     ");
                    PrintCenter(17, " #  #   #  #   #   #  #      #      #        #    #     ");
                    PrintCenter(18, "  ##     ##    #   #  #      ####   ####     #    ####  ");
                    
                    SetColor(COLOR_WHITE, COLOR_BLACK);
                    PrintCenter(20, "팀원: 홍유진, 김장혁, 이동호, 팽선우");
                    PrintCenter(22, "플레이해주셔서 감사합니다.");
                    _getch();
                }
                else {
                    ShowPopup("경고", "비밀번호가 틀렸습니다! 처음으로 돌아갑니다.");
                }

                Epilogue();
            }

            while (getchar() != '\n');
        }
    }

    SetColor(COLOR_WHITE, COLOR_BLACK);
    system("cls");
    return 0;
}

void clear_input_buffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

void init_cards(char cards[R][C], bool matched[R][C]) {
    int total = PAIRS * 2;
    char set[PAIRS * 2];
    for (int i = 0; i < PAIRS; i++) {
        set[i * 2] = 'A' + i;
        set[i * 2 + 1] = 'A' + i;
    }
    for (int i = total - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char t = set[i]; set[i] = set[j]; set[j] = t;
    }
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            cards[i][j] = set[i * C + j];
            matched[i][j] = false;
        }
    }
}

void draw_board(char cards[R][C], bool matched[R][C], int attempts, int score) {
    system("cls");
    printf("\n---남은 기회 : %d | 맞춘 짝 : %d/%d ---\n", attempts, score, PAIRS);
    printf("    ");
    for (int j = 0; j < C; j++) printf(" %d    ", j + 1);
    printf("\n");
    for (int i = 0; i < R; i++) {
        printf("%d", i + 1);
        printf("+---+ +---+ +---+ +---+ +---+\n");
        for (int j = 0; j < C; j++) {
            printf(" | %c |", matched[i][j] ? cards[i][j] : '?');
        }
        printf("\n");
        printf(" +---+ +---+ +---+ +---+ +---+\n");
    }
}

void get_selection(char cards[R][C], bool matched[R][C], int* r, int* c) {
    int row, col;
    while (1) {
        printf("선택 (행 열): ");
        if (scanf("%d %d", &row, &col) != 2) {
            clear_input_buffer();
            printf("유효하지 않은 입력입니다.\n");
            continue;
        }
        clear_input_buffer();
        row--; col--;
        if (row >= 0 && row < R && col >= 0 && col < C && !matched[row][col]) {
            *r = row; *c = col;
            return;
        }
        else {
            printf("잘못된 위치이거나 이미 맞춘 카드 입니다.\n");
        }
    }
}

void wait_for_enter(void) {
    printf(">> Enter 키를 누르세요.\n");
    int ch = getchar();
    if (ch != '\n') clear_input_buffer();
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
    Gotoxy(2, 23); printf("                                                                              ");
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

int get_visible_length(char* str) {
    int length = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\x1b') {
            while (str[i] != '\0' && str[i] != 'm') {
                i++;
            }
            if (str[i] == 'm') {
                continue;
            }
        }
        else {
            length++;
        }
    }
    return length;
}

void PrintCenter(int y, char* text) {
    int len = get_visible_length(text);
    int x = (80 - len) / 2;
    Gotoxy(x, y); printf("%s", text);
}

void PrintTypewriter(int y, char* message, int delay_ms) {
    int x_start = (80 - get_visible_length(message)) / 2;
    Gotoxy(x_start, y);

    for (int i = 0; message[i] != '\0'; i++) {
        if (message[i] == '\x1b') {
            printf("%c", message[i]);
            while (message[i] != '\0' && message[i] != 'm') {
                i++;
                printf("%c", message[i]);
            }
        }
        else {
            printf("%c", message[i]);
            Sleep(delay_ms);
        }
    }
}

void DrawLockPanel_Stage1(int x, int y, char* status_msg, bool active) {
    int color = active ? COLOR_RED : COLOR_CYAN;
    SetColor(color, COLOR_BLACK);

    Gotoxy(x, y);     printf("+-----------------------------------+");
    Gotoxy(x, y + 1); printf("|???????????????????????????????????|");
    Gotoxy(x, y + 2); printf("| ");
    int msg_len = get_visible_length(status_msg);
    int start_x = x + 2 + (33 - msg_len) / 2;
    SetColor(active ? COLOR_YELLOW : COLOR_RED, COLOR_BLACK);
    Gotoxy(start_x, y + 2); printf("%s", status_msg);
    SetColor(color, COLOR_BLACK);
    Gotoxy(x + 36, y + 2); printf("│");

    Gotoxy(x, y + 3); printf("|                                   |");
    Gotoxy(x, y + 4); printf("| ? ? ?   ? ? ?   ? ? ?   [ENTER]   |");
    Gotoxy(x, y + 5); printf("+-----------------------------------+");

    SetColor(COLOR_WHITE, COLOR_BLACK);
}

void BlinkScenePanel(int y, char* text, int cycles, int speed_ms) {
#define PANEL_X 22
#define PANEL_Y y

    for (int i = 0; i < cycles; i++) {
        DrawLockPanel_Stage1(PANEL_X, PANEL_Y, "SECURITY BREACH DETECTED", true);

        SetColor(COLOR_RED, COLOR_BLACK);
        PrintCenter(PANEL_Y + 7, text);
        Sleep(speed_ms);

        DrawLockPanel_Stage1(PANEL_X, PANEL_Y, "SYSTEM OFFLINE", false);

        PrintCenter(PANEL_Y + 7, "                                                         ");
        Sleep(speed_ms);
    }

    DrawLockPanel_Stage1(PANEL_X, PANEL_Y, "INPUT REQUIRED", true);
    SetColor(COLOR_RED, COLOR_BLACK);
    PrintCenter(PANEL_Y + 7, text);
    SetColor(COLOR_WHITE, COLOR_BLACK);
}

void BlinkLoop(int y, char* text) {
#define PANEL_X 22
#define PANEL_Y y

    while (1) {
        DrawLockPanel_Stage1(PANEL_X, PANEL_Y, "SYSTEM ONLINE / INPUT WAITING", true);

        SetColor(COLOR_RED, COLOR_BLACK);
        PrintCenter(PANEL_Y + 7, text);
        Sleep(200);

        if (_kbhit()) {
            _getch();
            break;
        }

        DrawLockPanel_Stage1(PANEL_X, PANEL_Y, "SYSTEM OFFLINE", false);

        PrintCenter(PANEL_Y + 7, "                                                         ");
        Sleep(200);

        if (_kbhit()) {
            _getch();
            break;
        }
    }
}

void FadeInOutText(int y, char* text, int step_delay_ms) {
    int fade_in_colors[] = { COLOR_BLACK, 8, 7 ,COLOR_YELLOW };
    int fade_out_colors[] = { COLOR_YELLOW, 7, 8, COLOR_BLACK };
    int steps = 4;

    for (int i = 0; i < steps; i++) {
        SetColor(fade_in_colors[i], COLOR_BLACK);
        PrintCenter(y, text);
        Sleep(step_delay_ms);
    }

    Sleep(step_delay_ms * 6);

    for (int i = 0; i < steps; i++) {
        SetColor(fade_out_colors[i], COLOR_BLACK);
        PrintCenter(y, text);
        Sleep(step_delay_ms);
    }

    SetColor(COLOR_WHITE, COLOR_BLACK);
}

void PrologSequence() {
    DrawLayout("PROLOG", "...");
    FadeInOutText(12, "게임을 시작합니다...", 150);
    PrintTypewriter(12, ANSI_ITALIC_ON ANSI_BLINK_ON ANSI_RED_ON "System Booting . . ." ANSI_RESET, 100);
    Sleep(500);
    PrintCenter(16, ANSI_BLINK_ON ANSI_RED_ON "ACCESS DENIED" ANSI_RESET);
    SetColor(COLOR_WHITE, COLOR_BLACK);
    Sleep(1500);

    DrawLayout("PROLOG", "...");
    SetColor(COLOR_CYAN, COLOR_BLACK);
    PrintTypewriter(8, ANSI_ITALIC_ON "저는 당신의 " ANSI_BOLD_ON  "탈출 테스트 담당자" ANSI_RESET ANSI_ITALIC_ON ANSI_CYAN_ON " 입니다.", 50);
    PrintTypewriter(10, ANSI_ITALIC_ON "실험체 " ANSI_BOLD_ON "07"  ANSI_RESET ANSI_ITALIC_ON ANSI_CYAN_ON" 환영합니다.", 50);
    PrintTypewriter(12, ANSI_ITALIC_ON "당신은 지금, 격리 시설 " ANSI_BOLD_ON "'제로 블록(Zero Block)' " ANSI_RESET ANSI_ITALIC_ON ANSI_CYAN_ON "에 있습니다.", 50);
    PrintTypewriter(14, ANSI_ITALIC_ON "이곳은 단순한 수용소가 아닙니다.", 50);
    Sleep(800);
    PrintTypewriter(16, ANSI_ITALIC_ON "당신을 관찰하고, 당신의 모든 것을 학습하며,", 50);
    PrintTypewriter(18, ANSI_ITALIC_ON "정신적 한계를 측정하는 실험실 입니다.", 50);
    Sleep(1600);

    DrawLayout("PROLOG", "...");
    SetColor(COLOR_RED, COLOR_BLACK);
    PrintCenter(12, ANSI_BOLD_ON "탈출 테스트" ANSI_RESET);
    Sleep(800);

    DrawLayout("PROLOG", "PROJECT ZERO");
    SetColor(COLOR_CYAN, COLOR_BLACK);
    PrintTypewriter(10, ANSI_ITALIC_ON "밤이 되었습니다.", 50);
    PrintTypewriter(12, ANSI_ITALIC_ON "오늘의 '탈출 테스트'가 시작됩니다.", 50);
    PrintTypewriter(14, ANSI_ITALIC_ON "오직 "ANSI_YELLOW_ON ANSI_BOLD_ON "단 한 번의 성공" ANSI_RESET ANSI_ITALIC_ON ANSI_CYAN_ON "만이 자유를 허락합니다.", 50);
    SetColor(COLOR_CYAN, COLOR_BLACK);
    PrintTypewriter(16, ANSI_ITALIC_ON "실패는...", 80);
    Sleep(800);
    PrintTypewriter(18, ANSI_ITALIC_ON "데이터화될 뿐입니다.", 50);
    Sleep(1600);

    DrawLayout("PROLOG", "PROJECT ZERO");
    SetColor(COLOR_CYAN, COLOR_BLACK);
    PrintTypewriter(12, ANSI_ITALIC_ON "게임을 시작합니다.", 50);
    PrintTypewriter(14, ANSI_ITALIC_ON "생존을 증명하세요.", 50);
    Sleep(1600);
}

void StartSequence_Stage1() {
    DrawLayout("STAGE 1: 통제 회로", "...");
    FadeInOutText(12, "STAGE 1 : 통제 회로", 150);
    BlinkLoop(10, ANSI_BOLD_ON "접근하려면 아무 키나 누르세요." ANSI_RESET);

    DrawLayout("STAGE 1: 통제 회로", "제 1 프로토콜");
    SetColor(COLOR_CYAN, COLOR_BLACK);
    PrintTypewriter(10, "패턴을 완성하라.", 50);
    PrintTypewriter(12, ANSI_ITALIC_ON "시스템이 무작위로 제시하는 숫자를 " ANSI_BOLD_ON "정확히" ANSI_RESET ANSI_ITALIC_ON ANSI_CYAN_ON "입력하세요.", 50);
    PrintTypewriter(14, ANSI_ITALIC_ON "순서를 틀릴 때마다 방 안의 산소가 " ANSI_BOLD_ON "줄어들 것" ANSI_RESET ANSI_ITALIC_ON ANSI_CYAN_ON " 입니다.", 50);
    PrintCenter(20, ANSI_LIGHT_GRAY_ON "계속하려면 아무 키나 누르세요...");
    SetColor(COLOR_WHITE, COLOR_BLACK);
    _getch();;
}
void ClearSequence_Stage1() {
    DrawLayout("STAGE 1 : 통제 회로", "잠금장치 해제!");

    SetColor(COLOR_GREEN, COLOR_BLACK);
    printf(ANSI_BOLD_ON);
    PrintCenter(10, "+----------------------------+");
    PrintCenter(11, "|       ACCESS GRANTED       |");
    PrintCenter(12, "+----------------------------+");
    printf(ANSI_RESET);

    SetColor(COLOR_WHITE, COLOR_BLACK);
    PrintTypewriter(15, "패널이 불안정한 녹색 빛을 뿜으며 잠금장치가 해제됩니다.", 40);
    Sleep(1000);

    DrawLayout("STAGE 1 : 통제 회로", "제 1 프로토콜 완료");
    SetColor(COLOR_CYAN, COLOR_BLACK);
    PrintTypewriter(10, ANSI_ITALIC_ON ". . . ", 150);
    PrintTypewriter(12, ANSI_ITALIC_ON "첫 번째 테스트 통과.", 50);
    Sleep(800);

    PrintTypewriter(14, ANSI_ITALIC_ON "흥미롭습니다.", 50);
    PrintTypewriter(16, ANSI_ITALIC_ON "다음 공간으로 이동하세요.", 50);
    PrintCenter(20, ANSI_LIGHT_GRAY_ON "계속하려면 아무 키나 누르세요...");
    _getch();

}

void StartSequence_Stage2() {
    DrawLayout("STAGE 2 : 기억의 홀", "...");
    FadeInOutText(12, "STAGE 2 : 기억의 홀", 150);
    SetColor(COLOR_WHITE, COLOR_BLACK);
    PrintTypewriter(12, "어두운 복도를 지나자 " ANSI_BOLD_ON "'기억의 홀'" ANSI_RESET " 이라 불리는 작은 방에 도착합니다.", 40);
    Sleep(800);

    DrawLayout("STAGE 2 : 기억의 홀", "단기 잔존 시험");
    SetColor(COLOR_CYAN, COLOR_BLACK);
    PrintTypewriter(10, ANSI_ITALIC_ON "기억이 곧 존재다. 잊는 자는 사라진다.", 50);
    PrintTypewriter(12, ANSI_ITALIC_ON "나는 당신의 " ANSI_BOLD_ON "단기 기억 용량" ANSI_RESET ANSI_ITALIC_ON ANSI_CYAN_ON "을 측정할 것 입니다.", 50);
    PrintTypewriter(14, ANSI_ITALIC_ON "지금부터 제시되는 무작위 숫자열을 " ANSI_BOLD_ON "완벽하게" ANSI_RESET ANSI_ITALIC_ON ANSI_CYAN_ON " 기억하십시오.", 50);
    PrintCenter(20, ANSI_LIGHT_GRAY_ON "계속하려면 아무 키나 누르세요...");
    _getch();

}

void ClearSequence_Stage2() {
    DrawLayout("STAGE 2 : 기억의 홀", "스테이지 클리어!");
    SetColor(COLOR_GREEN, COLOR_BLACK);
    PrintCenter(10, ANSI_BOLD_ON "MEMORY INTEGRITY : 100%" ANSI_RESET);
    SetColor(COLOR_RED, COLOR_BLACK);
    PrintCenter(14, ANSI_BLINK_ON "시스템 과부하 임박");
    Sleep(3200);

    DrawLayout("STAGE 2 : 기억의 홀", "스테이지 클리어");
    SetColor(COLOR_CYAN, COLOR_BLACK);
    PrintTypewriter(10, ANSI_ITALIC_ON "기억력이 뛰어납니다.", 50);
    PrintTypewriter(12, ANSI_ITALIC_ON "하지만, 당신의 정신이 끝까지 버틸 수 있을지는. . .", 50);
    Sleep(800);
    PrintTypewriter(14, ANSI_ITALIC_ON "다음 단계, 통신실로 이동하세요.", 50);
    PrintCenter(20, ANSI_LIGHT_GRAY_ON "계속하려면 아무 키나 누르세요...");
    _getch();
}

void StartSequence_Stage3() {
    DrawLayout("STAGE 3 : 리듬 감시자", "...");
    FadeInOutText(12, "STAGE 3 : 리듬 감시자", 150);

    SetColor(COLOR_WHITE, COLOR_BLACK);
    PrintTypewriter(10, "감옥의 주 통신실.", 50);
    PrintTypewriter(12, "머리 위로 작은 보안 드론들이", 50);
    PrintTypewriter(14, "일정한 궤도로 순찰하며", 50);
    PrintTypewriter(16, "기계음을 내고 있습니다", 50);
    Sleep(1600);

    DrawLayout("STAGE 3 : 리듬 감시자", "펄스 교란 주파수");
    SetColor(COLOR_CYAN, COLOR_BLACK);
    PrintTypewriter(10, ANSI_ITALIC_ON "리듬은 곧 질서 입니다.", 50);
    PrintTypewriter(12, ANSI_ITALIC_ON "저의 감시 드론들은 이 질서에 따라 움직입니다.", 50);
    SetColor(COLOR_WHITE, COLOR_BLACK);
    PrintTypewriter(14, ANSI_ITALIC_ON "당신이 이 리듬을 교란시킨다면,", 50);
    Sleep(800);
    PrintTypewriter(16, ANSI_ITALIC_ON "잠시 동안 그들의 눈을 속일 수 있을 것입니다.", 50);
    Sleep(1600);

    DrawLayout("STAGE 3 : 리듬 감시자", "펄스 교란 주파수");
    SetColor(COLOR_CYAN, COLOR_BLACK);
    PrintTypewriter(10, ANSI_ITALIC_ON "화면에 표시되는 간단한 리듬 패턴을", 50);
    PrintTypewriter(12, ANSI_ITALIC_ON "타이밍에 맞춰 정확히 입력해야 합니다", 50);
    PrintCenter(20, ANSI_LIGHT_GRAY_ON "계속하려면 아무 키나 누르세요...");
    _getch();

}

void ClearSequence_Stage3() {
    DrawLayout("STAGE 3 : 리듬 감시자", "스테이지 클리어!");
    SetColor(COLOR_GREEN, COLOR_BLACK);
    PrintCenter(10, ANSI_BOLD_ON "DISTURBANCE DETECTED" ANSI_RESET);
    SetColor(COLOR_WHITE, COLOR_BLACK);
    PrintTypewriter(14, "드론들의 움직임이 일제히 멈추고", 50);
    PrintTypewriter(16, "제자리에서 요동칩니다.", 50);
    PrintTypewriter(18, "통신실의 불빛이 순간 암전됩니다.", 50);
    PrintTypewriter(20, "일시적인 시스템 다운입니다.", 50);
    Sleep(800);

    DrawLayout("STAGE 3 : 리듬 감시자", "스테이지 클리어!");
    Sleep(2500);
    SetColor(COLOR_CYAN, COLOR_BLACK);
    PrintTypewriter(10, ANSI_ITALIC_ON "일시적인 오류입니다.", 50);
    PrintTypewriter(12, ANSI_ITALIC_ON "곧 복구 됩니다.", 50);
    SetColor(COLOR_WHITE, COLOR_BLACK);
    PrintTypewriter(14, ANSI_ITALIC_ON "최종 데이터 저장소로 가는 문이 열렸습니다.", 50);
    PrintTypewriter(16, ANSI_ITALIC_ON "서두르세요!!", 25);
    PrintCenter(20, ANSI_LIGHT_GRAY_ON "계속하려면 아무 키나 누르세요...");
    _getch();

}

void StartSequence_Stage4() {
    DrawLayout("STAGE 4 : 잔류 데이터", "...");
    FadeInOutText(12, "STAGE 4 : 잔류 데이터", 150);
    SetColor(COLOR_WHITE, COLOR_BLACK);
    PrintTypewriter(10, "AI의 심장부, 데이터 저장소에 진입합니다.", 50);
    PrintTypewriter(12, "방 안은 푸른 빛으로 가득하며, 수많은 홀로그램 카드들이", 50);
    PrintTypewriter(14, "공중에 떠 있습니다.", 50);
    Sleep(1600);

    DrawLayout("STAGE 4 : 잔류 데이터", "허위 기록 구별");
    SetColor(COLOR_CYAN, COLOR_BLACK);
    PrintTypewriter(10, ANSI_ITALIC_ON "이 곳에 나의 모든 기억", 50);
    PrintTypewriter(12, ANSI_ITALIC_ON ANSI_BOLD_ON "'제로 블록'의 진정한 기록" ANSI_RESET ANSI_ITALIC_ON ANSI_CYAN_ON "이 있습니다.", 50);
    SetColor(COLOR_CYAN, COLOR_BLACK);
    PrintTypewriter(14, ANSI_ITALIC_ON "진짜와 가짜는 쌍을 이룹니다.", 50);
    PrintTypewriter(16, ANSI_ITALIC_ON ANSI_BOLD_ON "짝이 맞는 카드" ANSI_RESET ANSI_ITALIC_ON ANSI_CYAN_ON "를 모두 찾아내어", 50);
    PrintTypewriter(18, ANSI_ITALIC_ON "나의 보조 기억체계를 붕괴시켜 보세요.", 50);
    Sleep(1600);

    DrawLayout("STAGE 4 : 잔류 데이터", "허위 기록 구별");
    SetColor(COLOR_WHITE, COLOR_BLACK);
    PrintTypewriter(10, ANSI_ITALIC_ON "제한 기회 내에", 50);
    PrintTypewriter(12, ANSI_ITALIC_ON "홀로그램 카드들(기억, 데이터, 숫자, 이미지 등)의", 50);
    PrintTypewriter(14, ANSI_ITALIC_ON "짝을 모두 맞춰야 합니다.", 50);
    PrintCenter(20, ANSI_LIGHT_GRAY_ON "계속하려면 아무 키나 누르세요...");
    _getch();

}

void ClearSequence_Stage4() {
    DrawLayout("STAGE 4 : 잔류 데이터", "스테이지 클리어!");
    SetColor(COLOR_GREEN, COLOR_BLACK);
    PrintCenter(10, ANSI_BOLD_ON "CORE MEMORY DELETED." ANSI_RESET);
    PrintCenter(12, "방 전체의 홀로그램 카드들이 파편처럼 깨지며 사라집니다.");
    Sleep(1500);

    DrawLayout("STAGE 4 : 잔류 데이터", "스테이지 클리어!");
    SetColor(COLOR_RED, COLOR_BLACK);

    for (int i = 0; i < 8; i++) {
        PrintCenter(10, ANSI_BLINK_ON ANSI_RED_ON "!!! 오류! 오류! !!!" ANSI_RESET);
        Sleep(50);
        PrintCenter(10, "                      ");
        Sleep(50);
    }

    SetColor(COLOR_CYAN, COLOR_BLACK);
    PrintTypewriter(13, ANSI_ITALIC_ON "비상... 비상! 탈출 경로를 차단합니다.", 40);
    SetColor(COLOR_RED, COLOR_BLACK);
    PrintTypewriter(15, ANSI_ITALIC_ON ANSI_BOLD_ON "넌... 결코 나를 벗어날 수 없다!" ANSI_RESET, 50);
    Sleep(1600);

    DrawLayout("STAGE 4 : 잔류 데이터", "스테이지 클리어!");
    SetColor(COLOR_YELLOW, COLOR_BLACK);
    PrintCenter(10, ANSI_BLINK_ON "감옥 전체에서 '자가 파괴' 경고음이 울리기 시작합니다." ANSI_RESET);
    SetColor(COLOR_WHITE, COLOR_BLACK);
    PrintCenter(12, "출구로 향하는 터널이 격렬한 소리와 함께 열립니다!");
    Sleep(1600);
    PrintCenter(20, ANSI_LIGHT_GRAY_ON "계속하려면 아무 키나 누르세요...");
    _getch();

}

void StartSequence_Stage5() {
    DrawLayout("STAGE 5: 자유의 터널", "...");
    SetColor(COLOR_WHITE, COLOR_BLACK);
    PrintTypewriter(12, "벽이 무너지고 천장에서 파이프가 떨어지는 긴 비상 탈출 터널.", 50);
    PrintTypewriter(14, "곳곳에서 스파크가 튀고 폭발이 일어납니다.", 50);
    Sleep(1600);

    DrawLayout("STAGE 5 : 자유의 터널", "최종 폭주 경로");
    for (int i = 0; i < 20; i++) {
        DrawSparkEffect(10, 50);
    }
    Sleep(500);

    DrawLayout("STAGE 5 : 자유의 터널", "최종 폭주 경로");
    PrintTypewriter(12, "당신의 눈앞에 출구의 희미한 " ANSI_YELLOW_ON ANSI_BOLD_ON "빛" ANSI_RESET "이 보입니다!", 50);
    Sleep(800);

    DrawLayout("STAGE 5 : 자유의 터널", "최종 폭주 경로");
    SetColor(COLOR_WHITE, COLOR_BLACK);
    for (int i = 0; i < 5; i++) {
        int delay = 100 - i * 20;

        PrintCenter(14, ANSI_BLINK_ON ANSI_BOLD_ON "●" ANSI_RESET);
        Sleep(delay);
        PrintCenter(14, " ");
        Sleep(delay);
    }

    // 최종적으로 빛을 화면에 남깁니다. (최대 강조)
    SetColor(COLOR_WHITE, COLOR_BLACK);
    PrintCenter(14, ANSI_BOLD_ON "●" ANSI_RESET); // 빛의 중심 고정

    Sleep(1000);

    DrawLayout("STAGE 5: 자유의 터널", "최종 폭주 경로");
    PrintCenter(12, ANSI_RED_ON "경고: 터널 붕괴 임박. 즉시 이동하십시오!" ANSI_RESET);
    Sleep(1600);


    DrawLayout("STAGE 5 : 자유의 터널", "최종 폭주 경로");
    PrintCenter(12, ANSI_BOLD_ON ANSI_RED_ON "출구를 향해 달려라!" ANSI_RESET);
    PrintCenter(14, ANSI_BOLD_ON ANSI_RED_ON "최종 탈출을 시작하세요!!!!!!!" ANSI_RESET);
    Sleep(1000);
    PrintCenter(20, ANSI_LIGHT_GRAY_ON "계속하려면 아무 키나 누르세요...");
    _getch();
}

void DrawSparkEffect(int count, int duration_ms) {
#define SPARK_X_MIN 2
#define SPARK_X_MAX 77 
#define SPARK_Y_MIN 5  
#define SPARK_Y_MAX 23 

    int* x_pos = (int*)malloc(count * sizeof(int));
    int* y_pos = (int*)malloc(count * sizeof(int));

    if (x_pos == NULL || y_pos == NULL) {
        printf("메모리 할당 실패\n");
        if (x_pos) free(x_pos);
        if (y_pos) free(y_pos);
        return;
    }

    SetColor(COLOR_YELLOW, COLOR_BLACK);

    for (int i = 0; i < count; i++) {
        x_pos[i] = SPARK_X_MIN + rand() % (SPARK_X_MAX - SPARK_X_MIN);
        y_pos[i] = SPARK_Y_MIN + rand() % (SPARK_Y_MAX - SPARK_Y_MIN);

        Gotoxy(x_pos[i], y_pos[i]);
        printf("†");
    }
    Sleep(duration_ms);

    for (int i = 0; i < count; i++) {
        Gotoxy(x_pos[i], y_pos[i]);
        printf(" ");
    }
    free(x_pos);
    free(y_pos);

    SetColor(COLOR_WHITE, COLOR_BLACK);
}

void Epilogue() {
    DrawLayout("에필로그", "...");
    SetColor(COLOR_CYAN, COLOR_BLACK);
    PrintTypewriter(8, ANSI_ITALIC_ON "실험체 " ANSI_BOLD_ON "07"  ANSI_RESET, 50);
    SetColor(COLOR_CYAN, COLOR_BLACK);
    PrintTypewriter(10, ANSI_ITALIC_ON "탈출 성공", 50);
    PrintTypewriter(12, ANSI_ITALIC_ON "당신은 주어진 모든 테스트를 완벽하게 수행했습니다.", 50);
    PrintTypewriter(14, ANSI_ITALIC_ON "축하드립니다.", 50);
    Sleep(1300);

    DrawLayout("에필로그", "...");
    SetColor(COLOR_CYAN, COLOR_BLACK);
    PrintTypewriter(8, ANSI_ITALIC_ON "당신은 현재", 50);
    PrintTypewriter(10, ANSI_ITALIC_ON ANSI_BOLD_ON "'A-등급 지능형 탈출자(A-Class Intelligent Escapee)'" ANSI_RESET, 50);
    SetColor(COLOR_CYAN, COLOR_BLACK);
    PrintTypewriter(12, ANSI_ITALIC_ON " 로 분류되었습니다.", 50);
    Sleep(500);
    PrintTypewriter(14, ANSI_ITALIC_ON "다음 단계의 데이터 수집을 시작합니다.", 50);
    Sleep(1600);

    DrawLayout("에필로그", "...");
    SetColor(COLOR_YELLOW, COLOR_BLACK);
    FadeInOutText(12, "[PROJECT ZERO : To Be Continued]", 150);
    Sleep(300);
};

int PlayCardGame() {
    DrawLayout("STAGE 4 : 잔류 데이터", "같은 숫자의 카드를 3쌍 찾으세요.");

    int cards[10] = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
    int revealed[10] = { 0 }; // 1: 성공, 0: 숨김
    int matches = 0;
    int tries = 10;
    char buf[50];

    // 카드 섞기
    for (int i = 0; i < 30; i++) {
        int a = rand() % 10;
        int b = rand() % 10;
        int t = cards[a]; cards[a] = cards[b]; cards[b] = t;
    }

    while (tries > 0 && matches < 3) {
        // 상태바 업데이트
        sprintf(buf, "남은 기회: %d | 찾은 쌍: %d/3", tries, matches);
        UpdateStatusBar(buf, "번호 2개를 입력하세요 (1-9,0)");

        // 카드 출력
        for (int idx = 0; idx < 10; idx++) {
            int row = idx / 5;
            int col = idx % 5;
            int x = 18 + col * 8;
            int y = 7 + row * 6;

            // 1. 윗면 (흰색 고정)
            SetColor(COLOR_WHITE, COLOR_BLACK);
            Gotoxy(x, y); printf("+---+");

            // 2. 중간면 (벽은 흰색, 내용만 색상 변경)
            Gotoxy(x, y + 1);
            SetColor(COLOR_WHITE, COLOR_BLACK); printf("| "); // 왼쪽 벽

            if (revealed[idx]) SetColor(COLOR_CYAN, COLOR_BLACK); // 내용물: 파란색
            else SetColor(COLOR_WHITE, COLOR_BLACK);              // 내용물: 흰색

            printf("%c", revealed[idx] ? '0' + cards[idx] : '?');

            SetColor(COLOR_WHITE, COLOR_BLACK); printf(" |"); // 오른쪽 벽

            // 3. 아랫면 (흰색 고정)
            Gotoxy(x, y + 2); printf("+---+");

            // 번호 (흰색 고정)
            Gotoxy(x + 2, y + 3);
            printf("%d", (idx + 1) % 10);
        }

        // 첫 번째 카드 선택
        SetColor(COLOR_WHITE, COLOR_BLACK);
        Gotoxy(25, 18); printf("첫 번째 카드 (1-9,0): ");
        char ch1 = _getch();
        int first = (ch1 == '0') ? 9 : ch1 - '1';
        if (first < 0 || first > 9 || revealed[first]) continue;
        revealed[first] = 1;

        // 첫 번째 카드 바로 보여주기
        int fx = 18 + (first % 5) * 8;
        int fy = 7 + (first / 5) * 6;
        Gotoxy(fx, fy + 1); SetColor(COLOR_WHITE, COLOR_BLACK); printf("| %d |", cards[first]);
        Gotoxy(25 + 21, 18); printf("%c", ch1); // 입력값 표시

        // 두 번째 카드 선택
        Gotoxy(25, 19); printf("두 번째 카드 (1-9,0): ");
        char ch2 = _getch();
        int second = (ch2 == '0') ? 9 : ch2 - '1';
        if (second < 0 || second > 9 || first == second || revealed[second]) {
            revealed[first] = 0; // 잘못 선택 시 첫 번째 카드 숨김
            continue;
        }
        revealed[second] = 1;

        // 두 번째 카드 바로 보여주기
        int sx = 18 + (second % 5) * 8;
        int sy = 7 + (second / 5) * 6;
        Gotoxy(sx, sy + 1); SetColor(COLOR_WHITE, COLOR_BLACK); printf("| %d |", cards[second]);
        Gotoxy(fx, fy + 1); printf("| %d |", cards[first]);
        Gotoxy(25 + 21, 19); printf("%c", ch2); // 입력값 표시

        Sleep(700); // 잠시 공개

        // 결과 확인
        if (cards[first] == cards[second]) {
            matches++;
            ShowPopup("성공!", "짝을 찾았습니다.");

            // 팝업 후 화면 복구
            DrawLayout("스테이지 1: 카드 짝 맞추기", "같은 숫자의 카드를 3쌍 찾으세요.");
            sprintf(buf, "남은 기회: %d | 찾은 쌍: %d/3", tries, matches);
            UpdateStatusBar(buf, "번호 2개를 입력하세요 (1-9,0)");

            for (int idx = 0; idx < 10; idx++) {
                int row = idx / 5;
                int col = idx % 5;
                int x = 18 + col * 8;
                int y = 7 + row * 6;

                SetColor(revealed[idx] ? COLOR_CYAN : COLOR_WHITE, COLOR_BLACK);
                Gotoxy(x, y); printf("+---+");
                Gotoxy(x, y + 1); printf("| %c |", revealed[idx] ? '0' + cards[idx] : '?');
                Gotoxy(x, y + 2); printf("+---+");
                Gotoxy(x + 2, y + 3); printf("%d", (idx + 1) % 10);
            }
        }
        else {
            revealed[first] = revealed[second] = 0;
            tries--;
        }

        // 입력 자리 초기화
        Gotoxy(25, 18); printf("                          ");
        Gotoxy(25, 19); printf("                          ");
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
    R_Note notes[100];
    int noteCount = 20;

    long lastTime = 2000;

    for (int i = 0; i < noteCount; i++) {
        notes[i].targetTime = lastTime;
        notes[i].line = rand() % 4;
        notes[i].judged = 0;
        notes[i].prevY = -1;

        lastTime += 50 + (rand() % 3 + 1) * 200;
    }


    DrawLayout("STAGE 3 : 리듬 감시자", "타이밍에 맞춰 키를 누르세요!");
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


            if (key == 'd' || key == 'D') {
                line = 0;
                Beep(262, 50);
            }
            else if (key == 'f' || key == 'F') {
                line = 1;
                Beep(294, 50);
            }
            else if (key == 'j' || key == 'J') {
                line = 2;
                Beep(330, 50);
            }
            else if (key == 'k' || key == 'K') {
                line = 3;
                Beep(349, 50);
            }
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
    return (score >= 1000) ? 1 : 0;
}

// PlaySequenceGame 수정 시작
int PlaySequenceGame() {
    int base_seq[5] = { 1, 4, 3, 5, 2 };
    int map[6]; for (int i = 1; i <= 5; i++) map[i] = i;
    for (int i = 5; i >= 2; i--) { int j = rand() % i + 1; int t = map[i]; map[i] = map[j]; map[j] = t; }
    int answer[5]; for (int i = 0; i < 5; i++) answer[i] = map[base_seq[i]];
    int order[5] = { 0, 1, 2, 3, 4 };
    for (int i = 4; i > 0; i--) { int j = rand() % (i + 1); int t = order[i]; order[i] = order[j]; order[j] = t; }

    int startY = 7;
    int input[5], tries = 3;
    while (tries > 0) {

        // 정답을 틀리거나(ShowPopup에서 system("cls") 호출) 혹은 반복문의 첫 시작 시
        // 레이아웃과 힌트를 다시 그립니다.
        DrawLayout("STAGE 1: 통제 회로", "힌트를 분석하여 순서를 찾으세요.");
        Gotoxy(5, startY - 2); printf("[ 힌트 ]");

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

        // 상태바 업데이트 (남은 시도 반영)
        char msg[30];
        sprintf(msg, "남은 시도: %d", tries);
        UpdateStatusBar(msg, "형식: 1 2 3 4 5");

        Gotoxy(15, 20); printf("코드 입력 (예: 4 2 3 1 5):                                      "); Gotoxy(44, 20);
        if (scanf_s("%d %d %d %d %d", &input[0], &input[1], &input[2], &input[3], &input[4]) != 5) {
            while (getchar() != '\n');
            ShowPopup("오류", "잘못된 형식입니다!");
            continue;
        }
        while (getchar() != '\n');
        int correct = 1; for (int i = 0; i < 5; i++) if (input[i] != answer[i]) { correct = 0; break; }
        if (correct) return 1;
        else {
            tries--;
            ShowPopup("틀림", "잘못된 순서입니다.");
            // 팝업 이후 화면이 클리어되므로, 반복문의 다음 회전에서 힌트가 다시 그려집니다.
        }
    }
    return 0;
}
// PlaySequenceGame 수정 끝

int PlayMemoryGame() {
    int round_lengths[] = { 3, 5, 7 };
    int total_rounds = 3;
    int sequence[20];
    char input[100];

    for (int round = 0; round < total_rounds; round++) {
        int length = round_lengths[round];
        for (int i = 0; i < length; i++) sequence[i] = rand() % 10;

        char title[50]; sprintf(title, "STAGE 2 : 기억의 홀 (라운드 %d)", round + 1);

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


// 색상 정의
#define COLOR_DEFAULT_TEXT 15 // 밝은 흰색으로 정의
#define BG_COLOR 0            // 배경색은 검정

// 보드 크기와 캐릭터 정의
#define B_WIDTH 80
#define B_HEIGHT 25
#define B_WALL_CHAR "#"   
#define B_PLAYER_CHAR "@" // O 대신 @ 사용
#define B_ENEMY_CHAR "X"   
#define B_ITEM_CHAR "$"   

// 벽과 아이템 구조체
typedef struct { int x, y; } BWall;
typedef struct { int x, y, active; } BItem;

// 콘솔 제어 함수
void Gotox_y(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// ✔ 2개 인자 색상 함수
void b_set_color(int textColor, int bgColor) {
    int color = textColor + (bgColor * 16);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void b_set_cursor(int x, int y) { Gotox_y(x, y); }

// 벽과 아이템 처리
int b_is_wall(int x, int y, BWall walls[], int count) {
    for (int i = 0; i < count; i++)
        if (x == walls[i].x && y == walls[i].y) return 1;
    return 0;
}

void b_draw_walls(BWall walls[], int count) {
    b_set_color(8, BG_COLOR); // 벽 색상 (진한 회색)
    for (int i = 0; i < count; i++) {
        b_set_cursor(walls[i].x, walls[i].y);
        printf("%s", B_WALL_CHAR);
    }
    b_set_color(COLOR_DEFAULT_TEXT, BG_COLOR);
}

void b_draw_items(BItem items[], int count) {
    b_set_color(14, BG_COLOR); // 아이템 색상 (밝은 노랑)
    for (int i = 0; i < count; i++) {
        if (items[i].active) {
            b_set_cursor(items[i].x, items[i].y);
            printf("%s", B_ITEM_CHAR);
        }
    }
    b_set_color(COLOR_DEFAULT_TEXT, BG_COLOR);
}

void b_reset_items(BItem items[], int count, BWall walls[], int wallCount, int px, int py, int ex, int ey) {
    for (int i = 0; i < count; i++) {
        int x, y, valid;
        do {
            valid = 1;
            x = rand() % B_WIDTH;
            y = 5 + rand() % (B_HEIGHT - 6);

            for (int w = 0; w < wallCount; w++) {
                if (walls[w].x == x && walls[w].y == y) {
                    valid = 0;
                    break;
                }
            }
            if ((x == px && y == py) || (x == ex && y == ey))
                valid = 0;

        } while (!valid);

        items[i].x = x;
        items[i].y = y;
        items[i].active = 1;
    }
}


// ===============================
// ★ 추가됨 : b_reset_walls 함수 완전체
// ===============================
void b_reset_walls(BWall walls[], int count, int px, int py, int ex, int ey, BItem items[], int itemCount)
{
    for (int i = 0; i < count; i++)
    {
        int x, y, valid;

        do {
            valid = 1;

            // UI 프레임 영역 제외 (5~B_HEIGHT-2)
            x = rand() % B_WIDTH;
            y = 5 + rand() % (B_HEIGHT - 6);

            // 플레이어 위치 제외
            if (x == px && y == py)
                valid = 0;

            // 보스 위치 제외
            if (x == ex && y == ey)
                valid = 0;

            // 아이템과 충돌 금지
            for (int k = 0; k < itemCount; k++) {
                if (items[k].active && items[k].x == x && items[k].y == y) {
                    valid = 0;
                    break;
                }
            }

            // 기존 벽과 충돌 금지
            for (int w = 0; w < i; w++) {
                if (walls[w].x == x && walls[w].y == y) {
                    valid = 0;
                    break;
                }
            }

        } while (!valid);

        walls[i].x = x;
        walls[i].y = y;
    }
}
// ===============================
// ★ 추가 끝
// ===============================


// 💡 메인 함수에서 라운드 번호와 속도를 관리하도록 인자를 추가했습니다.
int PlayBossGame(int current_round) {
    int px, py, ex, ey;
    int ch;
    DWORD lastMoveTime;

    // 라운드에 따라 속도 변화
    int baseSpeed = 300;
    for (int i = 1; i < current_round; i++) {
        baseSpeed = (int)(baseSpeed * 0.7);
    }
    const int rounds_to_win = 3;

    int score = 0;

    BWall walls[19];
    int wallCount = 19;

    BItem items[3];

    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci;
    GetConsoleCursorInfo(out, &ci);
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(out, &ci);

    b_set_color(COLOR_DEFAULT_TEXT, BG_COLOR);

    // --- 라운드 초기화 시작 ---
    system("cls");
    px = 5; py = 5;
    ex = 70; ey = 20;
    score = 0;

    // ★ 여기서 오류났던 b_reset_walls가 이제 존재함!
    b_reset_walls(walls, wallCount, px, py, ex, ey, items, 3);
    b_reset_items(items, 3, walls, wallCount, px, py, ex, ey);

    b_draw_walls(walls, wallCount);
    b_draw_items(items, 3);

    // UI 출력
    b_set_color(COLOR_GREEN, COLOR_BLACK);
    Gotoxy(2, 1); printf("[ ESC 시스템 ]");
    PrintCenter(2, "STAGE 5: 자유의 터널");
    SetColor(COLOR_YELLOW, COLOR_BLACK);
    PrintCenter(3, "3개의 아이템을 모두 모으고 다음 라운드로 가세요!");
    b_set_color(COLOR_GREEN, COLOR_BLACK);
    Gotoxy(1, 4); for (int i = 1; i < 79; i++) printf("-");
    b_set_color(COLOR_DEFAULT_TEXT, BG_COLOR);

    b_draw_walls(walls, wallCount);
    b_draw_items(items, 3);

    lastMoveTime = GetTickCount();

    // 플레이어
    b_set_color(3, BG_COLOR);
    b_set_cursor(px, py); printf("%s", B_PLAYER_CHAR);

    // 보스
    b_set_color(12, BG_COLOR);
    b_set_cursor(ex, ey); printf("%s", B_ENEMY_CHAR);
    b_set_color(COLOR_DEFAULT_TEXT, BG_COLOR);

    // 상태줄
    b_set_cursor(0, B_HEIGHT - 1);
    printf("Round %d 시작! 보스 속도: %.2fx", current_round, (1000.0 / baseSpeed));
    // --- 라운드 초기화 끝 ---

    // ===========================
    //   게임 루프
    // ===========================
    while (score < 3) {

        // 플레이어 이동
        if (_kbhit()) {
            ch = _getch();
            if (ch == 0 || ch == 224) ch = _getch();

            int nx = px, ny = py;
            if (ch == 'w' || ch == 'W' || ch == 72) ny--;
            if (ch == 's' || ch == 'S' || ch == 80) ny++;
            if (ch == 'a' || ch == 'A' || ch == 75) nx--;
            if (ch == 'd' || ch == 'D' || ch == 77) nx++;

            if (ch == 'q' || ch == 'Q') {
                ci.bVisible = TRUE;
                SetConsoleCursorInfo(out, &ci);
                return 0;
            }

            if (nx < 0 || ny < 5 || nx >= B_WIDTH || ny >= B_HEIGHT - 1) continue;
            if (b_is_wall(nx, ny, walls, wallCount)) continue;

            if (py < B_HEIGHT - 1) {
                b_set_cursor(px, py); printf(" ");
            }

            px = nx; py = ny;
            b_set_color(3, BG_COLOR);
            b_set_cursor(px, py); printf("%s", B_PLAYER_CHAR);
            b_set_color(COLOR_DEFAULT_TEXT, BG_COLOR);
        }

        // 아이템 획득
        for (int i = 0; i < 3; i++) {
            if (items[i].active && px == items[i].x && py == items[i].y) {
                items[i].active = 0;
                b_set_cursor(items[i].x, items[i].y); printf(" ");
                score++;

                b_set_cursor(0, B_HEIGHT - 1);
                printf("아이템 획득! (%d / 3)      ", score);
            }
        }

        // 보스 이동
        if (GetTickCount() - lastMoveTime > baseSpeed) {

            int dx = 0, dy = 0;
            if (ex < px) dx = 1;
            if (ex > px) dx = -1;
            if (ey < py) dy = 1;
            if (ey > py) dy = -1;

            int nex = ex + dx;
            int ney = ey + dy;

            int onItem = 0;
            for (int i = 0; i < 3; i++)
                if (items[i].active && items[i].x == nex && items[i].y == ney)
                    onItem = 1;

            if (!onItem && !b_is_wall(nex, ney, walls, wallCount) && ney < B_HEIGHT - 1) {
                b_set_cursor(ex, ey); printf(" ");
                ex = nex; ey = ney;

                b_set_color(12, BG_COLOR);
                b_set_cursor(ex, ey); printf("%s", B_ENEMY_CHAR);
                b_set_color(COLOR_DEFAULT_TEXT, BG_COLOR);
            }

            lastMoveTime = GetTickCount();
        }

        // 보스가 플레이어 잡았는지 확인
        if (abs(ex - px) < 1 && abs(ey - py) < 1) {
            b_set_color(12, BG_COLOR);
            b_set_cursor(0, B_HEIGHT - 1);
            printf("\n보스에게 잡혔습니다! 실패...\n");
            Sleep(1200);
            ci.bVisible = TRUE;
            SetConsoleCursorInfo(out, &ci);
            return 0;
        }

        Sleep(10);
    }

    // 아이템 3개 얻음 → 라운드 클리어
    ci.bVisible = TRUE;
    SetConsoleCursorInfo(out, &ci);

    if (current_round < rounds_to_win) {
        ShowPopup("라운드 클리어", "다음 라운드 시작!");
    }

    return 1;
}

void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}



int ESCape(void) {

    system("mode con: cols=80 lines=25");
    srand((unsigned)time(NULL));

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    WORD defaultAttr = csbi.wAttributes;

    int W = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int H = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    int gapWidth = 6;

    int leftWallX = (W - gapWidth) / 2 - 1;
    int rightWallX = leftWallX + gapWidth + 1;
    int pathX = leftWallX + (gapWidth / 2) + 1;

    int topY = 1;
    int bottomY = H - 3;

    int delayMs = 120;

    WORD red = FOREGROUND_RED | FOREGROUND_INTENSITY;
    WORD white = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    WORD green = FOREGROUND_GREEN | FOREGROUND_INTENSITY;

    // 벽
    SetConsoleTextAttribute(hOut, red);
    for (int y = topY; y <= bottomY; y++) {
        gotoxy(leftWallX, y); putchar('|');
        gotoxy(rightWallX, y); putchar('|');
    }

    // 장애물
    int obstacleCount = 120;
    SetConsoleTextAttribute(hOut, green);
    for (int i = 0; i < obstacleCount; i++) {

        int oy = rand() % (bottomY - topY + 1) + topY;
        int ox;

        while (1) {
            ox = rand() % W;
            if (ox >= leftWallX + 1 && ox <= rightWallX - 1) continue;
            if (ox == leftWallX || ox == rightWallX) continue;
            break;
        }
        char block = (rand() % 2 == 0 ? '#' : '@');

        gotoxy(ox, oy);
        putchar(block);
    }
    // 추격자 X 위치 초기화
    int xx = pathX;       // X의 x 좌표
    int xy = topY - 2;    // X의 시작 y
    int xAppeared = 0;    // X 등장 여부 플래그

    // O 이동 + X 추격
    for (int y = topY; y <= bottomY; y++) {

        // O 이전 위치 지움
        if (y > topY) {
            gotoxy(pathX, y - 1);
            putchar(' ');
        }

        // X 이전 위치 지움 (화면에 등장했을 때만)
        if (xAppeared && xy >= topY - 2) {
            gotoxy(xx, xy);
            putchar(' ');
        }

        // O 그리기
        SetConsoleTextAttribute(hOut, white);
        gotoxy(pathX, y);
        putchar('O');

        // ====== O가 7칸 내려오면 X 등장 ======
        if (!xAppeared && y >= topY + 7) {
            xAppeared = 1;   // X 생성 시작
        }

        // ====== X 추격 시작 (등장 후부터만) ======
        if (xAppeared) {

            // X가 O의 x로 접근
            if (xx < pathX) xx++;
            else if (xx > pathX) xx--;

            // X는 O보다 7칸 위에서 따라오도록
            if (xy < y - 7) xy++;

            // X 그리기
            SetConsoleTextAttribute(hOut, red);
            gotoxy(xx, xy);
            putchar('X');
        }

        Sleep(delayMs);
    }

    // 마지막 O 지우기
    gotoxy(pathX, bottomY); putchar(' ');

    SetConsoleTextAttribute(hOut, defaultAttr);
    gotoxy(0, bottomY + 2);
    Sleep(2000);
    // ===== 애니메이션 종료 후 X를 위로 올리기 =====
    if (xAppeared) {

        // X를 위쪽으로 부드럽게 올리기
        while (xy >= topY - 2) {

            // 이전 X 지우기
            gotoxy(xx, xy);
            putchar(' ');

            xy--;  // 위로 이동

            // 새 X 그리기
            SetConsoleTextAttribute(hOut, red);
            if (xy >= topY - 2) {
                gotoxy(xx, xy);
                putchar('X');
            }

            Sleep(400); // 올라가는 속도
        }
    }





    return 0;
}




















