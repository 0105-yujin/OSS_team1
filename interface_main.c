#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include "interface.h"

int main() {
    InitUI();

    DrawLayout("ESCAPE ROOM", "Press Any Key to Start");
    getch();

    DrawLayout("STAGE 1: CARD GAME", "Find pairs!");
    UpdateStatusBar("Score: 0", "Arrow Keys: Move");

    for (int i = 0; i <= 100; i += 10) {
        PrintCenter(10, "Game is running...");
        
        char scoreText[20];
        sprintf(scoreText, "Score: %d", i);
        UpdateStatusBar(scoreText, "Running...");

        Gotoxy(30 + (i / 5), 12);
        printf(">");
        
        Sleep(100);
    }

    ShowPopup("CLEAR!", "You found a clue: [ 7 ]");

    DrawLayout("THE END", "Thank you for playing.");
    getch();

    return 0;
}