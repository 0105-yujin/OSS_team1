#include <stdio.h>
#include <string.h>


int times(int a, int b);

int main(void)
{
  int num1, num2, result = 0;
  char = op;

  printf("사칙연산 계산기\n");
  printf("입력 예시 : 3 + 4 \n");

  while(1)
    {
      printf("입력(종료는 q입력) : ");

      char line[100];
      if(fgets(line, sizeof(line), stdin) == NULL)
      {
        puts("입력 종료");
        return 0;
      }

      line[strcspn(line, "\r\n")] == '\0';

      if(line[0] == 'q' || line[0] == 'Q')
      {
        return 0;
      }

      int parsed = sscanf(line, " %d %c %d", &num1, &op, &num2);
      if(parsed != 3)
      {
        printf("입력 형식 오류\n\n");
        continue;
      }

      switch(op)
        {
          case '+':
            result = plus(num1,num2);
          case '-':
            result = minus(num1,num2);
          case '*':
            result = times(num1,num2);
          case '/':
            if (num2 == 0)
            {
              printf("지원하지 않는 입력 값\n\n");
              continue;
            }
            result = divide(num1,num2);
            break;
          default:
            printf("지원하지 않는 연산자\n",op);
            printf("\n");
            continue;
        }
      printf("결과 : %d %c %d = %d\n\n", num1, op, num2, result);
    }
  return 0;
}

int times(int a,int b){
 return a*b;
}
