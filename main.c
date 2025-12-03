#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

// UI 색상 정의
#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_YELLOW 6
#define COLOR_WHITE 7

// 카드 게임 매크로 정의
#define R 2
#define C 5
#define PAIRS 5
#define MAX_ATTEMPTS 10

// 최종 비밀번호 단서 정의
int FINAL_CODE[5] = { 7, 3, 9, 1, 5 };

// UI 관련 함수 선언
void InitUI(); // 콘솔 초기 설정 (커서 숨김, 창 제목 설정, 크기 설정)
void Gotoxy(int x, int y); // 커서 위치 이동
void SetColor(int textColor, int bgColor); // 텍스트 및 배경 색상 설정
void DrawLayout(char* title, char* subtitle); // 게임 화면 기본 레이아웃 (테두리, 제목) 그리기
void UpdateStatusBar(char* leftMsg, char* rightMsg); // 하단 상태 표시줄 메시지 업데이트
void ShowPopup(char* title, char* message); // 팝업창 표시 후 키 입력 대기
void PrintCenter(int y, char* text); // 특정 줄에 텍스트를 가운데 정렬하여 출력
long GetTick() { return (long)clock(); } // 시간 측정 (타이밍용)

// 게임 보조 함수 선언
void clear_input_buffer(void); // 입력 버퍼 비우기
void wait_for_enter(void); // Enter 키 입력 대기

// 스테이지 게임 함수 선언
int PlayCardGame(); // 스테이지 1: 카드 짝 맞추기 게임
int PlayRhythmGame(); // 스테이지 2: 리듬 게임
int PlaySequenceGame(); // 스테이지 3: 논리 퍼즐 게임 (순서 찾기)
void draw_puzzle_screen(int current_tries, int map[], int order[]); // SequenceGame 퍼즐 화면 및 힌트 출력
int PlayMemoryGame(); // 스테이지 4: 기억력 게임 (숫자 시퀀스 기억)
int PlayBossGame(int current_round); // 스테이지 5: 보스 미로 게임 (추격전)

