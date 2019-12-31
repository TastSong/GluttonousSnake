#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<stdlib.h>
#define U 1
#define D 2
#define L 3
#define R 4 //�ߵ�״̬��U���� ��D���£�L:�� R����

/*  ��ʾ�����ϵ�С��*  */
typedef struct SNAKE
{
	int x;/*������*/
	int y;/*������*/
	struct SNAKE *next;/*��һ�������ӵĽڵ�ָ��*/
}snake;

//ȫ�ֱ���//

int score = 0, add = 10;/* �ܵ÷���ÿ�γ�ʳ��÷� */
int status;/* �������ֵĴ��  */
int sleeptime = 200;/* ÿ�����е�ʱ���� */
snake *head, *food;/*��ͷָ�룬ʳ��ָ��*/
snake *q;/* �ߵĽṹ��ָ�� */
int endgamestatus = 0; /*��Ϸ�����������1��ײ��ǽ��2��ҧ���Լ���*/

//����ȫ������//
void Pos();//���ù��λ��
void CreatInterface();//������Ϸ���� 
void InitializeSnake();//��ʼ���ߵ����� 
int  BiteItself(); //�ж��Ƿ�ҧ���Լ� 
void CreateFood();//����ʳ�� 
void CantcrossWall();//ײǽ 
void SnakeMove();//���ƶ� 
void Pause();//��ͣ 
void GameCircle();//���� 
void WelcomeToGame();//��ӭ���� 
void EndGame();//������Ϸ 
void GameStart();//��ʼ��Ϸ 

void Pos(int x, int y)
{
	COORD pos;//pos�ǽṹ�����ͱ��� 
	HANDLE hOutput;//�½��Ĺ���� 
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);//�����ʵ���� 
	SetConsoleCursorPosition(hOutput, pos);//SetConsoleCursorPosition��API�ж�λ���ĺ��� 
}

void CreatInterface()
{
	int i;
	for (i = 0; i<58; i += 2)//��ӡ���±߿�
	{
		Pos(i, 0);
		printf("-");//�ϱ߿� 
		Pos(i, 26);
		printf("-");//�±߿� 
	}
	for (i = 1; i<26; i++)//��ӡ���ұ߿�
	{
		Pos(0, i);
		printf("-");//��߿� 
		Pos(56, i);
		printf("-");//�ұ߿� 
	}
}

void InitializeSnake()
{
	snake *tail;
	int i;
	tail = (snake*)malloc(sizeof(snake));//����β��ʼ��ͷ�巨����x,y�趨��ʼ��λ��
	tail->x = 24;
	tail->y = 5;
	tail->next = NULL;
	for (i = 1; i <= 4; i++)
	{
		head = (snake*)malloc(sizeof(snake));
		head->next = tail;
		head->x = 24 + 2 * i;
		head->y = 5;
		tail = head;
	}
	while (tail != NULL)//��ͷ��β���������
	{
		Pos(tail->x, tail->y);
		printf("*");
		tail = tail->next;
	}
}

int BiteItself()
{
	snake *self;
	self = head->next;
	while (self != NULL)
	{
		if (self->x == head->x && self->y == head->y)//ͷ��������������ӵ������غ� 
		{
			return 1;//ҧ�� 
		}
		self = self->next;
	}
	return 0;//û��ҧ�� 
}

void CreateFood()//�������ʳ��
{
	snake *food_1;
	srand((unsigned)time(NULL));
	food_1 = (snake*)malloc(sizeof(snake));
	do //��֤��Ϊż����ʹ��ʳ��������ͷ����
	{
		food_1->x = rand() % 52 + 2;//�������ʳ��ĺ����� 
	} while ((food_1->x % 2) != 0);
	food_1->y = rand() % 24 + 1;//�������ʳ��������� 
	q = head;
	while (q->next != NULL)
	{
		if (q->x == food_1->x && q->y == food_1->y) //�ж������Ƿ���ʳ���غ�
		{
			free(food_1);
			food_1 = NULL;
			CreateFood();
			break;
		}
		q = q->next;
	}
	if (food_1 != NULL)//ʳ��Ҫ�ǲ����ڣ����������ʳ�� 
	{
		Pos(food_1->x, food_1->y);
		food = food_1;
		printf("��");
	}
}

void CantcrossWall()
{
	if (head->x == 0 || head->x == 56 || head->y == 0 || head->y == 26)//��ͷ�ĺ�����������ײ���˱߽磬�Ͱ�endgamestatus����Ϊ���1 
	{
		endgamestatus = 1;//1,��ʾ��ǽ 
		EndGame();
	}
}

