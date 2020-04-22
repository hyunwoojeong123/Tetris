#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <ctime>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SPACE 32
#define WIDTH 12
#define HEIGHT 18
using namespace std;

char* brick[3] = { "  ", "��", "��" };
char* board[WIDTH+2][HEIGHT+2];
int score = 0;

struct tetrimino
{
	pair<int, int> st; // ���� x,y��ǥ
	int shape; // ��� 0~5
	int dir; // ����
};
//��Ʈ���� �� ��ǥ i : ��� j : ���� k : �� ���� ��ǥ
pair<int, int> block[6][4][4] =
{
	{{{1,0},{1,1},{1,2},{1,3}},{{0,1},{1,1},{2,1},{3,1}},{{2,0},{2,1},{2,2},{2,3}},{{0,2},{1,2},{2,2},{3,2}}},
	{{{1,0},{1,1},{2,1},{2,2}},{{0,2},{1,2},{1,1},{2,1}},{{1,0},{1,1},{2,1},{2,2}},{{0,2},{1,2},{1,1},{2,1}}},
	{{{1,0},{1,1},{1,2},{2,1}},{{0,1},{1,1},{2,1},{1,2}},{{1,0},{1,1},{1,2},{0,1}},{{1,0},{1,1},{2,1},{0,1}}},
	{{{1,0},{1,1},{1,2},{2,2}},{{0,1},{0,2},{1,1},{2,1}},{{1,0},{2,0},{2,1},{2,2}},{{2,1},{2,2},{1,2},{0,2}}},
	{{{1,1},{1,2},{2,1},{2,2}},{{1,1},{1,2},{2,1},{2,2}},{{1,1},{1,2},{2,1},{2,2}},{{1,1},{1,2},{2,1},{2,2}}},
	{{{2,0},{2,1},{2,2},{1,2}},{{1,0},{1,1},{2,1},{3,1}},{{1,0},{2,0},{1,1},{1,2}},{{1,0},{2,0},{3,0},{3,1}}}
};
//Ŀ�� �����
void CursorView(char show)
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}
//���� ����
void boardInit()
{
	for (int i = 0; i < WIDTH+2; i++)
	{
		for (int j = 0; j < HEIGHT+2; j++)
		{
			if (i == 0 || i == WIDTH + 1 || j == 0 || j == HEIGHT + 1)
				board[i][j] = brick[2];
			else
				board[i][j] = brick[0];
		}
	}
	
	return;
}
//��ǥ �̵�
void gotoxy(int x, int y)
{
	COORD posXY = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posXY);
}
//���� ����Ʈ
void printboard()
{
	for (int i = 0; i < WIDTH+2; i++)
	{
		for (int j = 0; j < HEIGHT+2; j++)
		{
			gotoxy(i*2, j);
			cout << board[i][j];
		}
		
	}
}
//�� ����Ʈ
void printblock(tetrimino t)
{
	for (int k = 0; k < 4; k++)
	{
		gotoxy((t.st.first + block[t.shape][t.dir][k].first)*2, t.st.second + block[t.shape][t.dir][k].second);
		cout << brick[1];
	}
}
//ȸ��, �̵� �������� üũ
bool can_move(tetrimino t)
{
	for (int k = 0; k < 4; k++)
	{
		if (board[t.st.first + block[t.shape][t.dir][k].first][t.st.second + block[t.shape][t.dir][k].second]
			!= "  " || t.st.first + block[t.shape][t.dir][k].first < 0 
			|| t.st.first + block[t.shape][t.dir][k].first > WIDTH+1 ||
			t.st.second + block[t.shape][t.dir][k].second < 0 ||
			t.st.second + block[t.shape][t.dir][k].second > HEIGHT+1) return false;
	}
	return true;
}
//�� ������ ������ �� �ִ��� üũ
bool is_done(tetrimino t)
{
	for (int k = 0; k < 4; k++)
	{
		if (board[t.st.first + block[t.shape][t.dir][k].first][t.st.second + block[t.shape][t.dir][k].second + 1]
			!= "  ") return true;
	}
	return false;
}
//������
void fix(tetrimino t)
{
	for (int k = 0; k < 4; k++)
	{
		board[t.st.first + block[t.shape][t.dir][k].first][t.st.second + block[t.shape][t.dir][k].second] = brick[1];
	}
}
//���� Ŭ����
void Lineclear()
{
	for (int j = HEIGHT; j >= 1; j--)
	{
		int cnt = 0;
		for (int i = 1; i <= WIDTH; i++)
		{
			if (board[i][j] != "  ") cnt++;
			else break;
		}
		if (cnt == WIDTH)
		{
			score += 100;
			//�ش� �� Ŭ����
			for (int i = 1; i <= WIDTH; i++)
			{
				board[i][j] = "  ";
			}
			
			//�ش� �� ���� ��ӽ�Ų��.
			for (int sj = j; sj >= 2; sj--)
			{
				for (int i = 1; i <= WIDTH; i++)
				{
					board[i][sj] = board[i][sj - 1];
				}
			}
			j++;
			
		}
	}
}
//���� ���
void printScore()
{
	for (int i = WIDTH + 11; i < WIDTH+12; i++)
	{
		for (int j = 3; j < 4; j++)
		{
			gotoxy(i * 2, j);
			cout << score;
		}
	}
	for (int i = WIDTH + 7; i <= WIDTH + 7; i++)
	{
		for (int j = 3; j < 4; j++)
		{
			gotoxy(i*2,j);
			cout << "����: ";
		}
	}
	for (int i = WIDTH + 6; i < WIDTH + 15; i++)
	{
		for (int j = 2; j < 5; j++)
		{
			if (i == WIDTH + 6 || i == WIDTH + 14 || j == 2 || j == 4)
			{
				gotoxy(i * 2, j);
				cout << brick[1];
			}
		}

	}
}
int main()
{
	srand((unsigned int)time(0));
	boardInit();
	CursorView(0);
	int nkey;
	tetrimino test;
	test.st = { 5,1 }; test.shape = 3; test.dir = 0;
	int time = 30;
	bool needtoMake = true; // �� ���� ������ �����
	bool gameover = false;
	while (1)
	{
		//�� ���� ���� �����, GAME OVER ����ϰ� �����Ѵ�.
		for (int i = 1; i < WIDTH + 1; i++)
		{
			if (board[i][1] != "  ")
			{
				gameover = true;
				break;
			}
		}
		if (gameover)
		{
			system("cls");
			gotoxy(10, 5);
			cout << "<���� ����>\n\n\n\n\n";
			
			break;
		}
		//������ ������ �Ǹ� �����.
		if (needtoMake)
		{
			test.st = { 5,1 };
			test.shape = rand() % 6;
			test.dir = 0;
			needtoMake = false;
		}
		//time������ ���� �ð��� ������ ������ �������� ��.
		time--;
		if (time == 0)
		{
			//���⼭, �ؿ� ���� �ְų�, �� ���̸� ���߰� ������
			if (is_done(test))
			{
				needtoMake = true;
				fix(test);
				Lineclear();
				time = 30;
				continue;
			}
			//�ƴϸ�, ��ĭ ������ ������ time �ʱ�ȭ
			else
			{
				test.st.second++;
				time = 30;
			}
		}
		printScore();
		printboard();
		printblock(test);
		//Ű �Է¿� ���� �� ����
		if (_kbhit())
		{
			nkey = _getch();
			tetrimino temp = test;
			switch (nkey)
			{
			case UP:
				test.dir++;
				if (test.dir == 4) test.dir = 0;
				break;
			case DOWN:
				test.st.second++;
				break;
			case LEFT:
				test.st.first -= 1;
				break;
			case RIGHT:
				test.st.first += 1;
				break;
			case SPACE:
				while (!is_done(test)) test.st.second++;
				break;
			}
			if (!can_move(test)) test = temp;
			Sleep(50);
			system("cls");
		}

	}
	return 0;
}