#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#define CLEAR "cls"
#else
#include <unistd.h>
#define CLEAR "clear"
#endif

// 각 라운드 숫자 개수 정의
int round_lengths[] = { 3, 5, 7 };
#define TOTAL_ROUNDS 3

// 랜덤 숫자 시퀀스 생성
void generate_sequence(int* seq, int length) {
    for (int i = 0; i < length; i++) {
        seq[i] = rand() % 10;
    }
}

// 숫자를 하나씩 2초 간격으로 출력
void show_sequence(int* seq, int length) {
    printf("기억하세요:\n");

    for (int i = 0; i < length; i++) {
        printf("%d\n", seq[i]);
        fflush(stdout);

#ifdef _WIN32
        Sleep(2000);   // 2초 대기
#else
        sleep(2);
#endif

        system(CLEAR); // 숫자 지우기 → 다음 숫자만 보이도록
    }
}

// 화면 지우기
void clear_screen() {
    system(CLEAR);
}

int main() {
    int sequence[20];
    int round = 0;
    char input[100];

    srand((unsigned int)time(NULL));

    printf("=== 숫자 외우기 게임 ===\n");
    printf("1라운드: 3개, 2라운드: 5개, 3라운드: 7개\n");
    printf("숫자가 하나씩 2초 간격으로 나타납니다.\n");
    printf("틀리면 게임 종료!\n\n");

    while (round < TOTAL_ROUNDS) {
        int length = round_lengths[round];

        printf("=== %d 라운드 (%d개) ===\n", round + 1, length);

        generate_sequence(sequence, length);
        show_sequence(sequence, length);
        clear_screen(); // 모든 숫자 지운 후 입력 시작

        printf("숫자를 입력하세요(띄어쓰기 없이): ");
#ifdef _WIN32
        if (scanf_s("%s", input, 100) != 1) {
#else
        if (scanf("%s", input) != 1) {
#endif
            printf("입력 오류!\n");
            return 1;
        }

        // 정답 비교
        int correct = 1;
        for (int i = 0; i < length; i++) {
            if (input[i] != sequence[i] + '0') {
                correct = 0;
                break;
            }
        }

        if (correct) {
            printf("정답! 다음 라운드로 갑니다.\n\n");
            round++;
        }
        else {
            printf("틀렸습니다! 게임 종료.\n정답: ");
            for (int i = 0; i < length; i++) printf("%d", sequence[i]);
            printf("\n");
            return 0;
        }
        }

    printf("축하합니다! 모든 라운드를 클리어했습니다!\n");

    return 0;
    }

