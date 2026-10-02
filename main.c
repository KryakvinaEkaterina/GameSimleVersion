#include <stdlib.h>
#include <time.h>
#include <locale.h>




#define SIZE 7

/* Глобальные переменные */
int health = 100;
int energy = 100;
int keys = 0;
int rooms = 0;
int escaped = 0;

/* Прототипы функций */
void showMap(char map[SIZE][SIZE], int playerX, int playerY); /* показываем карту*/
void enterRoom(char map[SIZE][SIZE], int x, int y);/* отвечает за события при входе в комнату*/
void randomEvent();
void showStatus();/* просто показываем характеристики игрока*/
void roomCounter();/* считает посещения комнат*/

int main()
{
	setlocale(LC_ALL, ""); /* теперь можно писать по русски*/
	/* Локальные переменные */
	char map[SIZE][SIZE] = {
		{'#', '#', '#', '#', '#', '#', '#'},
		{'#', ' ', ' ', ' ', '#', ' ', '#'},
		{'#', ' ', '#', ' ', '#', ' ', '#'},
		{'#', ' ', '#', ' ', ' ', ' ', '#'},
		{'#', ' ', '#', '#', '#', ' ', '#'},
		{'#', ' ', ' ', ' ', 'E', ' ', '#'},
		{'#', '#', '#', '#', '#', '#', '#'}
	};
	
	int playerX = 1; /*координаты игрока*/
	int playerY = 1;
	char move;
	
	srand(time(NULL));/* time- текущее время , srand - устанавливает начальное значение случайных чисел*/
	
	printf("=====================================\n");
	printf(" ESCAPE FROM LABORATORY\n");
	printf("=====================================\n\n");
	
	printf("Ты просыпаешься в неизвестной лаборатории.\n");
	printf("Система безопасности заблокировала выход.\n");
	printf("Тебе нужно найти ключ и выбраться.\n\n");
	
	printf("Управление:\n");
	printf("W - вверх\n");
	printf("S - вниз\n");
	printf("A - влево\n");
	printf("D - вправо\n");
	printf("Q - выйти из игры\n\n");
	
	/* Главный игровой цикл */
	while (health > 0 && escaped == 0)
	{
		showMap(map, playerX, playerY);
		showStatus();
		
		printf("\nТвой ход: ");
		scanf(" %c", &move);
		
		/* Проверка выхода из игры */
		if (move == 'q' || move == 'Q')
		{
			printf("\nТы покинул игру.\n");
			break;
		}
		
		/* Изменение координат */
		if (move == 'w' || move == 'W')
		{
			playerX--;
		}
		else if (move == 's' || move == 'S')
		{
			playerX++;
		}
		else if (move == 'a' || move == 'A')
		{
			playerY--;
		}
		else if (move == 'd' || move == 'D')
		{
			playerY++;
		}
		else
		{
			printf("\nНеизвестная команда!\n");
			continue;
		}
		
		/* Проверка столкновения со стеной */
		if (map[playerX][playerY] == '#')
		{
			printf("\nТы ударился о стену!\n");
			
			/* Возвращаем игрока назад */
			if (move == 'w' || move == 'W')
				playerX++;
			else if (move == 's' || move == 'S')
				playerX--;
			else if (move == 'a' || move == 'A')
				playerY++;
			else if (move == 'd' || move == 'D')
				playerY--;
			
			energy -= 2;
		}
		else
		{
			/* Игрок успешно переместился */
			energy -= 3;
			
			rooms++;
			
			roomCounter();
			
			/* Если игрок вошёл в комнату */
			if (map[playerX][playerY] == ' ')
			{
				enterRoom(map, playerX, playerY);
				randomEvent();
			}
			
			/* Если игрок нашёл выход */
			if (map[playerX][playerY] == 'E')
			{
				if (keys >= 1)
				{
					escaped = 1;
				}
				else
				{
					printf("\n=====================================\n");
					printf("ДВЕРЬ ЗАПЕРТА!\n");
					printf("Тебе нужен ключ.\n");
					printf("=====================================\n");
					
					/* Возвращаем игрока назад */
					if (move == 'w' || move == 'W')
						playerX++;
					else if (move == 's' || move == 'S')
						playerX--;
					else if (move == 'a' || move == 'A')
						playerY++;
					else if (move == 'd' || move == 'D')
						playerY--;
				}
				
			}
		}
		
		/* Проверка энергии */
		if (energy <= 0)
		{
			energy = 0;
			printf("\nТы полностью вымотался...\n");
			health -= 10;
		}
		
		printf("\n");
	}
	
	/* Конец игры */
	if (escaped == 1)
	{
		printf("\n");
		printf("=====================================\n");
		printf(" ТЫ ВЫБРАЛСЯ!\n");
		printf("=====================================\n");
		printf("Лаборатория осталась позади.\n");
		printf("Здоровье: %d\n", health);
		printf("Энергия: %d\n", energy);
		printf("Посещено комнат: %d\n", rooms);
		printf("\nПоздравляем! Ты успешно сбежал.\n");
	}
	else if (health <= 0)
	{
		printf("\n");
		printf("=====================================\n");
		printf(" GAME OVER\n");
		printf("=====================================\n");
		printf("Ты не смог выбраться из лаборатории.\n");
	}
	
	return 0;
}


