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

location location_n;  // ���� ��ġ�� ���� ����ü 'location_n' ����

typedef struct pos
{
	int x;
	int y;
}pos;

pos loc[4] = { {1,0},{-1,0},{0,-1},{0,1} };  // �̷� ã�� ����(N,S,E,W)�� ��Ÿ���� 'loc'

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
	pstack->head = NULL;  // ������ ���� head�� NULL�� initialize
}

int SIsEmpty(Stack* pstack)
{
	if (pstack->head == NULL)  // stack�� ��� head���� NULL�� ����ȴ�.
		return TRUE;
	else
		return FALSE;
}

void SPush(Stack* pstack, Data data)
{
	Node* newNode = (Node*)malloc(sizeof(Node));  // �� ��� ����

	newNode->data = data;  // �� ��忡 ������ ����
	newNode->next = pstack->head;  // �� ��尡 �ֱٿ� �߰��� ��带 ����Ŵ

	pstack->head = newNode;  // ������ ���� head�� �� ��带 ����Ŵ
}

Data SPop(Stack* pstack)
{
	Data rdata;
	Node* rnode;

	if (SIsEmpty(pstack)) {
		printf("Stack Memory Error!");
		exit(-1);
	}

	rdata = pstack->head->data;  // ������ ����� �����͸� �ӽ÷� ����
	rnode = pstack->head;  // ������ ����� �ּ� ���� �ӽ÷� ����

	pstack->head = pstack->head->next;  // ������ ����� ���� ��带 head�� ����Ŵ
	free(rnode);  // ��� ����
	return rdata;  // ������ ����� ������ ��ȯ
}

Data SPeek(Stack* pstack)
{
	if (SIsEmpty(pstack)) {
		printf("Stack Memory Error!");
		exit(-1);
	}

	return pstack->head->data;  // head�� ����Ű�� ��忡 ����� ������ ��ȯ
}

void showmaze(int(*maze)[num])  
{
	int i = 0, j = 0;

	for (i = 0; i < num; i++)
	{
		for (j = 0; j < num; j++)
		{
			if (maze[i][j] == 2)
				printf("OUT");  // maze���� 2�� 'OUT'���� ���(�ⱸ)
			else if (maze[i][j] == 3)
				printf("��");  // 3�� '��'���� ���(���)
			else if (maze[i][j] == 4)
				printf("IN");  // 4�� 'IN'���� ���(�Ա�)
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
	int initial_x, initial_y;  // �Ա��� x,y��ǥ, ���� ���� �࿡ �ش�
	int temp[num][num] = { 0 };  // ������ ��θ� ǥ���ϴ� 'temp'�迭

	Stack stack1, stack2, stack3;
	StackInit(&stack1);
	StackInit(&stack2);
	StackInit(&stack3);

	showmaze(maze);  
	printf("Starting point x-coordinate (column): ");  // �Ա� x��ǥ �Է�
	scanf_s("%d", &initial_x);
	printf("Starting point y-coordinate (row): ");  // �Ա� y��ǥ �Է�
	scanf_s("%d", &initial_y);
	if (maze[initial_y - 1][initial_x - 1] == 0)
	{
		SPush(&stack1, initial_y);
		SPush(&stack2, initial_x);
		SPush(&stack3, 0);  // stack�� ����
	}

	else
	{
		while (maze[initial_y - 1][initial_x - 1] == 1)
		{
			printf("Please re-enter. ex) x,y  ");
			scanf_s("%d , %d", &initial_x, &initial_y);
		}  // �Է¹��� �Ա��� ��ȿ���� ���� ��� �ٸ� ����Ʈ �Է¹���

		SPush(&stack1, initial_y);
		SPush(&stack2, initial_x);
		SPush(&stack3, 0);  // �Է¹��� ��ȿ�� �Ա� ������ stack�� ����
	}

	maze[initial_y - 1][initial_x - 1] = 4;

	while (*top!=NULL && find == 0)  // stack�� empty�� �ǰ� �ⱸ�� ã�� ������ �ݺ�
	{
		location_n = SPop(&stack1);
		location_n = SPop(&stack2);
		location_n = SPop(&stack3);
		a = location_n.a;
		b = location_n.b;
		c = location_n.c;  // ���ٸ� ���� ��� �Դ� ��θ� pop�Ͽ� �� �� �ִ� ���� ���� ������ �ǵ��ư�

		while (c < 4 && find == 0)  // �ⱸ�� ã�� ������ �������� Ž�� �ݺ�
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
			}                       // �ⱸ�� ã�� ���

			else if (maze[next_x][next_y] == 0 && temp[next_x][next_y] == 0)  // �� �� �ִ� ��ΰ� �ִ� ���
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
				++c;  // �� �� ���� ����� ���
		}
	}

	printf("\n");

	if (find)  // �ⱸ�� ã�� ���
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

	else  // ��ΰ� ���� ���
	{
		printf("(%d,%d)", initial_x, initial_y);
		printf("\n");
		printf("There is no path. \n\n");
		system("pause");
	}
}