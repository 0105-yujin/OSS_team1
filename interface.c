#include <stdio.h>
#include <conio.h>
#include "interface.h"

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

    getch();

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