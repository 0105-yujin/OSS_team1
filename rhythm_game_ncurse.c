#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>    
#include <sys/time.h>   
#include <ncurses.h>    // ncurses 라이브러리
#include <string.h>     

  

#define PERFECT_WINDOW 50
#define GOOD_WINDOW 100
#define MISS_WINDOW 200


#define GAME_HEIGHT 20      
#define JUDGE_LINE_Y GAME_HEIGHT 
#define LANE_0_X 10        
#define LANE_WIDTH 5       


#define LOOKAHEAD_MS 2000 


typedef struct {
    unsigned long time_ms; 
    int line;      
    bool judged;   
} Note;


int score = 0;
int combo = 0;
int maxCombo = 0;
unsigned long startTime; 
char last_judgment[40] = ""; 

unsigned long get_ms_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (unsigned long)(tv.tv_sec) * 1000 + (unsigned long)(tv.tv_usec) / 1000;
}

unsigned long getCurrentTime_ms() {
    return get_ms_time() - startTime;
}

void judgeMiss(Note* note) {
    if (note->judged) return;
    note->judged = true;
    combo = 0;
    
    
    sprintf(last_judgment, "[%07lums] Line %d: MISS", note->time_ms, note->line);
}

void judgeHit(unsigned long hitTime_ms, int line, Note* pattern, int noteCount) {
    int bestNoteIndex = -1;
    unsigned long minDiff = 10000; 

    for (int i = 0; i < noteCount; i++) {
        if (pattern[i].line == line && !pattern[i].judged) {

            unsigned long diff_check = (hitTime_ms > pattern[i].time_ms) ? 
                                       (hitTime_ms - pattern[i].time_ms) : 
                                       (pattern[i].time_ms - hitTime_ms);
            
            if (diff_check <= MISS_WINDOW && diff_check < minDiff) {
                minDiff = diff_check;
                bestNoteIndex = i;
            }
        }
    }

    if (bestNoteIndex == -1) {
        sprintf(last_judgment, "[%07lums] Line %d: (Bad Hit)", hitTime_ms, line);
        return;
    }

    Note* targetNote = &pattern[bestNoteIndex];
    unsigned long diff = minDiff; 

    if (diff <= PERFECT_WINDOW) {
        sprintf(last_judgment, "[%07lums] Line %d: PERFECT! (%dms)", hitTime_ms, line, (int)diff);
        score += 100;
        combo++;
    } else if (diff <= GOOD_WINDOW) {
        sprintf(last_judgment, "[%07lums] Line %d: Good. (%dms)", hitTime_ms, line, (int)diff);
        score += 50;
        combo++;
    } else {
        sprintf(last_judgment, "[%07lums] Line %d: Miss. (%dms)", hitTime_ms, line, (int)diff);
        combo = 0;
    }
    
    targetNote->judged = true;
    if (combo > maxCombo) {
        maxCombo = combo;
    }
}


void draw_board() {
    // 판정선 그리기 
    mvprintw(JUDGE_LINE_Y, LANE_0_X - 2, "[d]");
    mvprintw(JUDGE_LINE_Y, LANE_0_X + LANE_WIDTH - 2, "[f]");
    mvprintw(JUDGE_LINE_Y, LANE_0_X + LANE_WIDTH * 2 - 2, "[j]");
    mvprintw(JUDGE_LINE_Y, LANE_0_X + LANE_WIDTH * 3 - 2, "[k]");
    
    for (int x = 0; x < 40; x++) {
        mvprintw(JUDGE_LINE_Y + 1, x, "-"); 
    }

    //  레인 경계 그리기 
    for (int y = 0; y < JUDGE_LINE_Y; y++) {
        mvprintw(y, LANE_0_X - 3, "|");
        mvprintw(y, LANE_0_X + LANE_WIDTH * 4 - 3, "|");
    }

    // 점수 및 상태 표시 
    mvprintw(2, 40, "Score: %d", score);
    mvprintw(3, 40, "Combo: %d", combo);
    mvprintw(5, 40, "Last: %s", last_judgment);
    mvprintw(7, 40, "Time: %.1f s", (float)getCurrentTime_ms() / 1000.0);
}


void draw_notes(Note* pattern, int noteCount, unsigned long currentTime_ms) {
    for (int i = 0; i < noteCount; i++) {
        if (pattern[i].judged) continue; 

        // 노트 시간과 현재 시간의 차이 
        long time_diff = (long)pattern[i].time_ms - (long)currentTime_ms;

        // 화면에 보일 시간 
        if (time_diff < LOOKAHEAD_MS && time_diff > -MISS_WINDOW) {
            
            // 판정선 비율 계산
            double y_percent = 1.0 - ((double)time_diff / LOOKAHEAD_MS);
            
            // 실제 y좌표 
            int y = (int)(y_percent * JUDGE_LINE_Y);
            
            // 레인 x좌표 
            int x = LANE_0_X + pattern[i].line * LANE_WIDTH - 2; 

            if (y >= 0 && y <= JUDGE_LINE_Y + 1) { // 판정선 넘어도
                mvprintw(y, x, " O ");
            }
        }
    }
}



int main() {
    
    Note pattern[] = {
        {2000, 0, false}, {2500, 1, false}, {3000, 2, false}, {3500, 3, false},
        {4000, 0, false}, {4000, 1, false}, {4500, 2, false}, {4600, 3, false},
        {4700, 2, false}, {5500, 1, false}, {6000, 0, false},
    };
    int noteCount = sizeof(pattern) / sizeof(Note);

   
    initscr();              
    cbreak();               
    noecho();               
    keypad(stdscr, TRUE);   
    nodelay(stdscr, TRUE);  
    curs_set(0);            

    mvprintw(5, 5, "--- 리듬 게임 시작 ---");
    mvprintw(6, 5, "키: [d] [f] [j] [k] (종료: q)");
    mvprintw(8, 5, "3초 후 시작합니다...");
    refresh(); 
    sleep(3); 

    startTime = get_ms_time(); 
    unsigned long currentTime_ms = 0;
    
    int judgeIndex = 0; 
    bool gameRunning = true;

    while (gameRunning) {
        clear(); 
        
        currentTime_ms = getCurrentTime_ms();

        //  키보드 입력 확인 
        int ch = getch(); 
        if (ch != ERR) { 
            int line = -1;
            if (ch == 'd') line = 0;
            else if (ch == 'f') line = 1;
            else if (ch == 'j') line = 2;
            else if (ch == 'k') line = 3;
            else if (ch == 'q') { 
                gameRunning = false;
                break;
            }

            if (line != -1) {
                judgeHit(currentTime_ms, line, pattern, noteCount);
            }
        }

        // Miss 판정 
        while (judgeIndex < noteCount && 
               currentTime_ms > pattern[judgeIndex].time_ms + MISS_WINDOW) {
            
            if (!pattern[judgeIndex].judged) {
                judgeMiss(&pattern[judgeIndex]);
            }
            judgeIndex++;
        }

        
        draw_board(); 
        draw_notes(pattern, noteCount, currentTime_ms); 

        
        refresh(); 

        
        if (judgeIndex >= noteCount) {
            gameRunning = false;
            sleep(2); 
        }

        usleep(10000); 
        }

   
    endwin();
    
    printf("\n--- 게임 종료 ---\n"); 
    if (combo > maxCombo) {
        maxCombo = combo;
    }
    
    bool isPass = (score >= 150); 

    printf("최종 점수: %d\n", score);
    printf("최대 콤보: %d\n", maxCombo);
    printf("통과 여부: %s\n", isPass ? "Pass" : "Fail");

    return 0;
}