/* Вывод карты */
void showMap(char map[SIZE][SIZE], int playerX, int playerY)
{
	printf("\n\n");
	printf("----------- КАРТА -----------\n");
	
	/*
	Вложенные циклы:
	внешний for перебирает строки,
	внутренний for перебирает столбцы.
	*/
	
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (i == playerX && j == playerY)
			{
				printf("@ ");
			}
			else
			{
				printf("%c ", map[i][j]);
			}
		}
		
		printf("\n");
	}
	
	printf("------------------------------\n");
}


/* Обработка комнаты */
void enterRoom(char map[SIZE][SIZE], int x, int y)
{
	/* Локальные переменные */
	int choice;/* выбор пользователья 1&&2 */
	int chance;
	
	chance = rand() % 4; /* случайное число от 0 до 3 */
	
	printf("\nТы вошёл в неизвестную комнату.\n");
	
	if (chance == 0)
	{
		printf("\nНа столе лежит старый ящик.\n");
		printf("Открыть его?\n");
		printf("1 - Да\n");
		printf("2 - Нет\n");
		printf("Выбор: ");
		
		scanf("%d", &choice);
		
		if (choice == 1)
		{
			printf("\nТы открываешь ящик...\n");
			
			if (rand() % 2 == 0) /* случайный результат 0 && 1 */
			{
				printf("Внутри оказался КЛЮЧ!\n");
				keys++;
			}
			else
			{
				printf("Ящик оказался пустым.\n");
			}
		}
		else
		{
			printf("\nТы решил не рисковать.\n");
		}
	}
	else if (chance == 1)
	{
		printf("\nТы нашёл аптечку!\n");
		
		health += 20;
		
		if (health > 100)
		{
			health = 100;
		}
		
		printf("Здоровье восстановлено.\n");
	}
	else if (chance == 2)
	{
		printf("\nВ комнате ничего нет.\n");
		printf("Слишком тихо...\n");
	}
	else
	{
		printf("\nТы обнаружил энергетический напиток.\n");
		energy += 25;
		
		if (energy > 100)
		{
			energy = 100;
		}
		
		printf("Энергия восстановлена.\n");
	}
}


/* Случайное событие */
void randomEvent()
{
	int event;
	
	event = rand() % 5;
	
	if (event == 0)
	{
		printf("\n ВНИМАНИЕ!\n");
		printf("Сработала система безопасности!\n");
		
		health -= 15;
		
		printf("Ты потерял 15 здоровья.\n");
	}
	else if (event == 1)
	{
		printf("\nТы услышал странный шум в вентиляции...\n");
	}
	else if (event == 2)
	{
		printf("\nЛампочки начинают мигать.\n");
		printf("Энергия лаборатории нестабильна.\n");
	}
	else if (event == 3)
	{
		printf("\nТы обнаружил скрытый проход!\n");
		energy += 10;
		
		if (energy > 100)
		{
			energy = 100;
		}
	}
	else
	{
		printf("\nКомната безопасна.\n");
	}
}


/* Вывод характеристик */
void showStatus()
{
	printf("\n");
	printf("Здоровье: %d/100\n", health);
	printf("Энергия: %d/100\n", energy);
	printf("Ключи: %d\n", keys);
	printf("Комнаты: %d\n", rooms);
}


/* Статическая переменная */
void roomCounter()
{
	static int visits = 0;
	
	visits++;
	
	if (visits == 1)
	{
		printf("\nЭто твоя первая исследованная комната.\n");
	}
	else
	{
		printf("\nВсего посещений комнат за игру: %d\n", visits);
	}
}
