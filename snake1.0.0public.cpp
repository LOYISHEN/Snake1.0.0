//filename:main.c
//author:PenG

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <Windows.h>
#include <time.h>





//定义地图的宽和高
#define WIDTH 26
#define HEIGHT 20

//定义地图中节点类型
#define ISEMPTY 0
#define ISBODY 1
#define ISFOOD 2
#define ISWALL 3

//定义四个方向控制键和一个暂停键
#define KEY_UP '8'
#define KEY_DOWN '2'
#define KEY_LEFT '4'
#define KEY_RIGHT '6'
#define KEY_SUSPEND '5'





//定义游戏速度
const int GAME_SPEED = 24;

//定义转向的方向
enum D { UP, RIGHT, DOWN, LEFT };
//定义当前方向，用于移动转向时比较
enum D tempD = RIGHT;

typedef struct snake   //存储身体信息
{
	int x;  //身体节点 x 坐标
	int y;  //身体节点 y 坐标
	struct snake *last;  //上一身体节点地址
}snake;

//地图数组
char map[HEIGHT][WIDTH];

//定义蛇和食物的指针，食物的结构其实跟蛇身体的结构一样 
snake *tail, *body, *head, *food;

//上一尾巴位置
snake last_tail;

int snake_length = 3;  //蛇的长度，初始化为3














//启动游戏
void start(void);

//初始化蛇
void initSnake(void);

//把蛇的信息写到地图中
void writeSnake(void);

//放一个食物
void initFood(void);

//初始化墙
void initWall(void);

//清除地图上蛇的身体
void cleanSnake(void);

//处理方向，返回正确方向
enum D judgeDirection(enum D direction);

//清除多余的输入
void cleanInput(void);

//获取输入的方向
enum D getDirection(void);

//判断有没有吃到食物，吃到了就增加长度
//吃到食物返回 true，否则返回 false
bool eatFood(enum D direction);

//把map数组的数据显示出来
//这里要改一下
void printMap(void);

//移动蛇（注意不要让蛇向后走了）
void moveSnake(enum D direction);

//检测有没有撞到墙
bool knockWall(void);

//检测有没有撞到自己的身体
bool biteSelf(void);

//释放蛇的内存
void freeSnake(void);















int main(void)
{

	start();

	return 0;
}

//启动游戏
void start(void)
{
	int size = (WIDTH - 2) * (HEIGHT - 2);

	initSnake();
	initFood();
	initWall();

	while (1)
	{
		//clrscr();
		//清屏
		system("cls");

		
		cleanSnake();
		tempD = judgeDirection(getDirection());
		if (eatFood(tempD))
		{
			writeSnake();
			snake_length++;

			if (snake_length >= size)
			{
				printMap();
				
				break;
			}

			initFood();
		}
		else
		{

			//在这里控制蛇的走向
			moveSnake(tempD);

		}
		printMap();

		//撞到墙或者咬到自己就重新开始
		if (knockWall() || biteSelf())
		{
			cleanInput();
			Sleep(1);
			printf("\nYou lose!!");
			printf("\nPress any key to continue...");
			_getch();
			system("cls");
			cleanSnake();
			freeSnake();
			map[food->y][food->x] = ISEMPTY;
			free(food);
			initSnake();
			initFood();
			initWall();
			tempD = RIGHT;
			snake_length = 3;
		}  //if  end
		Sleep(50 / GAME_SPEED);

	}  //while (main cycle) end
	
	system("cls");
	printf("WoW!!\nYou win!!");
}

void initSnake(void)  //初始化蛇
{
	//蛇初始的位置
	int x, y;
	x = 2;
	y = 1;

	//获取内存空间
	tail = (snake *)malloc(sizeof(snake));
	body = (snake *)malloc(sizeof(snake));
	head = (snake *)malloc(sizeof(snake));

	//把位置信息写入链表中
	tail->x = x - 1;
	tail->y = y;
	tail->last = body;
	body->x = x;
	body->y = y;
	body->last = head;
	head->x = x + 1;
	head->y = y;
	head->last = NULL;

	writeSnake();
}

//把蛇的信息写到地图中
void writeSnake(void)
{
	body = tail;	//不能动tail
	while (body != NULL)
	{
		map[body->y][body->x] = ISBODY;
		body = body->last;
	}  //while  end
}

//放一个食物
void initFood(void)
{
	int tx, ty;
	food = (snake *)malloc(sizeof(snake));

	//获取一个随机的位置，而且要避开墙和蛇的位置
	srand((unsigned)time(NULL));
	do
	{
		tx = (rand() % (WIDTH - 2)) + 1;
		ty = (rand() % (HEIGHT - 2)) + 1;
	} while (map[ty][tx] != ISEMPTY //取到空的位置
				|| tx == 0 || ty == 0 || tx == (WIDTH - 1) || ty == (HEIGHT - 1));	//避开墙

	food->x = tx;
	food->y = ty;
	food->last = NULL;
	map[ty][tx] = ISFOOD;
}