// 메인 함수
int main() {
    system("cls");
    InitUI();
    srand((unsigned int)time(NULL));

    char msgBuf[100];

    while (1) {
        // 메인 메뉴 화면 출력
        DrawLayout(" ESC_ TEAM ", "방탈출 프로젝트");
        PrintCenter(10, "1. 게임 시작");
        PrintCenter(12, "2. 종료");
        UpdateStatusBar("번호를 선택하세요", "팀 ESC");

        char choice = _getch();

        if (choice == '2') {
            break;
        }
        else if (choice == '1') {
            // 프롤로그 출력
            DrawLayout("프롤로그", "아무 키나 누르세요...");
            PrintCenter(10, "당신은 잠긴 방에서 깨어납니다...");
            PrintCenter(12, "5개의 단서를 찾아 탈출하세요.");
            _getch();

            // 스테이지 1 실행 및 결과 처리
            if (PlayCardGame() == 0) {
                ShowPopup("실패", "게임 오버 (1단계)");
                continue;
            }
            sprintf(msgBuf, "첫 번째 단서 획득: [ %d ]", FINAL_CODE[0]);
            ShowPopup("스테이지 클리어", msgBuf);

            // 스테이지 2 실행 및 결과 처리
            if (PlayRhythmGame() == 0) {
                ShowPopup("실패", "게임 오버 (2단계)");
                continue;
            }
            sprintf(msgBuf, "두 번째 단서 획득: [ %d ]", FINAL_CODE[1]);
            ShowPopup("스테이지 클리어", msgBuf);

            // 스테이지 3 실행 및 결과 처리
            if (PlaySequenceGame() == 0) {
                ShowPopup("실패", "게임 오버 (3단계)");
                continue;
            }
            sprintf(msgBuf, "세 번째 단서 획득: [ %d ]", FINAL_CODE[2]);
            ShowPopup("스테이지 클리어", msgBuf);

            // 스테이지 4 실행 및 결과 처리
            if (PlayMemoryGame() == 0) {
                ShowPopup("실패", "게임 오버 (4단계)");
                continue;
            }
            sprintf(msgBuf, "네 번째 단서 획득: [ %d ]", FINAL_CODE[3]);
            ShowPopup("스테이지 클리어", msgBuf);

            // 스테이지 5 (보스 미로) 3 라운드 실행 및 결과 처리
            int final_clue_get = 1;
            for (int round = 1; round <= 3; round++) {
                if (PlayBossGame(round) == 0) {
                    ShowPopup("실패", "게임 오버 (5단계)");
                    final_clue_get = 0;
                    break;
                }
                if (round < 3) {
                }
            }

            if (!final_clue_get) {
                continue;
            }

            sprintf(msgBuf, "마지막 단서 획득: [ %d ]", FINAL_CODE[4]);
            ShowPopup("스테이지 클리어", msgBuf);


            // 최종 관문: 도어락 비밀번호 입력 및 확인
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
                    // 탈출 성공 메시지 출력
                    ShowPopup("잠금 해제", "철컥! 문이 열립니다...");

                    DrawLayout("탈출 성공!", "축하합니다!");
                    SetColor(COLOR_GREEN, COLOR_BLACK);
                    PrintCenter(10, "#################################");
                    PrintCenter(11, "#         MISSION COMPLETE      #");
                    PrintCenter(12, "#################################");
                    SetColor(COLOR_WHITE, COLOR_BLACK);
                    PrintCenter(15, "팀원: 홍유진, 김명동, 김장혁, 이동호, 팽선우");
                    PrintCenter(17, "플레이해주셔서 감사합니다.");
                    _getch();
                }
                else {
                    ShowPopup("경고", "비밀번호가 틀렸습니다! 처음으로 돌아갑니다.");
                }
            }

            while (getchar() != '\n' && getchar() != EOF);
        }
    }

    SetColor(COLOR_WHITE, COLOR_BLACK);
    system("cls");
    return 0;
}

// 보조 함수 구현
void clear_input_buffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

void wait_for_enter(void) {
    printf(">> Enter 키를 누르세요.\n");
    int ch = getchar();
    if (ch != '\n') clear_input_buffer();
}

// UI 함수 구현
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

void PrintCenter(int y, char* text) {
    int len = 0; while (text[len] != '\0') len++;
    int x = (80 - len) / 2;
    Gotoxy(x, y); printf("%s", text);
}

