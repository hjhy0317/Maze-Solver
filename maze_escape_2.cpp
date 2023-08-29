#include <stdio.h>          
#include <stdlib.h>
#include "ListBaseStack.h"
#define num 21

typedef struct location
{
	int a;
	int b;
	int c;
} location;

location location_n;  // 현재 위치에 대한 구조체 'location_n' 정의

typedef struct pos
{
	int x;
	int y;
}pos;

pos loc[4] = { {1,0},{-1,0},{0,-1},{0,1} };  // 미로 찾는 방향(N,S,E,W)을 나타내는 'loc'

int maze[num][num] =  
{
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
	{1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1},
	{1,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,1},
	{1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1},
	{1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,1},
	{1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1},
	{1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,1},
	{1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1},
	{1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1},
	{1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1},
	{1,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,1},
	{1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1},
	{1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1},
	{1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1},
	{1,0,1,0,1,0,1,0,0,0,0,0,1,0,1,0,1,0,1,0,1},
	{1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1},
	{1,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1},
	{1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,2},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int* top = NULL;

void StackInit(Stack * pstack)
{
	pstack->head = NULL;  // 포인터 변수 head는 NULL로 initialize
}

int SIsEmpty(Stack* pstack)
{
	if (pstack->head == NULL)  // stack이 비면 head에는 NULL이 저장된다.
		return TRUE;
	else
		return FALSE;
}

void SPush(Stack* pstack, Data data)
{
	Node* newNode = (Node*)malloc(sizeof(Node));  // 새 노드 생성

	newNode->data = data;  // 새 노드에 데이터 저장
	newNode->next = pstack->head;  // 새 노드가 최근에 추가된 노드를 가리킴

	pstack->head = newNode;  // 포인터 변수 head가 새 노드를 가리킴
}

Data SPop(Stack* pstack)
{
	Data rdata;
	Node* rnode;

	if (SIsEmpty(pstack)) {
		printf("Stack Memory Error!");
		exit(-1);
	}

	rdata = pstack->head->data;  // 삭제할 노드의 데이터를 임시로 저장
	rnode = pstack->head;  // 삭제할 노드의 주소 값을 임시로 저장

	pstack->head = pstack->head->next;  // 삭제할 노드의 다음 노드를 head가 가리킴
	free(rnode);  // 노드 삭제
	return rdata;  // 삭제된 노드의 데이터 반환
}

Data SPeek(Stack* pstack)
{
	if (SIsEmpty(pstack)) {
		printf("Stack Memory Error!");
		exit(-1);
	}

	return pstack->head->data;  // head가 가리키는 노드에 저장된 데이터 반환
}

void showmaze(int(*maze)[num])  
{
	int i = 0, j = 0;

	for (i = 0; i < num; i++)
	{
		for (j = 0; j < num; j++)
		{
			if (maze[i][j] == 2)
				printf("OUT");  // maze에서 2는 'OUT'으로 출력(출구)
			else if (maze[i][j] == 3)
				printf("■");  // 3은 '■'으로 출력(경로)
			else if (maze[i][j] == 4)
				printf("IN");  // 4는 'IN'으로 출력(입구)
			else
				printf("%d"" ", maze[i][j]);
		}
		printf("\n");
	}
	printf("\n");

}

int main()
{
	int i, a, b, c, next_x, next_y, d, e;
	bool find = false;
	int initial_x, initial_y;  // 입구의 x,y좌표, 각각 열과 행에 해당
	int temp[num][num] = { 0 };  // 지나온 경로를 표시하는 'temp'배열

	Stack stack1, stack2, stack3;
	StackInit(&stack1);
	StackInit(&stack2);
	StackInit(&stack3);

	showmaze(maze);  
	printf("Starting point x-coordinate (column): ");  // 입구 x좌표 입력
	scanf_s("%d", &initial_x);
	printf("Starting point y-coordinate (row): ");  // 입구 y좌표 입력
	scanf_s("%d", &initial_y);
	if (maze[initial_y - 1][initial_x - 1] == 0)
	{
		SPush(&stack1, initial_y);
		SPush(&stack2, initial_x);
		SPush(&stack3, 0);  // stack에 저장
	}

	else
	{
		while (maze[initial_y - 1][initial_x - 1] == 1)
		{
			printf("Please re-enter. ex) x,y  ");
			scanf_s("%d , %d", &initial_x, &initial_y);
		}  // 입력받은 입구가 유효하지 않은 경우 다른 포인트 입력받음

		SPush(&stack1, initial_y);
		SPush(&stack2, initial_x);
		SPush(&stack3, 0);  // 입력받은 유효한 입구 정보를 stack에 저장
	}

	maze[initial_y - 1][initial_x - 1] = 4;

	while (*top!=NULL && find == 0)  // stack이 empty가 되고 출구를 찾을 때까지 반복
	{
		location_n = SPop(&stack1);
		location_n = SPop(&stack2);
		location_n = SPop(&stack3);
		a = location_n.a;
		b = location_n.b;
		c = location_n.c;  // 막다른 길인 경우 왔던 경로를 pop하여 갈 수 있는 곳이 생길 때까지 되돌아감

		while (c < 4 && find == 0)  // 출구를 찾을 때까지 동서남북 탐색 반복
		{
			next_x = a + loc[c].x;
			next_y = b + loc[c].y;

			if (maze[next_x][next_y] == 2)
			{
				location_n.a = a;
				location_n.b = b;
				location_n.c = ++c;
				SPush(&top, &location_n);
				find = true;
			}                       // 출구를 찾은 경우

			else if (maze[next_x][next_y] == 0 && temp[next_x][next_y] == 0)  // 갈 수 있는 경로가 있는 경우
			{
				temp[next_x][next_y] = 1;
				location_n.a = a;
				location_n.b = b;
				location_n.c = ++c;
				SPush(&stack1);
				SPush(&stack2);
				SPush(&stack3);
				a = next_x;
				b = next_y;
				c = 0;
			}                
			else
				++c;  // 갈 수 없는 경로인 경우
		}
	}

	printf("\n");

	if (find)  // 출구를 찾은 경우
	{
		for (d = 0; d < num; d++)
		{
			for (e = 0; e < num; e++)
			{
				for (i = 0; i <= top; i++)
				{
					if (stack1 == d && stack2 == e)
					{
						maze[d][e] = 3; 
					}
				}
			}
		}
		maze[initial_y - 1][initial_x - 1] = 4;
		printf("(%d,%d)", initial_x, initial_y);
		printf("\n");

		printf("Starting point : IN \nExit : OUT \n");


		showmaze(maze);
		printf("Maze Escape Success! \n\n");
		system("pause");
	}

	else  // 경로가 없는 경우
	{
		printf("(%d,%d)", initial_x, initial_y);
		printf("\n");
		printf("There is no path. \n\n");
		system("pause");
	}
}