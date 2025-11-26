#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <stdbool.h>

#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_YELLOW 6
#define COLOR_WHITE 7

#define R 2
#define C 5
#define PAIRS 5
#define MAX_ATTEMPTS 10

int FINAL_CODE[5] = { 7, 3, 9, 1, 5 };

void InitUI();
void Gotoxy(int x, int y);
void SetColor(int textColor, int bgColor);
void DrawLayout(char* title, char* subtitle);
void UpdateStatusBar(char* leftMsg, char* rightMsg);
void ShowPopup(char* title, char* message);
void PrintCenter(int y, char* text);
long GetTick() { return (long)clock(); }

void clear_input_buffer(void);
void init_cards(char cards[R][C], bool matched[R][C]);
void draw_board(char cards[R][C], bool matched[R][C], int attempts, int score);
void get_selection(char cards[R][C], bool matched[R][C], int* r, int* c);
void wait_for_enter(void);

int PlayCardGame();
int PlayRhythmGame();
int PlaySequenceGame();
int PlayMemoryGame();
int PlayBossGame();

int main() {

	system("cls");
	InitUI();
	srand((unsigned int)time(NULL));

	char msgBuf[100];

	while (1) {
		DrawLayout(" ESC_ TEAM ", "방탈출 프로젝트");
		PrintCenter(10, "1. 게임 시작");
		PrintCenter(12, "2. 종료");
		UpdateStatusBar("번호를 선택하세요", "팀 ESC");

		char choice = _getch();

		if (choice == '2') {
			break;
		}
		else if (choice == '1') {

			DrawLayout("프롤로그", "아무 키나 누르세요...");
			PrintCenter(10, "당신은 잠긴 방에서 깨어납니다...");
			PrintCenter(12, "5개의 단서를 찾아 탈출하세요.");
			_getch();

			// ------------------------------------------------------------------
			// 스테이지 1: 카드 게임 (3회 시도)
			int attempts1 = 3;
			int stage1_success = 0;
			while (attempts1 > 0) {
				if (PlayCardGame() == 1) {
					stage1_success = 1;
					break;
				}
				else {
					attempts1--;
					if (attempts1 > 0) {
						sprintf(msgBuf, "실패! 다시 시도합니다. (남은 기회: %d)", attempts1);
						ShowPopup("재시도", msgBuf);
					}
				}
			}
			if (!stage1_success) {
				ShowPopup("실패", "게임 오버 (1단계): 3회 모두 실패했습니다.");
				continue;
			}
			sprintf(msgBuf, "첫 번째 단서 획득: [ %d ]", FINAL_CODE[0]);
			ShowPopup("스테이지 클리어", msgBuf);


			// ------------------------------------------------------------------
			// 스테이지 2: 리듬 게임 (3회 시도)
			int attempts2 = 3;
			int stage2_success = 0;
			while (attempts2 > 0) {
				if (PlayRhythmGame() == 1) {
					stage2_success = 1;
					break;
				}
				else {
					attempts2--;
					if (attempts2 > 0) {
						sprintf(msgBuf, "실패! 다시 시도합니다. (남은 기회: %d)", attempts2);
						ShowPopup("재시도", msgBuf);
					}
				}
			}
			if (!stage2_success) {
				ShowPopup("실패", "게임 오버 (2단계): 3회 모두 실패했습니다.");
				continue;
			}
			sprintf(msgBuf, "두 번째 단서 획득: [ %d ]", FINAL_CODE[1]);
			ShowPopup("스테이지 클리어", msgBuf);

			// ------------------------------------------------------------------
			// 스테이지 3: 순서 게임 (3회 시도)
			int attempts3 = 3;
			int stage3_success = 0;
			while (attempts3 > 0) {
				if (PlaySequenceGame() == 1) {
					stage3_success = 1;
					break;
				}
				else {
					attempts3--;
					if (attempts3 > 0) {
						sprintf(msgBuf, "실패! 다시 시도합니다. (남은 기회: %d)", attempts3);
						ShowPopup("재시도", msgBuf);
					}
				}
			}
			if (!stage3_success) {
				ShowPopup("실패", "게임 오버 (3단계): 3회 모두 실패했습니다.");
				continue;
			}
			sprintf(msgBuf, "세 번째 단서 획득: [ %d ]", FINAL_CODE[2]);
			ShowPopup("스테이지 클리어", msgBuf);

			// ------------------------------------------------------------------
			// 스테이지 4: 기억력 게임 (3회 시도)
			int attempts4 = 3;
			int stage4_success = 0;
			while (attempts4 > 0) {
				if (PlayMemoryGame() == 1) {
					stage4_success = 1;
					break;
				}
				else {
					attempts4--;
					if (attempts4 > 0) {
						sprintf(msgBuf, "실패! 다시 시도합니다. (남은 기회: %d)", attempts4);
						ShowPopup("재시도", msgBuf);
					}
				}
			}
			if (!stage4_success) {
				ShowPopup("실패", "게임 오버 (4단계): 3회 모두 실패했습니다.");
				continue;
			}
			sprintf(msgBuf, "네 번째 단서 획득: [ %d ]", FINAL_CODE[3]);
			ShowPopup("스테이지 클리어", msgBuf);


			// ------------------------------------------------------------------
			// 스테이지 5: 보스 게임 (3회 시도)
			int attempts5 = 3;
			int stage5_success = 0;
			while (attempts5 > 0) {
				if (PlayBossGame() == 1) {
					stage5_success = 1;
					break;
				}
				else {
					attempts5--;
					if (attempts5 > 0) {
						sprintf(msgBuf, "실패! 다시 시도합니다. (남은 기회: %d)", attempts5);
						ShowPopup("재시도", msgBuf);
					}
				}
			}
			if (!stage5_success) {
				ShowPopup("실패", "게임 오버 (5단계): 3회 모두 실패했습니다.");
				continue;
			}
			sprintf(msgBuf, "마지막 단서 획득: [ %d ]", FINAL_CODE[4]);
			ShowPopup("스테이지 클리어", msgBuf);


			DrawLayout("최종 관문", "도어락 비밀번호를 입력하세요");
			PrintCenter(8, "획득한 단서를 순서대로 입력하세요.");
			UpdateStatusBar("5자리 숫자 입력", "엔터 키로 입력");

			Gotoxy(35, 12);
			printf("비밀번호: ");

			int inputCode;
			if (scanf_s("%d", &inputCode) == 1) {
				int correctCode = FINAL_CODE[0] * 10000 +
					FINAL_CODE[1] * 1000 +
					FINAL_CODE[2] * 100 +
					FINAL_CODE[3] * 10 +
					FINAL_CODE[4];

				if (inputCode == correctCode) {
					ShowPopup("잠금 해제", "철컥! 문이 열립니다...");

					DrawLayout("탈출 성공!", "축하합니다!");
					SetColor(COLOR_GREEN, COLOR_BLACK);
					PrintCenter(10, "#################################");
					PrintCenter(11, "#       MISSION COMPLETE        #");
					PrintCenter(12, "#################################");
					SetColor(COLOR_WHITE, COLOR_BLACK);
					PrintCenter(15, "팀원: 홍유진, 김명동, 김장혁, 이동호, 팽선우");
					PrintCenter(17, "플레이해주셔서 감사합니다.");
					_getch();
				}
				else {
					ShowPopup("경고", "비밀번호가 틀렸습니다! 처음으로 돌아갑니다.");
				}
			}

			while (getchar() != '\n');
		}
	}

	SetColor(COLOR_WHITE, COLOR_BLACK);
	system("cls");
	return 0;
}

