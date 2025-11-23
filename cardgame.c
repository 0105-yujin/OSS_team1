#define_CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define R 4 //행
#define C 5 //열
#define PAIRS 5 //짝의 개수
#define MAX_ATTEMPTS 10

char cards[R][C];
bool matched[R][C];
int attempts = MAX_ATTEMPTS;
int score = 0;

void clear_input_buffer(void)
{
  int ch;
  while((ch = getchar()) != '\n' && ch != EOF)
    {}
}

void init_cards() //카드 초기화 및 랜덤 배치
{
  int total = PAIRS * 2;
  char set[PAIRS * 2];
  for (int i = 0; i< PAIRS; i++)
  {
    set[i*2] = 'A'+i;
    set[i*2+1] = 'A'+i;
  }

  srand((unsigned)time(NULL));
  for (int i = total - 1; i > 0; i--)
    {
      int j = rand() % (i+1);
      char t = set[i];
      set[i] = set[j];
      set[j] = t;
    }

  for (int i = 0; i < R; i++)
    {
      for (int j = 0; j < C; j++)
        {
          cards[i][j] = set[i * C + j];
          matched[i][j] = false;
        }
    }
}

void draw_board() //게임판 출력
{
  system("cls");
  printf("\n---남은 기회 : %d | 맞춘 짝 : %d/%d ---\n", attempts, score, PAIRS);
  printf("   ");
  for (int j = 0; j < C; j++) printf("  %d ", j+1);
  printf("\n");
  for (int i = 0; i < R; i++)
    {
      printf("%d ",i+1);
      for (int j = 0; j < C; j++)
        {
          printf(" [%c]", matched[i][j] ? card[i][j] : '?');
        }
      printf("\n");
    }
  }
  
void get_selection(int* r, int* c) //좌표 입력 받기
{
  int row, col;
  while(1)
    {
      printf("선택 (행 열): ");
      if (scanf("%d %d", &row, &col) != 2)
      {
        clear_input_buffer();
        printf("유효하지 않은 입력입니다. 숫자 두 개를 입력하세요.\n");
        continue;
      }
      clear_input_buffer();
      row--; col--;
      if( row>= 0 && row < R && col >= 0 && col < C && !matched[row][col])
      {
        *r = row; *c = col;
        return;
      }
      else
      {
        printf("잘못된 위치이거나 이미 맞춘 카드 입니다.\n");
      }
    }
}

void wait_for_enter(void)
{
  printf(">> Enter 키를 누르세요.\n");
  int ch = getchar();
  if (ch != '\n') clear_input_buffer();
}

int main(void)
{
  init_cards();
  printf("카드 짝맞추기 게임 시작! 기회 %번. \n", MAX_ATTEMPTS);

  while(attempts > 0 && score < PAIRS)
    {
      draw_board();

      int r1, c1, r2, c2;

      get_selection(&r1, &c1);
      printf("첫 번째 선택된 카드 : %c\n",card[r1][c1]);

      get_selection(&r2,&c2);
      while(r1==r2 && c1== c2)
        {
          printf("같은 카드를 선택했습니다. 다시 선택하세요.\n");
          gets_selection(&r2,&c2);
        }
      printf("두 번째 선택된 카드 : %c\n", cards[r2][c2]);

      if(cards[r1][c1] == cards[r2][c2]);
      {
        printf("[성공] 짝을 맞췄습니다!\n");
        matched[r1][c1] = true;
        matched[r2][c2] = true;
        score++;
      }
      else
      {
        printf("[실패] 짝이 틀렸습니다.\n");
        attempts--;
      }

      wait_for_enter();
    }

  printf("\n==================================\n");
  if (score == PAIRS) 
  {
    printf("       ★ GAME CLEAR! ★\n");
  }
  else 
  {
    printf("       ★ GAME OVER ★\n");
  }
  printf("==================================\n");

  return 0;
}







