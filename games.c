#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include "interface.h"
#include "games.h"

int PlayRhythmGame() {
    DrawLayout("STAGE 2: RHYTHM GAME", "Press Space when '>' meets '<'");
    UpdateStatusBar("Score: 0", "Space: HIT | Q: Quit");

    int score = 0;
    int combo = 0;
    
    for(int i=3; i>0; i--) {
        char countStr[10];
        sprintf(countStr, "%d", i);
        PrintCenter(10, countStr);
        Sleep(1000);
    }
    PrintCenter(10, "START!");
    Sleep(500);
    PrintCenter(10, "      "); 

    for (int i = 0; i < 10; i++) {
        for (int x = 10; x < 40; x += 2) {
            Gotoxy(x, 12); printf(">");      
            Gotoxy(80-x, 12); printf("<");   
            Gotoxy(40, 12); printf("[ ]");   
            Sleep(50); 
            Gotoxy(x, 12); printf(" ");
            Gotoxy(80-x, 12); printf(" ");
        }
        Gotoxy(40, 12); printf("[O]"); 
        
        int hit = 0;
        for(int w=0; w<20; w++) {
            if (kbhit()) {
                char key = getch();
                if (key == ' ') { hit = 1; break; } 
                if (key == 'q') return 0; 
            }
            Sleep(10);
        }

        if (hit) {
            score += 100;
            combo++;
            UpdateStatusBar("HIT!", "NICE TIMING!");
        } else {
            combo = 0;
            UpdateStatusBar("MISS...", "Focus!");
        }

        char scoreText[30];
        sprintf(scoreText, "Score: %d | Combo: %d", score, combo);
        UpdateStatusBar(scoreText, "Space: HIT");
        Sleep(500);
        Gotoxy(40, 12); printf("   "); 
    }

    if (score >= 500) return 1;
    else return 0;
}

int PlaySequenceGame() {
    srand(time(NULL));

    int base_seq[5] = { 1, 4, 3, 5, 2 };
    int map[6]; 
    for (int i = 1; i <= 5; i++) map[i] = i;
    
    for (int i = 5; i >= 2; i--) {
        int j = rand() % i + 1;
        int tmp = map[i];
        map[i] = map[j];
        map[j] = tmp;
    }

    int answer[5];
    for (int i = 0; i < 5; i++) {
        answer[i] = map[base_seq[i]];
    }

    int order[5] = { 0, 1, 2, 3, 4 };
    for (int i = 4; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = order[i];
        order[i] = order[j];
        order[j] = tmp;
    }

    DrawLayout("STAGE 3: LOGIC PUZZLE", "Analyze hints and find the order.");
    UpdateStatusBar("Input: 1 2 3 4 5", "Focus on logic");

    int startY = 7;
    Gotoxy(5, startY - 2); printf("[ HINTS ]");
    
    for (int k = 0; k < 5; k++) {
        Gotoxy(5, startY + (k * 2)); 
        printf("%d. ", k + 1);

        switch (order[k]) {
        case 0:
            printf("Number %d is at the 1st position.", map[1]);
            break;
        case 1:
            printf("Number %d is immediately before %d.", map[4], map[3]);
            break;
        case 2:
            printf("Number %d comes before %d.", map[3], map[5]);
            break;
        case 3:
            printf("Number %d is immediately before %d.", map[5], map[2]);
            break;
        case 4:
            printf("Number %d comes before %d.", map[1], map[5]);
            break;
        }
    }

    int input[5];
    int tries = 3; 

    while (tries > 0) {
        char msg[30];
        sprintf(msg, "Tries Left: %d", tries);
        UpdateStatusBar(msg, "Format: 1 2 3 4 5");

        Gotoxy(15, 20);
        printf("Enter Code (e.g. 4 2 3 1 5):                   "); 
        Gotoxy(44, 20); 

        if (scanf("%d %d %d %d %d", &input[0], &input[1], &input[2], &input[3], &input[4]) != 5) {
            while (getchar() != '\n'); 
            ShowPopup("ERROR", "Invalid Format!");
            DrawLayout("STAGE 3: LOGIC PUZZLE", "Analyze hints and find the order."); 
            
            Gotoxy(5, startY - 2); printf("[ HINTS ]");
            for (int k = 0; k < 5; k++) {
                Gotoxy(5, startY + (k * 2));
                printf("%d. ", k + 1);
                switch (order[k]) {
                    case 0: printf("Number %d is at the 1st position.", map[1]); break;
                    case 1: printf("Number %d is immediately before %d.", map[4], map[3]); break;
                    case 2: printf("Number %d comes before %d.", map[3], map[5]); break;
                    case 3: printf("Number %d is immediately before %d.", map[5], map[2]); break;
                    case 4: printf("Number %d comes before %d.", map[1], map[5]); break;
                }
            }
            continue;
        }
        while (getchar() != '\n'); 

        int correct = 1;
        for (int i = 0; i < 5; i++) {
            if (input[i] != answer[i]) {
                correct = 0;
                break;
            }
        }

        if (correct) {
            return 1; 
        } else {
            tries--;
            ShowPopup("WRONG", "Incorrect sequence.");
            
             DrawLayout("STAGE 3: LOGIC PUZZLE", "Analyze hints and find the order."); 
            
            Gotoxy(5, startY - 2); printf("[ HINTS ]");
            for (int k = 0; k < 5; k++) {
                Gotoxy(5, startY + (k * 2));
                printf("%d. ", k + 1);
                switch (order[k]) {
                    case 0: printf("Number %d is at the 1st position.", map[1]); break;
                    case 1: printf("Number %d is immediately before %d.", map[4], map[3]); break;
                    case 2: printf("Number %d comes before %d.", map[3], map[5]); break;
                    case 3: printf("Number %d is immediately before %d.", map[5], map[2]); break;
                    case 4: printf("Number %d comes before %d.", map[1], map[5]); break;
                }
            }
        }
    }

    return 0; 
}