// 스테이지 1: 카드 짝 맞추기 구현 (3쌍 찾기)
int PlayCardGame() {
    DrawLayout("스테이지 1: 카드 짝 맞추기", "같은 숫자의 카드를 3쌍 찾으세요.");

    int cards[10] = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
    int revealed[10] = { 0 };
    int matches = 0;
    int tries = 10;
    char buf[50];

    // 카드 섞기 로직
    for (int i = 0; i < 30; i++) {
        int a = rand() % 10;
        int b = rand() % 10;
        int t = cards[a]; cards[a] = cards[b]; cards[b] = t;
    }

    while (tries > 0 && matches < 3) {
        // 상태바 업데이트
        sprintf(buf, "남은 기회: %d | 찾은 쌍: %d/3", tries, matches);
        UpdateStatusBar(buf, "번호 2개를 입력하세요 (1-9,0)");

        // 카드 출력 로직
        for (int idx = 0; idx < 10; idx++) {
            int row = idx / 5;
            int col = idx % 5;
            int x = 18 + col * 8;
            int y = 7 + row * 6;

            SetColor(COLOR_WHITE, COLOR_BLACK);
            Gotoxy(x, y); printf("┌───┐");

            Gotoxy(x, y + 1);
            SetColor(COLOR_WHITE, COLOR_BLACK); printf("│ ");

            if (revealed[idx] == 2) SetColor(COLOR_MAGENTA, COLOR_BLACK);
            else if (revealed[idx] == 1) SetColor(COLOR_CYAN, COLOR_BLACK);
            else SetColor(COLOR_WHITE, COLOR_BLACK);

            printf("%c", (revealed[idx] >= 1) ? '0' + cards[idx] : '?');

            SetColor(COLOR_WHITE, COLOR_BLACK); printf(" │");

            Gotoxy(x, y + 2); printf("└───┘");

            Gotoxy(x + 2, y + 3);
            printf("%d", (idx + 1) % 10);
        }

        // 첫 번째 카드 선택 입력 처리
        SetColor(COLOR_WHITE, COLOR_BLACK);
        Gotoxy(25, 18); printf("첫 번째 카드 (1-9,0): ");
        char ch1 = _getch();
        int first = (ch1 == '0') ? 9 : ch1 - '1';

        // 유효성 검사 및 임시 공개
        if (first < 0 || first > 9 || revealed[first] == 2 || revealed[first] == 1) {
            Gotoxy(25, 18); printf("                                      ");
            Gotoxy(25, 19); printf("                                      ");
            continue;
        }
        revealed[first] = 1;

        // 첫 번째 카드 시각적 업데이트
        int fx = 18 + (first % 5) * 8;
        int fy = 7 + (first / 5) * 6;
        Gotoxy(fx + 2, fy + 1); SetColor(COLOR_CYAN, COLOR_BLACK); printf("%d", cards[first]);
        Gotoxy(25 + 21, 18); printf("%c", ch1);

        // 두 번째 카드 선택 입력 처리
        Gotoxy(25, 19); printf("두 번째 카드 (1-9,0): ");
        char ch2 = _getch();
        int second = (ch2 == '0') ? 9 : ch2 - '1';

        // 유효성 검사 및 임시 공개
        if (second < 0 || second > 9 || first == second || revealed[second] == 2) {
            revealed[first] = 0;
            Gotoxy(25, 18); printf("                                      ");
            Gotoxy(25, 19); printf("                                      ");
            continue;
        }
        revealed[second] = 1;

        // 두 번째 카드 시각적 업데이트
        int sx = 18 + (second % 5) * 8;
        int sy = 7 + (second / 5) * 6;
        Gotoxy(sx + 2, sy + 1); SetColor(COLOR_CYAN, COLOR_BLACK); printf("%d", cards[second]);
        Gotoxy(25 + 21, 19); printf("%c", ch2);

        Sleep(700);

        // 결과 확인 및 처리 (성공/실패)
        if (cards[first] == cards[second]) {
            matches++;
            revealed[first] = revealed[second] = 2;
            ShowPopup("성공!", "짝을 찾았습니다.");

            // 팝업 후 화면 재구성
            DrawLayout("스테이지 1: 카드 짝 맞추기", "같은 숫자의 카드를 3쌍 찾으세요.");
            sprintf(buf, "남은 기회: %d | 찾은 쌍: %d/3", tries, matches);
            UpdateStatusBar(buf, "번호 2개를 입력하세요 (1-9,0)");

            // 매치된 카드 색상 변경하여 다시 출력
            for (int idx = 0; idx < 10; idx++) {
                int row = idx / 5;
                int col = idx % 5;
                int x = 18 + col * 8;
                int y = 7 + row * 6;

                SetColor(COLOR_WHITE, COLOR_BLACK);
                Gotoxy(x, y); printf("┌───┐");

                Gotoxy(x, y + 1);
                SetColor(COLOR_WHITE, COLOR_BLACK); printf("│ ");

                if (revealed[idx] == 2) SetColor(COLOR_MAGENTA, COLOR_BLACK);
                else SetColor(COLOR_WHITE, COLOR_BLACK);

                printf("%c", (revealed[idx] >= 1) ? '0' + cards[idx] : '?');
                SetColor(COLOR_WHITE, COLOR_BLACK); printf(" │");

                Gotoxy(x, y + 2); printf("└───┘");
                Gotoxy(x + 2, y + 3); printf("%d", (idx + 1) % 10);
            }
        }
        else {
            revealed[first] = revealed[second] = 0;
            tries--;
        }

        Gotoxy(25, 18); printf("                                      ");
        Gotoxy(25, 19); printf("                                      ");
    }

    return (matches >= 3) ? 1 : 0; // 3쌍 이상 찾으면 성공
}