void SnakeMove()//��ǰ��,�� U,�� D,�� L,�� R
{
	snake  *nexthead;
	CantcrossWall();//�����߲�����ײǽ 
	nexthead = (snake*)malloc(sizeof(snake));//��̬�����ڴ�ռ� 
	if (status == U)//���Ϸ���� 
	{
		nexthead->x = head->x;//�����겻�䣬�������һ 
		nexthead->y = head->y - 1;
		if (nexthead->x == food->x && nexthead->y == food->y)//�����һ����ʳ��//
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL)
			{
				Pos(q->x, q->y);
				printf("*");
				q = q->next;
			}
			score = score + add;//�����µ÷� 
			CreateFood();//����ʳ�������ʳ�� 
		}

		else //�����һ��λ��û��ʳ��
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL)
			{
				Pos(q->x, q->y);
				printf("*");
				q = q->next;
			}
			Pos(q->next->x, q->next->y);
			printf(" ");
			free(q->next);
			q->next = NULL;
		}
	}
	if (status == D)//�����ƶ� 
	{
		nexthead->x = head->x;//�����겻�䣬�������һ 
		nexthead->y = head->y + 1;
		if (nexthead->x == food->x && nexthead->y == food->y) //��һ���ڵ���ʳ��
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL)
			{
				Pos(q->x, q->y);
				printf("*");
				q = q->next;
			}
			score = score + add;
			CreateFood();
		}
		else //��һ�ڵ�û��ʳ��
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL)
			{
				Pos(q->x, q->y);
				printf("*");
				q = q->next;
			}
			Pos(q->next->x, q->next->y);
			printf(" ");
			free(q->next);
			q->next = NULL;
		}
	}
	if (status == L)//�����ƶ� 
	{
		nexthead->x = head->x - 2;//�������2�������겻�� 
		nexthead->y = head->y;
		if (nexthead->x == food->x && nexthead->y == food->y)//��һ�ڵ���ʳ��
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL)
			{
				Pos(q->x, q->y);
				printf("*");
				q = q->next;
			}
			score = score + add;
			CreateFood();
		}
		else //��һ�ڵ�û��ʳ��
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL)
			{
				Pos(q->x, q->y);
				printf("*");
				q = q->next;
			}
			Pos(q->next->x, q->next->y);
			printf(" ");
			free(q->next);
			q->next = NULL;
		}
	}
	if (status == R)//�����ƶ� 
	{
		nexthead->x = head->x + 2;//�������2�������겻�� 
		nexthead->y = head->y;
		if (nexthead->x == food->x && nexthead->y == food->y)//��һ�ڵ���ʳ��
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL)
			{
				Pos(q->x, q->y);
				printf("*");
				q = q->next;
			}
			score = score + add;
			CreateFood();
		}
		else //��һ���ڵ�û��ʳ��
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL)
			{
				Pos(q->x, q->y);
				printf("*");
				q = q->next;
			}
			Pos(q->next->x, q->next->y);
			printf(" ");
			free(q->next);
			q->next = NULL;
		}
	}
	if (BiteItself() == 1) //�ж��Ƿ��ҧ���Լ�
	{
		endgamestatus = 2;
		EndGame();//ҧ���Լ��ͻ������Ϸ 
	}
}

void Pause()//��Ϸ��ͣ
{
	while (1)
	{
		Sleep(300);//�ӳ�300ms 
	}
}

void GameCircle()//������Ϸ
{

	Pos(64, 15);
	printf("���ܴ�ǽ������ҧ���Լ�");
	Pos(64, 16);
	printf("���������Ҽ������ߵ��ƶ�");
	Pos(64, 17);
	printf("ESC:�˳���Ϸ��");
	while (1)
	{
		Pos(64, 10);
		printf("�÷֣�%d", score);
		Pos(64, 11);
		printf("ÿ��ʳ��ĵ÷֣�%d", add);
		if (GetAsyncKeyState(VK_UP) && status != D)
		{
			status = U;//��Ӧ��һ������״̬ 
		}
		else if (GetAsyncKeyState(VK_DOWN) && status != U)
		{
			status = D;
		}
		else if (GetAsyncKeyState(VK_LEFT) && status != R)
		{
			status = L;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && status != L)
		{
			status = R;
		}
		Sleep(sleeptime);//��Ϣһ��ʱ�� 
		SnakeMove();
	}
}

void WelcomeToGame()//��ӭ������Ϸ����ʾ����
{
	Pos(20, 12);
	printf("��ӭ����̰����С��Ϸ\n");
	system("Pause");//����ϵͳ����������Ļ 
	system("cls");//����ϵͳ����������ʵ�ֶ�̬ˢ�� 
}

void EndGame()//������Ϸ
{
	system("cls");
	Pos(24, 12);
	if (endgamestatus == 1)
	{

		printf("��ײ��ǽ���ˣ���Ϸ������\n");

	}
	else if (endgamestatus == 2)
	{
		printf("��ҧ���Լ��ˣ���Ϸ������");
	}

	Pos(24, 13);
	printf("���ķ�����%d\n", score);
	//	exit(0);
}

void GameStart()
{
	system("mode con cols=100 lines=30");  //������Ĵ�С����Ϊ100�к�30�� ,Ҫ��Ĭ�ϵĻ�������� 
	WelcomeToGame();
	CreatInterface();
	InitializeSnake();
	CreateFood();
}
int main()
{
	GameStart();//��ʼ 
	GameCircle();//��ת 
	EndGame();//���� 
	return 0;
}