void clear_input_buffer(void) {
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF) {}
}

void init_cards(char cards[R][C], bool matched[R][C]) {
	int total = PAIRS * 2;
	char set[PAIRS * 2];
	for (int i = 0; i < PAIRS; i++) {
		set[i * 2] = 'A' + i;
		set[i * 2 + 1] = 'A' + i;
	}
	for (int i = total - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		char t = set[i]; set[i] = set[j]; set[j] = t;
	}
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			cards[i][j] = set[i * C + j];
			matched[i][j] = false;
		}
	}
}

void draw_board(char cards[R][C], bool matched[R][C], int attempts, int score) {
	system("cls");
	printf("\n---남은 기회 : %d | 맞춘 짝 : %d/%d ---\n", attempts, score, PAIRS);
	printf("    ");
	for (int j = 0; j < C; j++) printf(" %d    ", j + 1);
	printf("\n");
	for (int i = 0; i < R; i++) {
		printf("%d", i + 1);
		printf("┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐\n");
		for (int j = 0; j < C; j++) {
			printf(" │ %c │", matched[i][j] ? cards[i][j] : '?');
		}
		printf("\n");
		printf(" └───┘ └───┘ └───┘ └───┘ └───┘\n");
	}
}

void get_selection(char cards[R][C], bool matched[R][C], int* r, int* c) {
	int row, col;
	while (1) {
		printf("선택 (행 열): ");
		if (scanf("%d %d", &row, &col) != 2) {
			clear_input_buffer();
			printf("유효하지 않은 입력입니다.\n");
			continue;
		}
		clear_input_buffer();
		row--; col--;
		if (row >= 0 && row < R && col >= 0 && col < C && !matched[row][col]) {
			*r = row; *c = col;
			return;
		}
		else {
			printf("잘못된 위치이거나 이미 맞춘 카드 입니다.\n");
		}
	}
}

