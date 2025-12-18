#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH  80
#define HEIGHT 25
#define WALL_CHAR "#"
#define PLAYER_CHAR "o"
#define ENEMY_CHAR "X"
#define ITEM_CHAR "$"




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
typedef struct { int x, y; int active; } BItem;

// 콘솔 제어 함수
void Gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//  2개 인자 색상 함수
void b_set_color(int textColor, int bgColor) {
    int color = textColor + (bgColor * 16);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void b_set_cursor(int x, int y) { Gotoxy(x, y); }

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
            y = rand() % (B_HEIGHT - 1);

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

// 보스게임 실행
int PlayBossGame() {
    int px, py, ex, ey;
    int ch, round = 1;
    DWORD lastMoveTime;
    int baseSpeed = 300;
    int score = 0;
    const int rounds_to_win = 3;

    BWall walls[] = {
        {10,3},{15,5},{20,8},{35,6},{40,10},{45,15},{60,7},{62,8},{64,9},
        {25,18},{30,20},{50,22},{12,12},{18,14},{22,16},{70,5},{72,9},{74,12}
    };
    int wallCount = sizeof(walls) / sizeof(walls[0]);
    BItem items[3];

    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci;
    GetConsoleCursorInfo(out, &ci);
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(out, &ci);

    //  시작 시 기본 텍스트 색상을 밝은 흰색으로 설정
    b_set_color(COLOR_DEFAULT_TEXT, BG_COLOR);

restart_round:
    system("cls");
    px = 5; py = 5;
    ex = 70; ey = 20;
    score = 0;

    b_reset_items(items, 3, walls, wallCount, px, py, ex, ey);
    b_draw_walls(walls, wallCount);
    b_draw_items(items, 3);

    lastMoveTime = GetTickCount();

    // 플레이어 색상 적용 (청록색 3)
    b_set_color(3, BG_COLOR);
    b_set_cursor(px, py); printf("%s", B_PLAYER_CHAR);

    // 보스 색상 적용 (빨강 12)
    b_set_color(12, BG_COLOR);
    b_set_cursor(ex, ey); printf("%s", B_ENEMY_CHAR);
    b_set_color(COLOR_DEFAULT_TEXT, BG_COLOR); // 기본 색상으로 복구

    //  상태 표시줄 (B_HEIGHT - 1) 강조
    b_set_cursor(0, B_HEIGHT - 1);
    printf("Round %d 시작! 보스 속도: %.2fx", round, (1000.0 / baseSpeed));

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
                ci.bVisible = TRUE;
                SetConsoleCursorInfo(out, &ci);
                return 0;
            }

            if (nx < 0 || ny < 0 || nx >= B_WIDTH || ny >= B_HEIGHT - 1) continue;
            if (b_is_wall(nx, ny, walls, wallCount)) continue;

            if (py < B_HEIGHT - 1) {
                b_set_cursor(px, py); printf(" ");
            }

            px = nx; py = ny;
            b_set_color(3, BG_COLOR); // 플레이어 색상 설정
            b_set_cursor(px, py); printf("%s", B_PLAYER_CHAR);
            b_set_color(COLOR_DEFAULT_TEXT, BG_COLOR); // 기본 색상으로 복구
        }

        // 아이템 획득
        for (int i = 0; i < 3; i++) {
            if (items[i].active && px == items[i].x && py == items[i].y) {
                items[i].active = 0;
                b_set_cursor(items[i].x, items[i].y); printf(" ");
                score++;

                b_set_cursor(0, B_HEIGHT - 1);
                printf("아이템 획득! (%d / 3)      ", score); // 지워지는 영역 확보
            }
        }

        // 보스 움직임
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

            if (ney < B_HEIGHT - 1 && !b_is_wall(nex, ney, walls, wallCount) && !onItem) {
                if (ey < B_HEIGHT - 1) {
                    b_set_cursor(ex, ey); printf(" ");
                }
                ex = nex; ey = ney;
                b_set_color(12, BG_COLOR); // 보스 색상 설정
                b_set_cursor(ex, ey); printf("%s", B_ENEMY_CHAR);
                b_set_color(COLOR_DEFAULT_TEXT, BG_COLOR); // 기본 색상으로 복구
            }
            lastMoveTime = GetTickCount();
        }

        // 보스가 플레이어 잡음
        if (abs(ex - px) < 1 && abs(ey - py) < 1) {
            b_set_color(12, BG_COLOR); // 실패 메시지는 빨간색
            b_set_cursor(0, B_HEIGHT - 1);
            printf("\n보스에게 잡혔습니다! 실패...                                  \n");
            Sleep(1200);
            ci.bVisible = TRUE;
            SetConsoleCursorInfo(out, &ci);
            return 0;
        }

        // 라운드 클리어
        if (score == 3) {
            round++;
            if (round > rounds_to_win) {
                b_set_color(10, BG_COLOR); // 성공 메시지는 밝은 초록색
                b_set_cursor(0, B_HEIGHT - 1);
                printf("\n보스전 클리어! 축하합니다.                                   \n");
                Sleep(1000);
                ci.bVisible = TRUE;
                SetConsoleCursorInfo(out, &ci);
                return 1;
            }

            baseSpeed = (int)(baseSpeed * 0.6);
            b_set_color(10, BG_COLOR); // 라운드 클리어 메시지는 밝은 초록색
            b_set_cursor(0, B_HEIGHT - 1);
            printf("\n아이템 모두 획득! 다음 라운드 시작...                             \n");
            Sleep(1200);
            goto restart_round;
        }

        Sleep(10);
    }
    return 0;
}

