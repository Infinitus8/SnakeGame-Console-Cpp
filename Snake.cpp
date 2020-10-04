#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <ctime>

bool gameOver;

const int width = 20;
const int height = 20;
const int waitTime = 100;  // milliseconds 

int x, y, fruitX, fruitY, score;

int tailX[width * height], tailY[width * height];
int nTail;

int dirX[] = { 0, 1, 0, -1, 0 };
int dirY[] = { 0, 0, -1, 0, 1 };
int direction;

std::pair<int, int> generateUnoccupiedPos()
{
	int occupied = 1, line, column;
	while (occupied)
	{
		occupied = 0;
		line = rand() % (height-2) + 1;
		column = rand() % (width-2) + 1;
		for (int i = 0; i != nTail; i++)
		{
			if (tailX[i] == column && tailY[i] == line)
				occupied = 1;
		}
		if (x == column && y == line)
			occupied = 1;
	}
	return std::make_pair(line, column);
}


void newFruit()
{
	std::pair<int, int> pos = generateUnoccupiedPos();
	fruitX = pos.first;
	fruitY = pos.second;
}

char waitForInput(int time)
{
	char c = ' '; //default return
	while (time != 0)
	{
		if (_kbhit())
		{
			c = _getch(); //get the char
			//break;
		}

		Sleep(1);
		--time;
	}
	return c;
}

void Setup()
{
	srand(time(0));
	gameOver = false;
	direction = 0;
	score = 0;
	nTail = 0;
	x = width / 2;
	y = height / 2;
	std::pair<int, int> pos = generateUnoccupiedPos();
	newFruit();
}


void Draw()
{
	std::system("CLS");
	for (int i = 0; i != height + 1; i++)
	{
		for (int j = 0; j != width + 1; j++)
		{
			
			bool isEmpty = true;
			for (int t = 0; t != nTail; t++)
			{
				if (tailX[t] == j && tailY[t] == i)
				{
					std::cout << "0";
					isEmpty = false;
					break;
				}
			}
			
			if (i == 0 || i == height || j == 0 || j == width)
				std::cout << "#";
			else if (i == y && j == x)
				std::cout << "O";

			else if (i == fruitY && j == fruitX)
				std::cout << "%";
			else if (isEmpty)
				std::cout << " ";

				
		}
		std::cout << std::endl;
	}
	std::cout << std::endl << "Score: " << score;
}

void Input()
{
	char commandDir[128], command;
	
	commandDir[' '] = direction;
	commandDir['d'] = 1;
	commandDir['w'] = 2;
	commandDir['a'] = 3;
	commandDir['s'] = 4;

	command = waitForInput(waitTime);
	int currentDir = commandDir[command];
	// head can't rotate 180 degrees, otherwise when player miss clicked snake can hit his tail
	if ((dirX[direction] == -dirX[currentDir] && dirX[direction] != 0)
		|| (dirY[direction] == -dirY[currentDir] && dirY[direction] != 0))
		return;
	direction = currentDir;

}

void Logic()
{
	int nextX = x, nextY = y, tempX, tempY;

	x += dirX[direction];
	y += dirY[direction];
	
	for (int i = 0; i != nTail; i++)
	{
		tempX = tailX[i];
		tempY = tailY[i];
		tailX[i] = nextX;
		tailY[i] = nextY;
		nextX = tempX;
		nextY = tempY;

	}

	if (y == 0 || y == height || x == 0 || x == width)
		gameOver = true;

	for (int i = 0; i != nTail; i++)
		if (x == tailX[i] && y == tailY[i])
			gameOver = true;

	if (x == fruitX && y == fruitY)
	{
		newFruit();
		score += 10;
		nTail++;
		if (nTail == 1)
		{
			tailX[nTail - 1] = x - dirX[direction];
			tailY[nTail - 1] = y - dirY[direction];
		}
		else
		{
			tailX[nTail - 1] = tempX;
			tailY[nTail - 1] = tempY;
		}

	}
		
}

int main()
{
	Setup();
	while (!gameOver)
	{
		Draw();
		Input();
		Logic();
		std::cout << std::endl << fruitX << "  " << fruitY;
		
	}

	return 0;
}