// 스테이지 2: 리듬 게임 구현
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
    FILE* fp;

    system("cls");
    R_Note notes[100];
    int noteCount = 0;

    static int patternIndex = 0;
    patternIndex = (patternIndex % 3) + 1;

    char filename[20];
    sprintf(filename, "pattern%d.txt", patternIndex);

    // 리듬 패턴 파일 로딩 (없으면 기본 패턴 사용)
    fp = fopen(filename, "r");
    if (fp != NULL) {
        while (fscanf(fp, "%ld %d", &notes[noteCount].targetTime, &notes[noteCount].line) != EOF) {
            notes[noteCount].judged = 0;
            notes[noteCount].prevY = -1;
            noteCount++;
            if (noteCount >= 100) break;
        }
        fclose(fp);
    }
    else {
        R_Note default_notes[] = {
            {1000, 0, 0, -1}, {1500, 1, 0, -1}, {2000, 2, 0, -1},
            {2500, 3, 0, -1}, {3000, 0, 0, -1}, {3500, 1, 0, -1}
        };
        for (int i = 0; i < sizeof(default_notes) / sizeof(R_Note); i++) {
            notes[i] = default_notes[i];
        }
        noteCount = sizeof(default_notes) / sizeof(R_Note);
    }

    // 게임 화면 레이아웃 및 레인 그리기
    DrawLayout("스테이지 2: 리듬 게임", "타이밍에 맞춰 키를 누르세요!");
    SetColor(COLOR_CYAN, COLOR_BLACK);
    PrintCenter(4, "키: [D] [F] [J] [K]");

    for (int y = 5; y <= 22; y++) {
        for (int i = 0; i < 4; i++) {
            Gotoxy(R_LANE_START_X + (i * R_LANE_WIDTH), y); printf("|");
            Gotoxy(R_LANE_START_X + (i * R_LANE_WIDTH) + R_LANE_WIDTH, y); printf("|");
        }
    }

    // 판정선 및 키 표시
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

        // 키 입력 처리
        if (_kbhit()) {
            int key = _getch();
            int line = -1;
            if (key == 'd' || key == 'D') line = 0; else if (key == 'f' || key == 'F') line = 1;
            else if (key == 'j' || key == 'J') line = 2; else if (key == 'k' || key == 'K') line = 3;
            else if (key == 'q' || key == 'Q') { gameRunning = 0; break; }

            if (line != -1) {
                // 입력된 키에 해당하는 노트 판정
                int hitIdx = -1; long minDiff = 9999;
                for (int i = 0; i < noteCount; i++) {
                    if (notes[i].line == line && !notes[i].judged) {
                        long diff = abs(currentTime - notes[i].targetTime);
                        if (diff < 400) {
                            if (diff < minDiff) { minDiff = diff; hitIdx = i; }
                        }
                    }
                }

                // 판정 결과에 따른 점수 및 콤보 처리
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
                else {
                }
            }
        }

        // 노트 이동 및 미스 판정 로직
        for (int i = 0; i < noteCount; i++) {
            if (notes[i].judged) continue;
            allFinished = 0;
            long diff = notes[i].targetTime - currentTime;

            // 미스 판정
            if (diff < -300) {
                notes[i].judged = 1; combo = 0; strcpy(msg, "놓침    ");
                if (notes[i].prevY >= 5 && notes[i].prevY < 24) {
                    Gotoxy(R_LANE_START_X + (notes[i].line * R_LANE_WIDTH) + 2, notes[i].prevY); printf("  ");
                }
                continue;
            }

            // 노트 위치 계산 및 화면에 그리기
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

        // 점수 및 상태 표시 업데이트
        SetColor(COLOR_WHITE, COLOR_BLACK);
        Gotoxy(50, 10); printf("점수 : %d    ", score);
        Gotoxy(50, 12); printf("콤보 : %d    ", combo);
        Gotoxy(50, 14); printf("%s        ", msg);

        if (allFinished) { Sleep(1000); break; }
        Sleep(30);
    }

    return (score >= 300) ? 1 : 0; // 300점 이상이면 성공
}


