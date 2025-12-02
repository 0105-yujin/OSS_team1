// ★ 반드시 가장 위에 위치해야 함
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

typedef struct { int x, y; } Wall;
typedef struct { int x, y; int active; } Item;

void set_cursor(int x, int y) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(h, pos);
}

void set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int is_wall(int x, int y, Wall walls[], int count) {
    for (int i = 0; i < count; i++) {
        if (x == walls[i].x && y == walls[i].y)
            return 1;
    }
    return 0;
}

void draw_walls(Wall walls[], int count) {
    set_color(8);
    for (int i = 0; i < count; i++) {
        set_cursor(walls[i].x, walls[i].y);
        printf(WALL_CHAR);
    }
    set_color(7);
}

void draw_items(Item items[], int count) {
    set_color(14);
    for (int i = 0; i < count; i++) {
        if (items[i].active) {
            set_cursor(items[i].x, items[i].y);
            printf(ITEM_CHAR);
        }
    }
    set_color(7);
}

void reset_items(Item items[], int count, Wall walls[], int wallCount, int playerX, int playerY, int enemyX, int enemyY) {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < count; i++) {
        int x, y, valid;
        do {
            valid = 1;
            x = rand() % WIDTH;
            y = rand() % HEIGHT;
            for (int w = 0; w < wallCount; w++) {
                if (walls[w].x == x && walls[w].y == y) { valid = 0; break; }
            }
            if ((x == playerX && y == playerY) || (x == enemyX && y == enemyY)) valid = 0;
        } while (!valid);
        items[i].x = x;
        items[i].y = y;
        items[i].active = 1;
    }
}

int main(void) {

    // ★ UTF-8 활성화 (이모지 깨짐 방지)
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // ★ 콘솔 크기 강제 설정
    system("mode con: cols=80 lines=25");

    int x, y, ex, ey;
    int ch, round = 1;
    DWORD lastMoveTime;
    int baseSpeed = 300;
    int score = 0;

    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci;

    // ★ 커서 숨김 확실하게 적용
    GetConsoleCursorInfo(out, &ci);
    ci.dwSize = 1;
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(out, &ci);

    Wall walls[] = {
        {10,3},{15,5},{20,8},{35,6},{40,10},{45,15},{60,7},{62,8},{64,9},
        {25,18},{30,20},{50,22},{12,12},{18,14},{22,16},{70,5},{72,9},{74,12}
    };
    int wallCount = sizeof(walls)/sizeof(walls[0]);

    Item items[3];

restart_round:
    system("cls");

    x = 5; y = 5;
    ex = 70; ey = 20;
    score = 0;

    reset_items(items, 3, walls, wallCount, x, y, ex, ey);

    draw_walls(walls, wallCount);
    draw_items(items, 3);

    lastMoveTime = GetTickCount();
    set_cursor(x, y); printf(PLAYER_CHAR);
    set_cursor(ex, ey); printf(ENEMY_CHAR);

    set_cursor(0, HEIGHT);
    printf("Round %d 시작! 보스 속도: %.2fx\n", round, (1000.0 / baseSpeed));

    while (1) {
        if (_kbhit()) {
            ch = _getch();
            if (ch == 0 || ch == 224) ch = _getch();
            int nx = x, ny = y;
            if (ch=='w'||ch=='W'||ch==72) ny--;
            if (ch=='s'||ch=='S'||ch==80) ny++;
            if (ch=='a'||ch=='A'||ch==75) nx--;
            if (ch=='d'||ch=='D'||ch==77) nx++;
            if (ch=='q'||ch=='Q') goto end;

            if (nx<0||ny<0||nx>=WIDTH||ny>=HEIGHT) continue;
            if (is_wall(nx, ny, walls, wallCount)) continue;

            set_cursor(x, y); printf(" ");
            x = nx; y = ny;
            set_cursor(x, y); printf(PLAYER_CHAR);
        }

        for (int i=0;i<3;i++) {
            if (items[i].active && x==items[i].x && y==items[i].y) {
                items[i].active = 0;
                set_cursor(items[i].x, items[i].y); printf(" ");
                score++;
                set_cursor(0, HEIGHT);
                printf("아이템 획득! (%d / 3)\n", score);
            }
        }

        if (GetTickCount() - lastMoveTime > baseSpeed) {
            int dx=0, dy=0;
            if (ex < x) dx=1;
            if (ex > x) dx=-1;
            if (ey < y) dy=1;
            if (ey > y) dy=-1;

            int nex = ex + dx;
            int ney = ey + dy;

            int onItem = 0;
            for (int i=0;i<3;i++) {
                if (items[i].active && items[i].x==nex && items[i].y==ney) { onItem=1; break; }
            }

            if (!is_wall(nex, ney, walls, wallCount) && !onItem) {
                set_cursor(ex, ey); printf(" ");
                ex = nex; ey = ney;
                set_cursor(ex, ey); printf(ENEMY_CHAR);
            }

            lastMoveTime = GetTickCount();
        }

        if (abs(ex-x)<1 && abs(ey-y)<1) {
            set_cursor(0, HEIGHT);
            printf("\n 추격자에게 잡혔습니다! 게임 오버.\n");
            break;
        }

        if (score==3) {
            round++;
            baseSpeed = (int)(baseSpeed*0.6);
            set_cursor(0, HEIGHT);
            printf("\n 아이템 모두 획득! 다음 라운드 시작...\n");
            Sleep(2000);
            goto restart_round;
        }

        Sleep(10);
    }

end:
    ci.bVisible = TRUE;
    SetConsoleCursorInfo(out, &ci);
    set_cursor(0, HEIGHT);
    return 0;
}
