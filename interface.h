#ifndef INTERFACE_H
#define INTERFACE_H

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

#endif