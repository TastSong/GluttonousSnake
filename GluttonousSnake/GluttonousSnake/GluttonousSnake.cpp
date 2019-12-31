#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<stdlib.h>
#define U 1
#define D 2
#define L 3
#define R 4 //蛇的状态，U：上 ；D：下；L:左 R：右

/*  表示蛇身上的小点*  */
typedef struct SNAKE
{
	int x;/*横坐标*/
	int y;/*纵坐标*/
	struct SNAKE *next;/*下一个蛇身子的节点指针*/
}snake;

//全局变量//

int score = 0, add = 10;/* 总得分与每次吃食物得分 */
int status;/* 按键数字的存放  */
int sleeptime = 200;/* 每次运行的时间间隔 */
snake *head, *food;/*蛇头指针，食物指针*/
snake *q;/* 蛇的结构体指针 */
int endgamestatus = 0; /*游戏结束的情况，1：撞到墙；2：咬到自己；*/

//声明全部函数//
void Pos();//设置光标位置
void CreatInterface();//创建游戏界面 
void InitializeSnake();//初始化蛇的身体 
int  BiteItself(); //判断是否咬到自己 
void CreateFood();//创建食物 
void CantcrossWall();//撞墙 
void SnakeMove();//蛇移动 
void Pause();//暂停 
void GameCircle();//运行 
void WelcomeToGame();//欢迎界面 
void EndGame();//结束游戏 
void GameStart();//开始游戏 

void Pos(int x, int y)
{
	COORD pos;//pos是结构体类型变量 
	HANDLE hOutput;//新建的光标句柄 
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);//句柄的实例化 
	SetConsoleCursorPosition(hOutput, pos);//SetConsoleCursorPosition是API中定位光标的函数 
}

void CreatInterface()
{
	int i;
	for (i = 0; i<58; i += 2)//打印上下边框
	{
		Pos(i, 0);
		printf("-");//上边框 
		Pos(i, 26);
		printf("-");//下边框 
	}
	for (i = 1; i<26; i++)//打印左右边框
	{
		Pos(0, i);
		printf("-");//左边框 
		Pos(56, i);
		printf("-");//右边框 
	}
}

void InitializeSnake()
{
	snake *tail;
	int i;
	tail = (snake*)malloc(sizeof(snake));//从蛇尾开始，头插法，用x,y设定开始的位置
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
	while (tail != NULL)//从头到尾，输出蛇身
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
		if (self->x == head->x && self->y == head->y)//头的坐标和其他身子的坐标重合 
		{
			return 1;//咬到 
		}
		self = self->next;
	}
	return 0;//没有咬到 
}

void CreateFood()//随机出现食物
{
	snake *food_1;
	srand((unsigned)time(NULL));
	food_1 = (snake*)malloc(sizeof(snake));
	do //保证其为偶数，使得食物能与蛇头对齐
	{
		food_1->x = rand() % 52 + 2;//随机生成食物的横坐标 
	} while ((food_1->x % 2) != 0);
	food_1->y = rand() % 24 + 1;//随机生成食物的纵坐标 
	q = head;
	while (q->next != NULL)
	{
		if (q->x == food_1->x && q->y == food_1->y) //判断蛇身是否与食物重合
		{
			free(food_1);
			food_1 = NULL;
			CreateFood();
			break;
		}
		q = q->next;
	}
	if (food_1 != NULL)//食物要是不存在，就随机生成食物 
	{
		Pos(food_1->x, food_1->y);
		food = food_1;
		printf("￥");
	}
}

void CantcrossWall()
{
	if (head->x == 0 || head->x == 56 || head->y == 0 || head->y == 26)//蛇头的横坐标纵坐标撞到了边界，就把endgamestatus设置为情况1 
	{
		endgamestatus = 1;//1,表示穿墙 
		EndGame();
	}
}

void SnakeMove()//蛇前进,上 U,下 D,左 L,右 R
{
	snake  *nexthead;
	CantcrossWall();//首先蛇不可以撞墙 
	nexthead = (snake*)malloc(sizeof(snake));//动态分配内存空间 
	if (status == U)//向上方向键 
	{
		nexthead->x = head->x;//横坐标不变，纵坐标减一 
		nexthead->y = head->y - 1;
		if (nexthead->x == food->x && nexthead->y == food->y)//如果下一个有食物//
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
			score = score + add;//计算新得分 
			CreateFood();//吃完食物后生成食物 
		}

		else //如果下一个位置没有食物
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
	if (status == D)//向下移动 
	{
		nexthead->x = head->x;//横坐标不变，纵坐标加一 
		nexthead->y = head->y + 1;
		if (nexthead->x == food->x && nexthead->y == food->y) //下一个节点有食物
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
		else //下一节点没有食物
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
	if (status == L)//向左移动 
	{
		nexthead->x = head->x - 2;//横坐标减2，纵坐标不变 
		nexthead->y = head->y;
		if (nexthead->x == food->x && nexthead->y == food->y)//下一节点有食物
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
		else //下一节点没有食物
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
	if (status == R)//向右移动 
	{
		nexthead->x = head->x + 2;//横坐标加2，纵坐标不变 
		nexthead->y = head->y;
		if (nexthead->x == food->x && nexthead->y == food->y)//下一节点有食物
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
		else //下一个节点没有食物
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
	if (BiteItself() == 1) //判断是否会咬到自己
	{
		endgamestatus = 2;
		EndGame();//咬到自己就会结束游戏 
	}
}

void Pause()//游戏暂停
{
	while (1)
	{
		Sleep(300);//延迟300ms 
	}
}

void GameCircle()//控制游戏
{

	Pos(64, 15);
	printf("不能穿墙，不能咬到自己");
	Pos(64, 16);
	printf("用上下左右键控制蛇的移动");
	Pos(64, 17);
	printf("ESC:退出游戏。");
	while (1)
	{
		Pos(64, 10);
		printf("得分：%d", score);
		Pos(64, 11);
		printf("每个食物的得分：%d", add);
		if (GetAsyncKeyState(VK_UP) && status != D)
		{
			status = U;//对应下一个按键状态 
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
		Sleep(sleeptime);//休息一段时间 
		SnakeMove();
	}
}

void WelcomeToGame()//欢迎来到游戏的显示界面
{
	Pos(20, 12);
	printf("欢迎来到贪吃蛇小游戏\n");
	system("Pause");//利用系统函数冻结屏幕 
	system("cls");//利用系统函数清屏，实现动态刷新 
}

void EndGame()//结束游戏
{
	system("cls");
	Pos(24, 12);
	if (endgamestatus == 1)
	{

		printf("你撞到墙壁了，游戏结束！\n");

	}
	else if (endgamestatus == 2)
	{
		printf("你咬到自己了，游戏结束！");
	}

	Pos(24, 13);
	printf("您的分数是%d\n", score);
	//	exit(0);
}

void GameStart()
{
	system("mode con cols=100 lines=30");  //将界面的大小设置为100列和30行 ,要是默认的话会有溢出 
	WelcomeToGame();
	CreatInterface();
	InitializeSnake();
	CreateFood();
}
int main()
{
	GameStart();//开始 
	GameCircle();//运转 
	EndGame();//结束 
	return 0;
}