void wait_for_enter(void) {
	printf(">> Enter 키를 누르세요.\n");
	int ch = getchar();
	if (ch != '\n') clear_input_buffer();
}

void InitUI() {
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	SetConsoleTitle("ESC: 방탈출 게임");
	system("mode con: cols=80 lines=25");
}

void Gotoxy(int x, int y) {
	COORD pos = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void SetColor(int textColor, int bgColor) {
	int color = textColor + (bgColor * 16);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void DrawLayout(char* title, char* subtitle) {
	system("cls");
	SetColor(COLOR_GREEN, COLOR_BLACK);
	for (int x = 0; x < 80; x++) { Gotoxy(x, 0); printf("="); Gotoxy(x, 24); printf("="); }
	for (int y = 0; y < 25; y++) { Gotoxy(0, y); printf("|"); Gotoxy(79, y); printf("|"); }
	SetColor(COLOR_WHITE, COLOR_BLACK);
	Gotoxy(2, 1); printf("[ ESC 시스템 ]");
	PrintCenter(2, title);
	if (subtitle != NULL) { SetColor(COLOR_YELLOW, COLOR_BLACK); PrintCenter(3, subtitle); }
	SetColor(COLOR_GREEN, COLOR_BLACK);
	Gotoxy(1, 4); for (int i = 1; i < 79; i++) printf("-");
	SetColor(COLOR_WHITE, COLOR_BLACK);
}

void UpdateStatusBar(char* leftMsg, char* rightMsg) {
	SetColor(COLOR_WHITE, COLOR_BLACK);
	Gotoxy(2, 23); printf("                                                                              ");
	if (leftMsg != NULL) { Gotoxy(2, 23); printf("%s", leftMsg); }
	if (rightMsg != NULL) {
		int len = 0; while (rightMsg[len] != '\0') len++;
		Gotoxy(78 - len, 23); printf("%s", rightMsg);
	}
}

void ShowPopup(char* title, char* message) {
	int w = 40, h = 10, sx = (80 - w) / 2, sy = (25 - h) / 2;
	SetColor(COLOR_WHITE, COLOR_BLUE);
	for (int y = sy; y < sy + h; y++) { Gotoxy(sx, y); for (int x = 0; x < w; x++) printf(" "); }
	for (int x = sx; x < sx + w; x++) { Gotoxy(x, sy); printf("-"); Gotoxy(x, sy + h - 1); printf("-"); }
	int tLen = 0; while (title[tLen]) tLen++; Gotoxy(sx + (w - tLen) / 2, sy + 1); printf("[%s]", title);
	int mLen = 0; while (message[mLen]) mLen++; Gotoxy(sx + (w - mLen) / 2, sy + 4); printf("%s", message);
	Gotoxy(sx + (w - 18) / 2, sy + 8); printf("아무 키나 누르세요...");
	_getch();
	SetColor(COLOR_WHITE, COLOR_BLACK);
	system("cls");
}

void PrintCenter(int y, char* text) {
	int len = 0; while (text[len] != '\0') len++;
	int x = (80 - len) / 2;
	Gotoxy(x, y); printf("%s", text);
}

int PlayCardGame() {
	DrawLayout("스테이지 1: 카드 짝 맞추기", "같은 숫자의 카드를 3쌍 찾으세요.");

	int cards[10] = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
	int revealed[10] = { 0 }; // 1: 성공, 0: 숨김
	int matches = 0;
	// 이 시도 횟수는 스테이지 자체의 기회 횟수입니다.
	// main의 3회 시도와 별개로, 게임 내부의 기회 횟수는 10회로 유지합니다.
	int tries = 10;
	char buf[50];

	// 카드 섞기
	for (int i = 0; i < 30; i++) {
		int a = rand() % 10;
		int b = rand() % 10;
		int t = cards[a]; cards[a] = cards[b]; cards[b] = t;
	}

	while (tries > 0 && matches < 3) {
		// 상태바 업데이트
		sprintf(buf, "남은 기회: %d | 찾은 쌍: %d/3", tries, matches);
		UpdateStatusBar(buf, "번호 2개를 입력하세요 (1-9,0)");

		// 카드 출력
		for (int idx = 0; idx < 10; idx++) {
			int row = idx / 5;
			int col = idx % 5;
			int x = 18 + col * 8;
			int y = 7 + row * 6;

			SetColor(revealed[idx] ? COLOR_CYAN : COLOR_WHITE, COLOR_BLACK);
			Gotoxy(x, y); printf("┌───┐");
			Gotoxy(x, y + 1); printf("│ %c │", revealed[idx] ? '0' + cards[idx] : '?');
			Gotoxy(x, y + 2); printf("└───┘");

			// 카드 밑 번호: 1~9,0 순서
			Gotoxy(x + 2, y + 3);
			printf("%d", (idx + 1) % 10);
		}

		// 첫 번째 카드 선택
		SetColor(COLOR_WHITE, COLOR_BLACK);
		Gotoxy(25, 18); printf("첫 번째 카드 (1-9,0): ");
		char ch1 = _getch();
		int first = (ch1 == '0') ? 9 : ch1 - '1';
		if (first < 0 || first > 9 || revealed[first]) continue;
		revealed[first] = 1;

		// 첫 번째 카드 바로 보여주기
		int fx = 18 + (first % 5) * 8;
		int fy = 7 + (first / 5) * 6;
		Gotoxy(fx, fy + 1); SetColor(COLOR_WHITE, COLOR_BLACK); printf("│ %d │", cards[first]);
		Gotoxy(25 + 21, 18); printf("%c", ch1); // 입력값 표시

		// 두 번째 카드 선택
		Gotoxy(25, 19); printf("두 번째 카드 (1-9,0): ");
		char ch2 = _getch();
		int second = (ch2 == '0') ? 9 : ch2 - '1';
		if (second < 0 || second > 9 || first == second || revealed[second]) {
			revealed[first] = 0; // 잘못 선택 시 첫 번째 카드 숨김
			// 잘못된 입력이나 이미 맞춘 카드를 선택했을 때는 시도 횟수를 줄이지 않음

			// 입력 자리 초기화
			Gotoxy(25, 18); printf("                                 ");
			Gotoxy(25, 19); printf("                                 ");
			continue;
		}
		revealed[second] = 1;

		// 두 번째 카드 바로 보여주기
		int sx = 18 + (second % 5) * 8;
		int sy = 7 + (second / 5) * 6;
		Gotoxy(sx, sy + 1); SetColor(COLOR_WHITE, COLOR_BLACK); printf("│ %d │", cards[second]);
		Gotoxy(fx, fy + 1); printf("│ %d │", cards[first]);
		Gotoxy(25 + 21, 19); printf("%c", ch2); // 입력값 표시

		Sleep(700); // 잠시 공개

		// 결과 확인
		if (cards[first] == cards[second]) {
			matches++;
			// 성공 시에는 revealed 상태 유지 (팝업에서 'CYAN' 색깔로 보임)
			// DrawLayout 호출 후 화면 복구가 필요
			ShowPopup("성공!", "짝을 찾았습니다.");

			// 팝업 후 화면 복구
			DrawLayout("스테이지 1: 카드 짝 맞추기", "같은 숫자의 카드를 3쌍 찾으세요.");
			sprintf(buf, "남은 기회: %d | 찾은 쌍: %d/3", tries, matches);
			UpdateStatusBar(buf, "번호 2개를 입력하세요 (1-9,0)");
		}
		else {
			revealed[first] = revealed[second] = 0;
			tries--;
		}

		// 입력 자리 초기화
		Gotoxy(25, 18); printf("                                 ");
		Gotoxy(25, 19); printf("                                 ");
	}

	return (matches >= 3) ? 1 : 0;
}


#define R_LANE_START_X 10
#define R_LANE_WIDTH 6
#define R_JUDGE_LINE_Y 20
#define R_SPEED_DIVIDER 100

typedef struct {
	long targetTime;
	int line;
	int judged;
	int prevY;
} R_Note;

int PlayRhythmGame() {
	system("cls");
	R_Note notes[] = {
		{2000, 0, 0, -1}, {3000, 1, 0, -1}, {4000, 2, 0, -1}, {5000, 3, 0, -1},
		{6000, 0, 0, -1}, {6500, 1, 0, -1}, {7000, 0, 0, -1}, {7500, 3, 0, -1}
	};
	int noteCount = sizeof(notes) / sizeof(R_Note);

	DrawLayout("스테이지 2: 리듬 게임", "타이밍에 맞춰 키를 누르세요!");
	SetColor(COLOR_CYAN, COLOR_BLACK);
	PrintCenter(4, "키: [D] [F] [J] [K]");

	for (int y = 5; y <= 22; y++) {
		for (int i = 0; i < 4; i++) {
			Gotoxy(R_LANE_START_X + (i * R_LANE_WIDTH), y); printf("|");
			Gotoxy(R_LANE_START_X + (i * R_LANE_WIDTH) + R_LANE_WIDTH, y); printf("|");
		}
	}
	SetColor(COLOR_YELLOW, COLOR_BLACK);
	Gotoxy(R_LANE_START_X, R_JUDGE_LINE_Y); printf("-------------------------");
	SetColor(COLOR_WHITE, COLOR_BLACK);
	Gotoxy(R_LANE_START_X + 2, R_JUDGE_LINE_Y + 1); printf("D");
	Gotoxy(R_LANE_START_X + 2 + R_LANE_WIDTH, R_JUDGE_LINE_Y + 1); printf("F");
	Gotoxy(R_LANE_START_X + 2 + R_LANE_WIDTH * 2, R_JUDGE_LINE_Y + 1); printf("J");
	Gotoxy(R_LANE_START_X + 2 + R_LANE_WIDTH * 3, R_JUDGE_LINE_Y + 1); printf("K");

	long startTime = GetTick();
	int score = 0, combo = 0, maxCombo = 0;
	int misses = 0; // 놓친 노트 카운트
	int gameRunning = 1;
	char msg[50] = "준비";

	while (gameRunning) {
		long currentTime = GetTick() - startTime;
		int allFinished = 1;

		if (_kbhit()) {
			int key = _getch();
			int line = -1;
			if (key == 'd' || key == 'D') line = 0; else if (key == 'f' || key == 'F') line = 1;
			else if (key == 'j' || key == 'J') line = 2; else if (key == 'k' || key == 'K') line = 3;
			else if (key == 'q' || key == 'Q') { gameRunning = 0; break; }

			if (line != -1) {
				int hitIdx = -1; long minDiff = 9999;
				for (int i = 0; i < noteCount; i++) {
					if (notes[i].line == line && !notes[i].judged) {
						// 판정 범위: 목표 시간 기준 400ms 전후
						long diff = abs(currentTime - notes[i].targetTime);
						if (diff < 400) {
							if (diff < minDiff) { minDiff = diff; hitIdx = i; }
						}
					}
				}
				if (hitIdx != -1) {
					notes[hitIdx].judged = 1;
					// 노트 지우기
					if (notes[hitIdx].prevY > 0 && notes[hitIdx].prevY < 24) {
						Gotoxy(R_LANE_START_X + (notes[hitIdx].line * R_LANE_WIDTH) + 2, notes[hitIdx].prevY); printf("  ");
					}
					// 점수 및 콤보 처리
					if (minDiff < 100) { score += 100; combo++; strcpy(msg, "완벽해요!"); }
					else if (minDiff < 250) { score += 50; combo++; strcpy(msg, "좋아요  "); }
					else { combo = 0; misses++; strcpy(msg, "나빠요  "); } // Misses 카운트 추가
					if (combo > maxCombo) maxCombo = combo;
				}
				else {
					// 헛손질 (노트가 없는 곳을 쳤을 경우)
					combo = 0;
					strcpy(msg, "MISS     ");
				}
			}
		}

		for (int i = 0; i < noteCount; i++) {
			if (notes[i].judged) continue;
			allFinished = 0;
			long diff = notes[i].targetTime - currentTime;

			// 놓침 판정 (Perfect Timing 기준 300ms 이후)
			if (diff < -300) {
				notes[i].judged = 1;
				combo = 0;
				misses++; // 놓친 노트 카운트
				strcpy(msg, "놓침    ");

				// 노트 지우기
				if (notes[i].prevY >= 5) {
					Gotoxy(R_LANE_START_X + (notes[i].line * R_LANE_WIDTH) + 2, notes[i].prevY); printf("  ");
				}
				continue;
			}

			// 노트 그리기 위치 계산
			int y = R_JUDGE_LINE_Y - (int)(diff / R_SPEED_DIVIDER);
			if (y != notes[i].prevY) {
				// 이전 위치 지우기
				if (notes[i].prevY >= 5 && notes[i].prevY < 24 && notes[i].prevY != R_JUDGE_LINE_Y) {
					Gotoxy(R_LANE_START_X + (notes[i].line * R_LANE_WIDTH) + 2, notes[i].prevY); printf("  ");
				}
				// 새 위치 그리기
				if (y >= 5 && y < 24) {
					Gotoxy(R_LANE_START_X + (notes[i].line * R_LANE_WIDTH) + 2, y);
					SetColor(COLOR_CYAN, COLOR_BLACK); printf("●");
				}
				notes[i].prevY = y;
			}
		}

		SetColor(COLOR_WHITE, COLOR_BLACK);
		Gotoxy(50, 10); printf("점수 : %d    ", score);
		Gotoxy(50, 12); printf("콤보 : %d    ", combo);
		Gotoxy(50, 14); printf("%s        ", msg);

		// 모든 노트 처리가 끝나면 게임 종료
		if (allFinished) {
			Sleep(1000);
			break;
		}
		Sleep(30);
	}

	// 전체 노트 중 50% 이상 (5개 이상) 성공해야 클리어
	// 8개 중 5개 이상 성공 (score 50 * 5 = 250 이상)
	return (score >= 300) ? 1 : 0; // 원본 코드의 조건 유지
}

int PlaySequenceGame() {
	int base_seq[5] = { 1, 4, 3, 5, 2 };
	int map[6]; for (int i = 1; i <= 5; i++) map[i] = i;
	for (int i = 5; i >= 2; i--) { int j = rand() % i + 1; int t = map[i]; map[i] = map[j]; map[j] = t; }
	int answer[5]; for (int i = 0; i < 5; i++) answer[i] = map[base_seq[i]];
	int order[5] = { 0, 1, 2, 3, 4 };
	for (int i = 4; i > 0; i--) { int j = rand() % (i + 1); int t = order[i]; order[i] = order[j]; order[j] = t; }

	DrawLayout("스테이지 3: 논리 퍼즐", "힌트를 분석하여 순서를 찾으세요.");
	UpdateStatusBar("입력: 1 2 3 4 5", "논리에 집중하세요");
	int startY = 7; Gotoxy(5, startY - 2); printf("[ 힌트 ]");

	for (int k = 0; k < 5; k++) {
		Gotoxy(5, startY + (k * 2)); printf("%d. ", k + 1);
		switch (order[k]) {
		case 0: printf("숫자 %d은(는) 첫 번째 위치에 있습니다.", map[1]); break;
		case 1: printf("숫자 %d은(는) %d 바로 앞에 있습니다.", map[4], map[3]); break;
		case 2: printf("숫자 %d은(는) %d 앞에 옵니다.", map[3], map[5]); break;
		case 3: printf("숫자 %d은(는) %d 바로 앞에 있습니다.", map[5], map[2]); break;
		case 4: printf("숫자 %d은(는) %d 앞에 옵니다.", map[1], map[5]); break;
		}
	}

	int input[5];
	// 스테이지 내부의 시도 횟수는 3회로 유지합니다. 
	// main의 3회 시도와 별개로, 이 3회 시도 안에 성공해야 합니다.
	int tries = 3;
	while (tries > 0) {
		char msg[30]; sprintf(msg, "남은 시도: %d", tries); UpdateStatusBar(msg, "형식: 1 2 3 4 5");
		Gotoxy(15, 20); printf("코드 입력 (예: 4 2 3 1 5):                                         "); Gotoxy(44, 20);
		if (scanf_s("%d %d %d %d %d", &input[0], &input[1], &input[2], &input[3], &input[4]) != 5) {
			while (getchar() != '\n');
			ShowPopup("오류", "잘못된 형식입니다!");
			continue;
		}
		while (getchar() != '\n');
		int correct = 1; for (int i = 0; i < 5; i++) if (input[i] != answer[i]) { correct = 0; break; }
		if (correct) return 1;
		else {
			tries--;
			// 마지막 시도 후에는 팝업을 띄우지 않음 (외부 main 루프가 처리)
			if (tries > 0) {
				ShowPopup("틀림", "잘못된 순서입니다. 다시 시도하세요.");
			}
		}
	}
	return 0;
}

int PlayMemoryGame() {
	int round_lengths[] = { 3, 5, 7 };
	int total_rounds = 3;
	int sequence[20];
	char input[100];

	for (int round = 0; round < total_rounds; round++) {
		int length = round_lengths[round];
		for (int i = 0; i < length; i++) sequence[i] = rand() % 10;

		char title[50]; sprintf(title, "스테이지 4: 기억력 (라운드 %d)", round + 1);

		for (int i = 0; i < length; i++) {
			DrawLayout(title, "숫자를 기억하세요 (2초 간격으로 나타납니다)");
			char numStr[20]; sprintf(numStr, "[ %d ]", sequence[i]);

			SetColor(COLOR_CYAN, COLOR_BLACK);
			PrintCenter(12, numStr);

			Sleep(2000);

			DrawLayout(title, "...");
			Sleep(200);
		}
		SetColor(COLOR_WHITE, COLOR_BLACK);

		DrawLayout(title, "기억한 숫자를 순서대로 입력하세요.");
		UpdateStatusBar("입력 완료 후 엔터", "띄어쓰기 없이 입력 (예: 123)");

		Gotoxy(25, 12); printf("정답 입력: ");

		if (scanf_s("%s", input, 100) != 1) {
			while (getchar() != '\n');
			// 입력 오류 시 즉시 실패 (외부 루프에서 재시도)
			return 0;
		}
		while (getchar() != '\n');

		int correct = 1;
		if (strlen(input) != length) correct = 0;
		else {
			for (int i = 0; i < length; i++) {
				if (input[i] != sequence[i] + '0') {
					correct = 0; break;
				}
			}
		}

		if (correct) {
			// 마지막 라운드가 아니면 다음 라운드 안내
			if (round < total_rounds - 1) {
				ShowPopup("정답!", "다음 라운드로 진행합니다.");
			}

		}
		else {
			char answerStr[50] = "정답: ";
			for (int i = 0; i < length; i++) {
				char temp[2]; sprintf(temp, "%d", sequence[i]);
				strcat(answerStr, temp);
			}
			ShowPopup("실패", answerStr);
			// 라운드 실패 시 전체 스테이지 실패로 처리 (외부 루프에서 재시도)
			return 0;
		}
	}
	return 1; // 모든 라운드 클리어
}

#define B_WIDTH 80
#define B_HEIGHT 25
#define B_WALL_CHAR "#"
#define B_PLAYER_CHAR "o"
#define B_ENEMY_CHAR "X"
#define B_ITEM_CHAR "$"

typedef struct { int x, y; } BWall;
typedef struct { int x, y; int active; } BItem;

void b_set_cursor(int x, int y) { Gotoxy(x, y); }
void b_set_color(int color) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); }

