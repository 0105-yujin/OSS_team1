#include <stdio.h>
#include <conio.h> // _getch 쓰려면 필요
#include "interface.h"
#include "games.h"

int main() {
    InitUI();

    while (1) {
        DrawLayout("NOVA APERIO", "Escape Room Project");
        PrintCenter(10, "1. GAME START");
        PrintCenter(12, "2. EXIT");
        UpdateStatusBar("Select Number", "Team Nova");

        // [수정] getch -> _getch
        char choice = _getch();

        if (choice == '2') {
            break;
        }
        else if (choice == '1') {
            DrawLayout("PROLOGUE", "Press Any Key...");
            PrintCenter(10, "You wake up in a locked room...");
            PrintCenter(12, "Find the clues to escape.");
            
            // [수정] getch -> _getch
            _getch();

            int result2 = PlayRhythmGame();
            if (result2 == 0) {
                ShowPopup("FAILED", "You failed the Rhythm Game.");
                continue;
            }
            ShowPopup("STAGE CLEAR", "Clue Found: [ 3 ]");

            int result3 = PlaySequenceGame();
            if (result3 == 0) {
                ShowPopup("FAILED", "You ran out of tries.");
                continue;
            }
            ShowPopup("STAGE CLEAR", "Clue Found: [ 9 ]");

            DrawLayout("THE END", "More stages coming soon...");
            
            // [수정] getch -> _getch
            _getch();
        }
    }

    CloseUI();
    return 0;
}