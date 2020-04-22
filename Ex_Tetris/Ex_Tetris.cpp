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

char* brick[3] = { "  ", "□", "■" };
char* board[WIDTH+2][HEIGHT+2];
int score = 0;

struct tetrimino
{
	pair<int, int> st; // 기준 x,y좌표
	int shape; // 모양 0~5
	int dir; // 방향
};
//테트리스 블럭 좌표 i : 모양 j : 방향 k : 각 블럭의 좌표
pair<int, int> block[6][4][4] =
{
	{{{1,0},{1,1},{1,2},{1,3}},{{0,1},{1,1},{2,1},{3,1}},{{2,0},{2,1},{2,2},{2,3}},{{0,2},{1,2},{2,2},{3,2}}},
	{{{1,0},{1,1},{2,1},{2,2}},{{0,2},{1,2},{1,1},{2,1}},{{1,0},{1,1},{2,1},{2,2}},{{0,2},{1,2},{1,1},{2,1}}},
	{{{1,0},{1,1},{1,2},{2,1}},{{0,1},{1,1},{2,1},{1,2}},{{1,0},{1,1},{1,2},{0,1}},{{1,0},{1,1},{2,1},{0,1}}},
	{{{1,0},{1,1},{1,2},{2,2}},{{0,1},{0,2},{1,1},{2,1}},{{1,0},{2,0},{2,1},{2,2}},{{2,1},{2,2},{1,2},{0,2}}},
	{{{1,1},{1,2},{2,1},{2,2}},{{1,1},{1,2},{2,1},{2,2}},{{1,1},{1,2},{2,1},{2,2}},{{1,1},{1,2},{2,1},{2,2}}},
	{{{2,0},{2,1},{2,2},{1,2}},{{1,0},{1,1},{2,1},{3,1}},{{1,0},{2,0},{1,1},{1,2}},{{1,0},{2,0},{3,0},{3,1}}}
};
//커서 숨기기
void CursorView(char show)
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}
//보드 구성
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
//좌표 이동
void gotoxy(int x, int y)
{
	COORD posXY = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posXY);
}
//보드 프린트
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
//블럭 프린트
void printblock(tetrimino t)
{
	for (int k = 0; k < 4; k++)
	{
		gotoxy((t.st.first + block[t.shape][t.dir][k].first)*2, t.st.second + block[t.shape][t.dir][k].second);
		cout << brick[1];
	}
}
//회전, 이동 가능한지 체크
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
//더 밑으로 내려갈 수 있는지 체크
bool is_done(tetrimino t)
{
	for (int k = 0; k < 4; k++)
	{
		if (board[t.st.first + block[t.shape][t.dir][k].first][t.st.second + block[t.shape][t.dir][k].second + 1]
			!= "  ") return true;
	}
	return false;
}
//굳히기
void fix(tetrimino t)
{
	for (int k = 0; k < 4; k++)
	{
		board[t.st.first + block[t.shape][t.dir][k].first][t.st.second + block[t.shape][t.dir][k].second] = brick[1];
	}
}
//라인 클리어
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
			//해당 줄 클리어
			for (int i = 1; i <= WIDTH; i++)
			{
				board[i][j] = "  ";
			}
			
			//해당 줄 위를 드롭시킨다.
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
//점수 출력
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
			cout << "점수: ";
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
	bool needtoMake = true; // 새 블럭을 만들어야 대는지
	bool gameover = false;
	while (1)
	{
		//맨 위에 블럭이 생기면, GAME OVER 출력하고 종료한다.
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
			cout << "<게임 오버>\n\n\n\n\n";
			
			break;
		}
		//새블럭을 만들어야 되면 만든다.
		if (needtoMake)
		{
			test.st = { 5,1 };
			test.shape = rand() % 6;
			test.dir = 0;
			needtoMake = false;
		}
		//time변수로 일정 시간이 지나면 밑으로 내려가게 함.
		time--;
		if (time == 0)
		{
			//여기서, 밑에 블럭이 있거나, 맨 끝이면 멈추고 굳히기
			if (is_done(test))
			{
				needtoMake = true;
				fix(test);
				Lineclear();
				time = 30;
				continue;
			}
			//아니면, 한칸 밑으로 내리고 time 초기화
			else
			{
				test.st.second++;
				time = 30;
			}
		}
		printScore();
		printboard();
		printblock(test);
		//키 입력에 따른 블럭 조작
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