int b_is_wall(int x, int y, BWall walls[], int count) {
	for (int i = 0; i < count; i++) if (x == walls[i].x && y == walls[i].y) return 1;
	return 0;
}

void b_draw_walls(BWall walls[], int count) {
	b_set_color(8); // 회색 벽
	for (int i = 0; i < count; i++) {
		b_set_cursor(walls[i].x, walls[i].y);
		printf("%s", B_WALL_CHAR);
	}
	b_set_color(COLOR_WHITE);
}

void b_draw_items(BItem items[], int count) {
	b_set_color(14); // 노란색 아이템
	for (int i = 0; i < count; i++) {
		if (items[i].active) {
			b_set_cursor(items[i].x, items[i].y);
			printf("%s", B_ITEM_CHAR);
		}
	}
	b_set_color(COLOR_WHITE);
}

void b_reset_items(BItem items[], int count, BWall walls[], int wallCount, int px, int py, int ex, int ey) {
	for (int i = 0; i < count; i++) {
		int x, y, valid;
		do {
			valid = 1;
			// 벽/플레이어/적과 겹치지 않는 위치를 찾음
			x = rand() % 78 + 1; // 1 ~ 78
			y = rand() % 23 + 1; // 1 ~ 23
			for (int w = 0; w < wallCount; w++) {
				if (walls[w].x == x && walls[w].y == y) { valid = 0; break; }
			}
			if ((x == px && y == py) || (x == ex && y == ey)) valid = 0;
			if (y < 5) valid = 0; // 레이아웃 상단바와 겹치지 않도록
		} while (!valid);
		items[i].x = x; items[i].y = y; items[i].active = 1;
	}
}

