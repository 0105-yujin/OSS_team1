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
    ret
