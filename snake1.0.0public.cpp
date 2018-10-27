//filename:main.c
//author:PenG

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <Windows.h>
#include <time.h>





//�����ͼ�Ŀ�͸�
#define WIDTH 26
#define HEIGHT 20

//�����ͼ�нڵ�����
#define ISEMPTY 0
#define ISBODY 1
#define ISFOOD 2
#define ISWALL 3

//�����ĸ�������Ƽ���һ����ͣ��
#define KEY_UP '8'
#define KEY_DOWN '2'
#define KEY_LEFT '4'
#define KEY_RIGHT '6'
#define KEY_SUSPEND '5'





//������Ϸ�ٶ�
const int GAME_SPEED = 24;

//����ת��ķ���
enum D { UP, RIGHT, DOWN, LEFT };
//���嵱ǰ���������ƶ�ת��ʱ�Ƚ�
enum D tempD = RIGHT;

typedef struct snake   //�洢������Ϣ
{
	int x;  //����ڵ� x ����
	int y;  //����ڵ� y ����
	struct snake *last;  //��һ����ڵ��ַ
}snake;

//��ͼ����
char map[HEIGHT][WIDTH];

//�����ߺ�ʳ���ָ�룬ʳ��Ľṹ��ʵ��������Ľṹһ�� 
snake *tail, *body, *head, *food;

//��һβ��λ��
snake last_tail;

int snake_length = 3;  //�ߵĳ��ȣ���ʼ��Ϊ3














//������Ϸ
void start(void);

//��ʼ����
void initSnake(void);

//���ߵ���Ϣд����ͼ��
void writeSnake(void);

//��һ��ʳ��
void initFood(void);

//��ʼ��ǽ
void initWall(void);

//�����ͼ���ߵ�����
void cleanSnake(void);

//�����򣬷�����ȷ����
enum D judgeDirection(enum D direction);

//������������
void cleanInput(void);

//��ȡ����ķ���
enum D getDirection(void);

//�ж���û�гԵ�ʳ��Ե��˾����ӳ���
//�Ե�ʳ�ﷵ�� true�����򷵻� false
bool eatFood(enum D direction);

//��map�����������ʾ����
//����Ҫ��һ��
void printMap(void);

//�ƶ��ߣ�ע�ⲻҪ����������ˣ�
void moveSnake(enum D direction);

//�����û��ײ��ǽ
bool knockWall(void);

//�����û��ײ���Լ�������
bool biteSelf(void);

//�ͷ��ߵ��ڴ�
void freeSnake(void);















int main(void)
{

	start();

	return 0;
}

//������Ϸ
void start(void)
{
	int size = (WIDTH - 2) * (HEIGHT - 2);

	initSnake();
	initFood();
	initWall();

	while (1)
	{
		//clrscr();
		//����
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

			//����������ߵ�����
			moveSnake(tempD);

		}
		printMap();

		//ײ��ǽ����ҧ���Լ������¿�ʼ
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

void initSnake(void)  //��ʼ����
{
	//�߳�ʼ��λ��
	int x, y;
	x = 2;
	y = 1;

	//��ȡ�ڴ�ռ�
	tail = (snake *)malloc(sizeof(snake));
	body = (snake *)malloc(sizeof(snake));
	head = (snake *)malloc(sizeof(snake));

	//��λ����Ϣд��������
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

//���ߵ���Ϣд����ͼ��
void writeSnake(void)
{
	body = tail;	//���ܶ�tail
	while (body != NULL)
	{
		map[body->y][body->x] = ISBODY;
		body = body->last;
	}  //while  end
}

//��һ��ʳ��
void initFood(void)
{
	int tx, ty;
	food = (snake *)malloc(sizeof(snake));

	//��ȡһ�������λ�ã�����Ҫ�ܿ�ǽ���ߵ�λ��
	srand((unsigned)time(NULL));
	do
	{
		tx = (rand() % (WIDTH - 2)) + 1;
		ty = (rand() % (HEIGHT - 2)) + 1;
	} while (map[ty][tx] != ISEMPTY //ȡ���յ�λ��
				|| tx == 0 || ty == 0 || tx == (WIDTH - 1) || ty == (HEIGHT - 1));	//�ܿ�ǽ

	food->x = tx;
	food->y = ty;
	food->last = NULL;
	map[ty][tx] = ISFOOD;
}

//��ʼ��ǽ
void initWall(void)
{
	//�����±�Ե
	for (int i = 0; i < WIDTH; i++)
	{
		map[0][i] = ISWALL;
		map[HEIGHT - 1][i] = ISWALL;
	}
	//�����ұ�Ե���ظ������ĸ���Ҳû��ϵ
	for (int i = 0; i < HEIGHT; i++)
	{
		map[i][0] = ISWALL;
		map[i][WIDTH - 1] = ISWALL;
	}
}

//�����ͼ���ߵ�����
void cleanSnake(void)
{
	body = tail;
	while (body != NULL)
	{
		map[body->y][body->x] = ISEMPTY;
		body = body->last;
	}  //while  end
}

//�����򣬷�����ȷ����
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

//������������
void cleanInput(void)
{
	while (_kbhit())
	{
		_getch();
	}
}

//��ȡ����ķ���
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

//�ж���û�гԵ�ʳ��Ե��˾����ӳ���
//�Ե�ʳ�ﷵ�� true�����򷵻� flase
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

	//�жϴ�ʱ�Ƿ�Ե�ʳ��
	if (food->x == tx && food->y == ty)
	{
		head->last = food;
		head = head->last;
		return true;
	}
	return false;
}

//��map�����������ʾ����
//����Ҫ��һ��
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

//�ƶ��ߣ�ע�ⲻҪ����������ˣ�
void moveSnake(enum D direction)
{
	int tx, ty;

	tempD = judgeDirection(direction);

	//β�Ʒ��ƶ���
	body = tail;	//����Ҫ�õ�tail
	last_tail.x = tail->x;
	last_tail.y = tail->y;
	while (body->last != NULL)
	{
		body = body->last;
	}  //while  end
	//�ظ����ÿռ䣬���˷��ˣ�
	//�����⼸��˳���ܱ�
	head->last = tail;
	tail = tail->last;
	head = head->last;
	head->last = NULL;

	//����ֻ��Ҫ���ݷ���������ͷ��λ�þ�����
	//��ʱbodyָ��head���һ��body
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

//�����û��ײ��ǽ
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

//�����û��ײ���Լ�������
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

//�ͷ��ߵ��ڴ�
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