int PlayBossGame() {
	int px, py, ex, ey;
	int ch, round = 1;
	DWORD lastMoveTime;
	int baseSpeed = 300;
	int score = 0;
	const int rounds_to_win = 2;

	BWall walls[] = {
		{10,6},{15,8},{20,11},{35,9},{40,13},{45,18},{60,10},{62,11},{64,12},
		{25,21},{30,23},{50,23},{12,15},{18,17},{22,19},{70,8},{72,12},{74,15}
	};
	int wallCount = sizeof(walls) / sizeof(walls[0]);

	BItem items[3];

	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO ci;
	GetConsoleCursorInfo(out, &ci);
	ci.bVisible = FALSE;
	SetConsoleCursorInfo(out, &ci);

restart_round:
	system("cls");
	DrawLayout("스테이지 5: 보스 추격전", "적에게 잡히지 않고 아이템 3개를 획득하세요!");
	b_set_color(COLOR_WHITE);
	Gotoxy(2, 4); for (int i = 2; i < 78; i++) printf("-"); // 레이아웃 선 재정의

	px = 5; py = 5;
	ex = 70; ey = 20;
	score = 0;

	// 아이템 위치 재설정 시 플레이어/적 위치 전달
	b_reset_items(items, 3, walls, wallCount, px, py, ex, ey);
	b_draw_walls(walls, wallCount);
	b_draw_items(items, 3);

	lastMoveTime = GetTickCount();
	b_set_cursor(px, py); b_set_color(COLOR_GREEN); printf("%s", B_PLAYER_CHAR);
	b_set_cursor(ex, ey); b_set_color(COLOR_RED); printf("%s", B_ENEMY_CHAR);

	b_set_cursor(0, B_HEIGHT - 1);
	b_set_color(COLOR_YELLOW);
	printf("Round %d 시작! 보스 속도: %.2fx (아이템: 0/3) [WASD 또는 방향키로 이동, Q:포기] ", round, (1000.0 / baseSpeed));
	b_set_color(COLOR_WHITE);

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

			// 경계 검사 (레이아웃 경계를 침범하지 않도록)
			if (nx < 1 || ny < 5 || nx >= 79 || ny >= 24) continue;
			if (b_is_wall(nx, ny, walls, wallCount)) continue;

			// 플레이어 이동
			b_set_cursor(px, py); printf(" ");
			px = nx; py = ny;
			b_set_cursor(px, py); b_set_color(COLOR_GREEN); printf("%s", B_PLAYER_CHAR);
			b_set_color(COLOR_WHITE);
		}

		// 아이템 획득 검사
		for (int i = 0; i < 3; i++) {
			if (items[i].active && px == items[i].x && py == items[i].y) {
				items[i].active = 0;
				b_set_cursor(items[i].x, items[i].y); printf(" ");
				score++;
				b_set_cursor(0, B_HEIGHT - 1);
				b_set_color(COLOR_YELLOW);
				printf("Round %d 진행 중 (아이템: %d / 3) 보스 속도: %.2fx (Q:포기)               ", round, score, (1000.0 / baseSpeed));
				b_set_color(COLOR_WHITE);
			}
		}

		// 적(보스) 이동 로직
		if (GetTickCount() - lastMoveTime > baseSpeed) {
			int dx = 0, dy = 0;
			// 플레이어를 향해 이동
			if (ex < px) dx = 1; else if (ex > px) dx = -1;
			if (ey < py) dy = 1; else if (ey > py) dy = -1;

			int nex = ex + dx;
			int ney = ey + dy;

			// 아이템 위로는 이동하지 않음
			int onItem = 0;
			for (int i = 0; i < 3; i++) if (items[i].active && items[i].x == nex && items[i].y == ney) onItem = 1;

			// 벽이나 아이템 위가 아니면 이동
			if (!b_is_wall(nex, ney, walls, wallCount) && !onItem) {
				b_set_cursor(ex, ey); printf(" ");
				ex = nex; ey = ney;
				b_set_cursor(ex, ey); b_set_color(COLOR_RED); printf("%s", B_ENEMY_CHAR);
				b_set_color(COLOR_WHITE);
			}
			else {
				// 벽에 막혔을 경우, 랜덤하게 방향을 바꿔 시도 (간단한 회피 로직)
				int attempts = 0;
				while (b_is_wall(nex, ney, walls, wallCount) || onItem) {
					dx = (rand() % 3) - 1;
					dy = (rand() % 3) - 1;
					if (dx == 0 && dy == 0) { dx = 1; } // 제자리에 있지 않도록 강제
					nex = ex + dx;
					ney = ey + dy;
					onItem = 0;
					for (int i = 0; i < 3; i++) if (items[i].active && items[i].x == nex && items[i].y == ney) onItem = 1;
					attempts++;
					if (attempts > 5) break; // 무한 루프 방지
				}

				if (!b_is_wall(nex, ney, walls, wallCount) && !onItem && nex > 0 && ney > 4 && nex < 79 && ney < 24) {
					b_set_cursor(ex, ey); printf(" ");
					ex = nex; ey = ney;
					b_set_cursor(ex, ey); b_set_color(COLOR_RED); printf("%s", B_ENEMY_CHAR);
					b_set_color(COLOR_WHITE);
				}
			}

			lastMoveTime = GetTickCount();
		}

		// 추격자에게 잡혔는지 확인 (접촉)
		if (abs(ex - px) <= 1 && abs(ey - py) <= 1) { // 1칸 이내 근접
			b_set_cursor(0, B_HEIGHT - 1);
			b_set_color(COLOR_RED);
			printf("\n[##] 추격자에게 잡혔습니다! 실패... [##]                        \n");
			Sleep(1200);
			ci.bVisible = TRUE;
			SetConsoleCursorInfo(out, &ci);
			return 0;
		}

		// 아이템 모두 획득 시 다음 라운드 또는 클리어
		if (score == 3) {
			round++;
			if (round > rounds_to_win) {
				b_set_cursor(0, B_HEIGHT - 1);
				b_set_color(COLOR_GREEN);
				printf("\n[**] 보스전 최종 클리어! 축하합니다. [**]                       \n");
				Sleep(1000);
				ci.bVisible = TRUE;
				SetConsoleCursorInfo(out, &ci);
				return 1;
			}
			// 다음 라운드, 속도 증가 (줄어듦)
			baseSpeed = (int)(baseSpeed * 0.7);
			b_set_cursor(0, B_HEIGHT - 1);
			b_set_color(COLOR_YELLOW);
			printf("\n[!!] 아이템 모두 획득! 다음 라운드 시작... (속도 증가) [!!]     \n");
			Sleep(1200);
			goto restart_round;
		}

		Sleep(10);
	}

	ci.bVisible = TRUE;
	SetConsoleCursorInfo(out, &ci);
	return 0;
}