//初始化墙
void initWall(void)
{
	//画上下边缘
	for (int i = 0; i < WIDTH; i++)
	{
		map[0][i] = ISWALL;
		map[HEIGHT - 1][i] = ISWALL;
	}
	//画左右边缘，重复画了四个角也没关系
	for (int i = 0; i < HEIGHT; i++)
	{
		map[i][0] = ISWALL;
		map[i][WIDTH - 1] = ISWALL;
	}
}

//清除地图上蛇的身体
void cleanSnake(void)
{
	body = tail;
	while (body != NULL)
	{
		map[body->y][body->x] = ISEMPTY;
		body = body->last;
	}  //while  end
}

//处理方向，返回正确方向
enum D judgeDirection(enum D direction)
{
	switch (direction)
	{
		case LEFT:
		case RIGHT:
			if (tempD != LEFT && tempD != RIGHT)
			{
				return direction;
			}
			else
			{
				return tempD;
			}
			break;

		case UP:
		case DOWN:
			if (tempD != UP && tempD != DOWN)
			{
				return direction;
			}
			else
			{
				return tempD;
			}
			break;

		default:
			//printf("error! at judgeDirection()");
			//assert(0);
			NULL;
			break;
	}  //switch  end
}

//清除多余的输入
void cleanInput(void)
{
	while (_kbhit())
	{
		_getch();
	}
}

//获取输入的方向
enum D getDirection(void)
{
	while (_kbhit())
	{
		switch (getch())
		{
			case KEY_UP:
				cleanInput();
				return UP;
				break;

			case KEY_DOWN:
				cleanInput();
				return DOWN;
				break;

			case KEY_LEFT:
				cleanInput();
				return LEFT;
				break;

			case KEY_RIGHT:
				cleanInput();
				return RIGHT;
				break;

			default:
				//printf("error! in getDirection()");
				//assert(0);
				NULL;
				break;
		}  //switch  end
	}  //while  end

	return tempD;
}

//判断有没有吃到食物，吃到了就增加长度
//吃到食物返回 true，否则返回 flase
bool eatFood(enum D direction)
{
	enum D t_dire = direction;

	int tx, ty;
	tx = head->x;
	ty = head->y;

	switch (t_dire)
	{
		case UP:
			ty--;
			break;

		case DOWN:
			ty++;
			break;

		case LEFT:
			tx--;
			break;

		case RIGHT:
			tx++;
			break;

		default:
			printf("error! in addSnake()");
			break;
	}  //switch end

	//判断此时是否吃到食物
	if (food->x == tx && food->y == ty)
	{
		head->last = food;
		head = head->last;
		return true;
	}
	return false;
}

//把map数组的数据显示出来
//这里要改一下
void printMap(void)
{
	for (int y = 0; y<HEIGHT; y++)
	{
		for (int x = 0; x<WIDTH; x++)
		{
			switch (map[y][x])
			{
				case ISWALL:
					printf("##");
					break;

				case ISEMPTY:
					printf("  ");
					break;

				case ISBODY:
					if (y == head->y && x == head->x)
					{
						printf("@@");
					}
					else if (y == tail->y && x == tail->x)
					{
						printf("oo");
					}
					else
					{
						printf("OO");
					}
					break;

				case ISFOOD:
					printf("$$");
					break;
				default:
					NULL;
					break;
			}
		}  //inside for  end
		printf("\n");
	}  //outside for  end
}

//移动蛇（注意不要让蛇向后走了）
void moveSnake(enum D direction)
{
	int tx, ty;

	tempD = judgeDirection(direction);

	//尾推法移动蛇
	body = tail;	//下面要用到tail
	last_tail.x = tail->x;
	last_tail.y = tail->y;
	while (body->last != NULL)
	{
		body = body->last;
	}  //while  end
	//重复利用空间，别浪费了！
	//下面这几行顺序不能变
	head->last = tail;
	tail = tail->last;
	head = head->last;
	head->last = NULL;

	//下面只需要根据方向设置蛇头的位置就行了
	//此时body指向head后第一个body
	tx = body->x;
	ty = body->y;
	switch (tempD)
	{
		case UP:
			ty--;
			break;

		case DOWN:
			ty++;
			break;

		case LEFT:
			tx--;
			break;

		case RIGHT:
			tx++;
			break;

		default:
			//printf("error! at moveSnake()");
			//assert(0);
			NULL;
			break;
	}  //switch  end
	head->x = tx;
	head->y = ty;

	writeSnake();
}

//检测有没有撞到墙
bool knockWall(void)
{
	int tx = head->x, ty = head->y;
	if (tx == 0 || tx == (WIDTH - 1) || ty == 0 || ty == (HEIGHT - 1))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//检测有没有撞到自己的身体
bool biteSelf(void)
{
	int tx = head->x, ty = head->y;
	body = tail;
	while (body->last != NULL)
	{
		if (body->x == tx && body->y == ty)
		{
			return true;
		}
		body = body->last;
	}  //while  end
	return false;
}

//释放蛇的内存
void freeSnake(void)
{
	body = tail;
	while (body != NULL)
	{
		tail = tail->last;
		free(body);
		body = tail;
	}  //while  end

	tail = NULL;
	body = NULL;
	head = NULL;
}