// 스테이지 3: 논리 퍼즐 (Sequence Game) 구현
int PlaySequenceGame() {
    // 기본 시퀀스와 맵핑 배열 초기화 및 섞기
    int base_seq[5] = { 1, 4, 3, 5, 2 };
    int map[6]; for (int i = 1; i <= 5; i++) map[i] = i;
    for (int i = 5; i >= 2; i--) { int j = rand() % i + 1; int t = map[i]; map[i] = map[j]; map[j] = t; }
    int answer[5]; for (int i = 0; i < 5; i++) answer[i] = map[base_seq[i]];
    int order[5] = { 0, 1, 2, 3, 4 };
    for (int i = 4; i > 0; i--) { int j = rand() % (i + 1); int t = order[i]; order[i] = order[j]; order[j] = t; }

    int input[5], tries = 3;

    // 초기 퍼즐 화면 출력
    draw_puzzle_screen(tries, map, order);

    while (tries > 0) {
        char msg[30]; sprintf(msg, "남은 시도: %d", tries);
        UpdateStatusBar(msg, "형식: 1 2 3 4 5");

        Gotoxy(15, 20); printf("코드 입력 (예: 4 2 3 1 5):                                         "); Gotoxy(44, 20);

        // 정답 입력 및 유효성 검사
        if (scanf_s("%d %d %d %d %d", &input[0], &input[1], &input[2], &input[3], &input[4]) != 5) {
            while (getchar() != '\n' && getchar() != EOF);
            ShowPopup("오류", "잘못된 형식입니다!");
            draw_puzzle_screen(tries, map, order);
            continue;
        }
        while (getchar() != '\n' && getchar() != EOF);

        // 정답 비교
        int correct = 1; for (int i = 0; i < 5; i++) if (input[i] != answer[i]) { correct = 0; break; }

        if (correct) return 1; // 성공
        else {
            tries--; ShowPopup("틀림", "잘못된 순서입니다."); // 실패, 시도 횟수 감소
            if (tries > 0) {
                draw_puzzle_screen(tries, map, order);
            }
        }
    }
    return 0; // 시도 횟수 초과 시 실패
}


// 스테이지 4: 기억력 게임 구현
int PlayMemoryGame() {
    int round_lengths[] = { 3, 5, 7 };
    int total_rounds = 3;
    int sequence[20];
    char input[100];

    for (int round = 0; round < total_rounds; round++) {
        int length = round_lengths[round];
        // 시퀀스 랜덤 생성
        for (int i = 0; i < length; i++) sequence[i] = rand() % 10;

        char title[50]; sprintf(title, "스테이지 4: 기억력 (라운드 %d)", round + 1);

        // 숫자 시퀀스 표시
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

        // 사용자 입력 받기
        DrawLayout(title, "기억한 숫자를 순서대로 입력하세요.");
        UpdateStatusBar("입력 완료 후 엔터", "띄어쓰기 없이 입력 (예: 123)");

        Gotoxy(25, 12); printf("정답 입력: ");

        if (scanf_s("%s", input, (unsigned int)sizeof(input)) != 1) {
            while (getchar() != '\n' && getchar() != EOF); return 0;
        }
        while (getchar() != '\n' && getchar() != EOF);

        // 입력된 정답 비교
        int correct = 1;
        if (strlen(input) != length) correct = 0;
        else {
            for (int i = 0; i < length; i++) {
                if (input[i] != sequence[i] + '0') {
                    correct = 0; break;
                }
            }
        }

        // 결과 처리 (다음 라운드 진행 또는 실패)
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
    return 1; // 3 라운드 모두 성공
}


// 스테이지 5: 보스 미로 게임 구현
#define COLOR_DEFAULT_TEXT 15
#define BG_COLOR 0
#define B_WIDTH 80
#define B_HEIGHT 25
#define B_WALL_CHAR "█"
#define B_PLAYER_CHAR "O"
#define B_ENEMY_CHAR "X"
#define B_ITEM_CHAR "$"

// 벽/아이템 위치 구조체
typedef struct { int x, y; } BWall;
typedef struct { int x, y, active; } BItem;

// 콘솔 제어 함수 (Gotoxy와 SetColor의 보스 게임 버전)
void Gotox_y(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void b_set_color(int textColor, int bgColor) {
    int color = textColor + (bgColor * 16);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void b_set_cursor(int x, int y) { Gotox_y(x, y); }

// 벽 체크
int b_is_wall(int x, int y, BWall walls[], int count) {
    for (int i = 0; i < count; i++)
        if (x == walls[i].x && y == walls[i].y) return 1;
    return 0;
}

// 벽 그리기
void b_draw_walls(BWall walls[], int count) {
    b_set_color(8, BG_COLOR);
    for (int i = 0; i < count; i++) {
        b_set_cursor(walls[i].x, walls[i].y);
        printf("%s", B_WALL_CHAR);
    }
    b_set_color(COLOR_DEFAULT_TEXT, BG_COLOR);
}

// 아이템 그리기
void b_draw_items(BItem items[], int count) {
    b_set_color(14, BG_COLOR);
    for (int i = 0; i < count; i++) {
        if (items[i].active) {
            b_set_cursor(items[i].x, items[i].y);
            printf("%s", B_ITEM_CHAR);
        }
    }
    b_set_color(COLOR_DEFAULT_TEXT, BG_COLOR);
}

// 아이템 위치 초기화 (벽, 플레이어/보스 위치를 피해 랜덤 배치)
void b_reset_items(BItem items[], int count, BWall walls[], int wallCount, int px, int py, int ex, int ey) {
    for (int i = 0; i < count; i++) {
        int x, y, valid;
        do {
            valid = 1;
            x = rand() % B_WIDTH;
            y = rand() % (B_HEIGHT - 2) + 1;

            if (x == 0 || x == B_WIDTH - 1 || y == 1 || y == B_HEIGHT - 2) { valid = 0; continue; }

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

// 스테이지 5: 보스 미로 게임 (라운드별 난이도 증가)
int PlayBossGame(int current_round) {
    int px, py, ex, ey;
    int ch;
    DWORD lastMoveTime;

    // 라운드에 따라 보스 속도 계산 (난이도 증가)
    int baseSpeed = 300;
    for (int i = 1; i < current_round; i++) {
        baseSpeed = (int)(baseSpeed * 0.6);
    }
    const int rounds_to_win = 3;

    int score = 0;

    // 미로 레이아웃 (벽 위치 정의)
    BWall walls[] = {
        {0,1},{0,2},{0,3},{0,4},{0,5},{0,6},{0,7},{0,8},{0,9},{0,10},{0,11},{0,12},{0,13},{0,14},{0,15},{0,16},{0,17},{0,18},{0,19},{0,20},{0,21},{0,22},
        {79,1},{79,2},{79,3},{79,4},{79,5},{79,6},{79,7},{79,8},{79,9},{79,10},{79,11},{79,12},{79,13},{79,14},{79,15},{79,16},{79,17},{79,18},{79,19},{79,20},{79,21},{79,22},
        {1,1},{2,1},{3,1},{4,1},{5,1},{6,1},{7,1},{8,1},{9,1},{10,1},{11,1},{12,1},{13,1},{14,1},{15,1},{16,1},{17,1},{18,1},{19,1},{20,1},{21,1},{22,1},{23,1},{24,1},{25,1},{26,1},{27,1},{28,1},{29,1},{30,1},{31,1},{32,1},{33,1},{34,1},{35,1},{36,1},{37,1},{38,1},{39,1},{40,1},{41,1},{42,1},{43,1},{44,1},{45,1},{46,1},{47,1},{48,1},{49,1},{50,1},{51,1},{52,1},{53,1},{54,1},{55,1},{56,1},{57,1},{58,1},{59,1},{60,1},{61,1},{62,1},{63,1},{64,1},{65,1},{66,1},{67,1},{68,1},{69,1},{70,1},{71,1},{72,1},{73,1},{74,1},{75,1},{76,1},{77,1},{78,1},
        {1,22},{2,22},{3,22},{4,22},{5,22},{6,22},{7,22},{8,22},{9,22},{10,22},{11,22},{12,22},{13,22},{14,22},{15,22},{16,22},{17,22},{18,22},{19,22},{20,22},{21,22},{22,22},{23,22},{24,22},{25,22},{26,22},{27,22},{28,22},{29,22},{30,22},{31,22},{32,22},{33,22},{34,22},{35,22},{36,22},{37,22},{38,22},{39,22},{40,22},{41,22},{42,22},{43,22},{44,22},{45,22},{46,22},{47,22},{48,22},{49,22},{50,22},{51,22},{52,22},{53,22},{54,22},{55,22},{56,22},{57,22},{58,22},{59,22},{60,22},{61,22},{62,22},{63,22},{64,22},{65,22},{66,22},{67,22},{68,22},{69,22},{70,22},{71,22},{72,22},{73,22},{74,22},{75,22},{76,22},{77,22},{78,22},
        {10,3},{15,5},{20,8},{35,6},{40,10},{45,15},{60,7},{62,8},{64,9},
        {25,18},{30,20},{50,20},{12,12},{18,14},{22,16},{70,5},{72,9},{74,12},
        {10,4},{10,5},{10,6},{10,7},{10,8},{10,9},
        {60,15},{60,16},{60,17},{60,18},{60,19},{60,20},
        {30,15},{31,15},{32,15},{33,15},{34,15},{35,15}
    };
    int wallCount = sizeof(walls) / sizeof(walls[0]);
    BItem items[3];

    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci;
    GetConsoleCursorInfo(out, &ci);
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(out, &ci);

    b_set_color(COLOR_DEFAULT_TEXT, BG_COLOR);

    // 라운드 초기화 (위치, 아이템, 화면)
    system("cls");
    px = 5; py = 5;
    ex = 70; ey = 20;
    score = 0;

    b_reset_items(items, 3, walls, wallCount, px, py, ex, ey);
    b_draw_walls(walls, wallCount);
    b_draw_items(items, 3);

    lastMoveTime = GetTickCount();

    // 플레이어/보스 초기 위치 표시
    b_set_color(3, BG_COLOR);
    b_set_cursor(px, py); printf("%s", B_PLAYER_CHAR);

    b_set_color(12, BG_COLOR);
    b_set_cursor(ex, ey); printf("%s", B_ENEMY_CHAR);
    b_set_color(COLOR_DEFAULT_TEXT, BG_COLOR);

    // 상태 표시줄 업데이트
    b_set_cursor(0, 0);
    char statusMsg[80];
    sprintf(statusMsg, "스테이지 5: 미로 탈출 | 라운드 %d | 속도: %.2fx", current_round, (1000.0 / baseSpeed));
    printf("%-79s", statusMsg);

    b_set_cursor(0, B_HEIGHT - 1);
    printf("아이템 획득! (%d / 3) | W, A, S, D 또는 방향키로 이동. Q 종료.                                 ", score);

    // 게임 루프
    while (score < 3) {
        // 플레이어 키 입력 처리 및 이동
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

            // 이동 유효성 검사 (경계, 벽)
            if (nx < 1 || ny < 2 || nx >= B_WIDTH - 1 || ny >= B_HEIGHT - 1) continue;
            if (b_is_wall(nx, ny, walls, wallCount)) continue;

            b_set_cursor(px, py); printf(" ");
            px = nx; py = ny;
            b_set_color(3, BG_COLOR);
            b_set_cursor(px, py); printf("%s", B_PLAYER_CHAR);
            b_set_color(COLOR_DEFAULT_TEXT, BG_COLOR);
        }

        // 아이템 획득 처리
        for (int i = 0; i < 3; i++) {
            if (items[i].active && px == items[i].x && py == items[i].y) {
                items[i].active = 0;
                b_set_cursor(items[i].x, items[i].y); printf(" ");
                score++;

                b_set_cursor(0, B_HEIGHT - 1);
                printf("아이템 획득! (%d / 3) | W, A, S, D 또는 방향키로 이동. Q 종료.                                 ", score);
            }
        }

        // 보스 움직임 (타이머 기반 추격)
        if (GetTickCount() - lastMoveTime > baseSpeed) {
            int dx = 0, dy = 0;
            if (ex < px) dx = 1;
            if (ex > px) dx = -1;
            if (ey < py) dy = 1;
            if (ey > py) dy = -1;

            int nex = ex + dx;
            int ney = ey + dy;

            // 보스 이동 유효성 검사 (벽, 아이템, 경계)
            int onWall = b_is_wall(nex, ney, walls, wallCount);
            int onItem = 0;
            for (int i = 0; i < 3; i++)
                if (items[i].active && items[i].x == nex && items[i].y == ney)
                    onItem = 1;

            if (nex >= 1 && ney >= 2 && nex < B_WIDTH - 1 && ney < B_HEIGHT - 1 && !onWall && !onItem) {
                b_set_cursor(ex, ey); printf(" ");
                ex = nex; ey = ney;
                b_set_color(12, BG_COLOR);
                b_set_cursor(ex, ey); printf("%s", B_ENEMY_CHAR);
                b_set_color(COLOR_DEFAULT_TEXT, BG_COLOR);
            }
            else {
                // 막혔을 경우 한 방향만 이동 시도
                if (!b_is_wall(ex + dx, ey, walls, wallCount)) {
                    if (ex + dx >= 1 && ex + dx < B_WIDTH - 1) {
                        b_set_cursor(ex, ey); printf(" ");
                        ex = ex + dx;
                    }
                }
                else if (!b_is_wall(ex, ey + dy, walls, wallCount)) {
                    if (ey + dy >= 2 && ey + dy < B_HEIGHT - 1) {
                        b_set_cursor(ex, ey); printf(" ");
                        ey = ey + dy;
                    }
                }

                b_set_color(12, BG_COLOR);
                b_set_cursor(ex, ey); printf("%s", B_ENEMY_CHAR);
                b_set_color(COLOR_DEFAULT_TEXT, BG_COLOR);
            }

            lastMoveTime = GetTickCount();
        }

        // 보스가 플레이어를 잡았는지 확인 (실패 조건)
        if (abs(ex - px) <= 1 && abs(ey - py) <= 1) {
            b_set_color(12, BG_COLOR);
            b_set_cursor(0, B_HEIGHT - 1);
            printf("보스에게 잡혔습니다! 실패...                                                                     \n");
            Sleep(1200);
            ci.bVisible = TRUE;
            SetConsoleCursorInfo(out, &ci);
            system("cls");
            return 0;
        }

        Sleep(10);
    }

    // 라운드 클리어 처리
    ci.bVisible = TRUE;
    SetConsoleCursorInfo(out, &ci);

    if (current_round < rounds_to_win) {
        ShowPopup("라운드 클리어", "다음 라운드 시작!");
    }

    return 1;
}

// PlaySequenceGame 도우미 함수 정의
void draw_puzzle_screen(int current_tries, int map[], int order[]) {
    // 논리 퍼즐 게임의 레이아웃 및 힌트를 화면에 출력
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
}
