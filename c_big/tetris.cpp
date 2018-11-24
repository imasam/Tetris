// ��Ϸ���ƣ�����˹����
// ���뻷����Visual Studio 2015 + EasyX���ṩgraphics.hͷ�ļ���
// ���ߣ������ѧԺ 2015��2�� ���� 2014301550147
// Դ�ļ�


#include<graphics.h>
#include<conio.h>
#include<time.h>
#include<math.h>
#include"tetris.h"	// ��Ϸͷ�ļ�


// ������
int main(void)
{
	Init();

	HWND wnd = GetHWnd();
	if (MessageBox(wnd, _T("���ڿ�ʼ��Ϸ��"), _T("START GAME!"), MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		CMD Cmd;
		while (true)
		{
			Cmd = GetCmd();
			TransCmd(Cmd);
		}
	}
}


// ��ʼ������
void Init()
{
	initgraph(SCRWIDTH, SCRHEIGHT);		// ��ʼ��ͼ��ϵͳ
	srand((unsigned)time(NULL));		// ʹ��ϵͳʱ�����������
	setbkmode(TRANSPARENT);				// ������䱳��ɫΪ͸��

	// ��Ϸ����
	settextstyle(50, 0, _T("΢���ź�"));
	outtextxy(50, 35, _T("Tetris"));

	// ��ʾ�÷�
	roundrect(25, 120, 170, 200, 18, 18);

	settextstyle(30, 0, _T("΢���ź�"));
	outtextxy(70, 120, _T("score"));
	outtextxy(35, 140, _T("-------------"));

	// ��ʾ����˵��
	roundrect(25, 210, 170, 440, 18, 18);

	settextstyle(20, 0, _T("΢���ź�"));
	outtextxy(70, 220, _T("����˵��"));
	outtextxy(30, 235, _T("-----------------------"));
	outtextxy(35, 260, _T("W��˳ʱ����ת"));
	outtextxy(35, 280, _T("A�������ƶ�"));
	outtextxy(35, 300, _T("S����������"));
	outtextxy(35, 320, _T("D�������ƶ�"));
	outtextxy(35, 340, _T("�ո�ֱ�ӵ���ײ�"));
	outtextxy(35, 380, _T("Q����ͣ��Ϸ"));
	outtextxy(35, 400, _T("ESC���˳���Ϸ"));

	//Face(SAD);

	setorigin(200, 20);		// �趨����ԭ��

	//	������Ϸ����
	rectangle(-1, -1, WIDTH * UNIT, HEIGHT * UNIT);									// ������Ϊ����Ϸ��
	roundrect((WIDTH + 1) * UNIT - 1, -1, (WIDTH + 5) * UNIT, 4 * UNIT, 3, 3);		// ��������ʾ��һ����

	NewGame();	// ��������Ϸ
}


// ����Բ��
// ������
//		happy��Բ������
/*
void Face(bool happy)
{

// ����Բ����Բ��
int x = (WIDTH - 2)*UNIT;
int y = (HEIGHT - 10) / 2 * UNIT;

//clearcircle(x, y, 24);
setcolor(BLACK);
fillcircle(x, y, 24);

// ��
setcolor(YELLOW);
setfillcolor(0x00FFFF);
fillcircle(x, y, 24);

// ��
setcolor(RED);
if (happy)		// Ц��
arc(x - 16, y - 16, x + 16, y + 16, (210 * PI / 180), (330 * PI / 180));
else			// ����
arc(x - 16, y - 16 + 24, x + 16, y + 16 + 24, (30 * PI / 180), (150 * PI / 180));

// �۾�
setcolor(BLUE);
setfillcolor(CYAN);
fillcircle(x - 6, y - 4, 2);
fillcircle(x + 6, y - 4, 2);

graphdefaults();	// ���û�ͼ����
}
*/


// ��������Ϸ
void NewGame()
{
	// �����Ϸ����
	setfillcolor(BLACK);										// ���������ɫΪ��ɫ
	solidrectangle(0, 0, WIDTH * UNIT - 1, HEIGHT * UNIT - 1);	// �Ծ��������Ϸ����
	ZeroMemory(GameArea, WIDTH * HEIGHT);						// ��ո������ڴ�

																// ������һ����Ĳ���
	NextBlock.type = rand() % 7;	// ���ȷ����һ�������״
	NextBlock.ang = rand() % 4;		// ���ȷ����һ�������ת״̬
	NextBlock.x = WIDTH + 1;		//
	NextBlock.y = HEIGHT - 1;		// ���ò����ķ�����ʾ���Ҳ���ʾ����

	delta_t = 500;	// �����Զ�������ʱ��

					// ��ʾ��ʼ�Ʒ�Ϊ0
	score -= 1000;
	ScorePrint();

	NewBlock();		// �����·���
}


// ��ȡ���̿�������
CMD GetCmd()
{
	while (true)
	{
		// �涨ʱ����δ������Ч�������Զ�����һ��
		DWORD newtime = GetTickCount();
		if (newtime - oldtime >= delta_t)
		{
			oldtime = newtime;
			return CMD_DOWN;
		}

		// ���յ���Ч���������ض�Ӧ����
		if (_kbhit())
		{
			switch (_getch())
			{
			case 'w':
			case 'W':return CMD_REVOLVE;
			case 'a':
			case 'A':return CMD_LEFT;
			case 'd':
			case 'D':return CMD_RIGHT;
			case 's':
			case 'S':return CMD_DOWN;
			case ' ':return CMD_SINK;
			case 'q':
			case 'Q':return CMD_PAUSE;
			case 27:return CMD_QUIT;
			}
		}
	}
}


// ������ָ���Ϊ����
// ������
//		_cmd�����������Ӧ�Ĳ���
void TransCmd(CMD _cmd)
{
	switch (_cmd)
	{
	case CMD_REVOLVE:	Revolve();	break;
	case CMD_LEFT:		Left();		break;
	case CMD_RIGHT:		Right();	break;
	case CMD_DOWN:		Down();		break;
	case CMD_SINK:		Sink();		break;
	case CMD_PAUSE:		Pause();	break;
	case CMD_QUIT:		Esc();		break;
	}
}


// �����·������
void NewBlock()
{
	CurrentBlock.type = NextBlock.type;		// ��һ���·�����״��Ϊ�����䷽����״
	NextBlock.type = rand() % 7;			// ��������µ���һ������״
	CurrentBlock.ang = NextBlock.ang;		// ��һ���·�����ת״̬��Ϊ�����䷽����ת״̬
	NextBlock.type = rand() % 4;			// ��������µ���һ������ת״̬
	CurrentBlock.x = (WIDTH - 4) / 2;
	CurrentBlock.y = HEIGHT + 3;

	// �����·�������Ϸ������ϱ߽�
	WORD temp = Blocks[CurrentBlock.type].ang[CurrentBlock.ang];
	while ((temp & 0xF) == 0)
	{
		CurrentBlock.y--;
		temp >>= 4;
	}

	DrawBlock(CurrentBlock, DRAW_SHOW);	// �����·���

										// ������һ������
	setfillcolor(BLACK);														// 
	solidrectangle((WIDTH + 1)* UNIT, 0, (WIDTH + 5)* UNIT - 1, 4 * UNIT - 1);	// �Ժ�ɫ����Ҳ���ʾ����
	DrawBlock(NextBlock, DRAW_SHOW);

	//oldtime = GetTickCount();													// ��ʱ���ж��Զ�����
}


// ���Ƶ�λ������
// ������
//		x��y����λ������λ�ú�������
//		_color����λ��������ɫ
//		_draw����λ�����λ��Ʒ���
void DrawUnitBlock(int x, int y, COLORREF _color, DRAW _draw)
{
	// �����ĸ��߽������
	int left = x * UNIT;
	int right = (x + 1) * UNIT - 1;
	int top = (HEIGHT - 1 - y) * UNIT;
	int bottom = (HEIGHT - y) * UNIT - 1;

	// ���Ƶ�λ������
	switch (_draw)
	{
		// ���Ƶ�λ������
	case DRAW_SHOW:
		setlinecolor(0x006060);
		roundrect(left + 1, top + 1, right - 1, bottom - 1, 5, 5);	// ��λ��������ڶ����߿�
		setlinecolor(0x003030);
		roundrect(left, top, right, bottom, 8, 8);					// ��λ���������һ���߿�
		setfillcolor(_color);
		setlinecolor(LIGHTGRAY);
		fillrectangle(left + 2, top + 2, right - 2, bottom - 2);	// ��λ������
		break;

		// �����λ������
	case DRAW_CLEAR:
		setfillcolor(BLACK);																				// 
		solidrectangle(x * UNIT, (HEIGHT - y - 1) * UNIT, (x + 1) * UNIT - 1, (HEIGHT - y) * UNIT - 1);		// �Ժ�ɫ�����Ϊ����
		break;

		// �̶���λ������
	case DRAW_FIX:
		setfillcolor(RGB(GetRValue(_color) * 0.7, GetGValue(_color) * 0.7, GetBValue(_color) * 0.7));	// �̶�����ɫ�䰵Ϊԭ�ȵ�70%����
		setlinecolor(DARKGRAY);
		fillrectangle(left + 1, top + 1, right - 1, bottom - 1);
		break;
	}
}


// �����·���
// ������
//		_block������Ĳ�����Ϣ
//		_draw��������Ʒ���
void DrawBlock(BLOCKINFO _block, DRAW _draw)
{
	WORD temp = Blocks[_block.type].ang[_block.ang];
	int x, y, i;

	// ��4 * 4�������Ԫ���жϸõ��Ƿ���Ҫ���Ƶ�λ������
	for (i = 0; i < 16; i++, temp <<= 1)			
		if (temp & 0x8000)
		{
			x = _block.x + i % 4;
			y = _block.y - i / 4;
			if (y < HEIGHT)
				DrawUnitBlock(x, y, Blocks[_block.type].color, _draw);
		}
}


// ��鷽���Ƿ�������ڵ�ǰλ��
// ������
//		_block������Ĳ�����Ϣ
bool Check(BLOCKINFO _block)
{
	WORD b = Blocks[_block.type].ang[_block.ang];
	int x, y, i;

	// ��4 * 4�������Ԫ���жϸõ��Ƿ���Ҫ���Ƶ�λ������
	for (i = 0; i < 16; i++, b <<= 1)
		if (b & 0x8000)
		{
			x = _block.x + i % 4;
			y = _block.y - i / 4;

			if (((x < 0) || (x >= WIDTH) || (y < 0)) || ((y < HEIGHT) && (GameArea[x][y])))		// �������ұ߽���λ�����з�������򲻿�����
				return false;		// �������ڵ�ǰλ�� 
		}
	return true;					// �������ڵ�ǰλ��
}


// ����˳ʱ����ת
void Revolve()
{
	int delta_x;		// xƫ����
	BLOCKINFO temp = CurrentBlock;
	temp.ang++;

	// �ж���ת���Ƿ񳬳��߽磬���������ƶ�dx
	if (Check(temp))
	{
		delta_x = 0;
		goto revolve;
	}

	temp.x = CurrentBlock.x - 1;
	if (Check(temp))
	{
		delta_x = -1;
		goto revolve;
	}

	temp.x = CurrentBlock.x + 1;
	if (Check(temp))
	{
		delta_x = 1;
		goto revolve;
	}

	temp.x = CurrentBlock.x - 2;
	if (Check(temp))
	{
		delta_x = -2;
		goto revolve;
	}

	temp.x = CurrentBlock.x + 2;
	if (Check(temp))
	{
		delta_x = 2;
		goto revolve;
	}
	return;

// ˳ʱ����ת����
revolve:		
	DrawBlock(CurrentBlock, DRAW_CLEAR);	// ���ԭ���ķ���
	CurrentBlock.ang++;						// ����ת��Ϊ��һ״̬
	CurrentBlock.x += delta_x;				// ���������ƶ����ⳬ���߽�
	DrawBlock(CurrentBlock, DRAW_SHOW);		// ������ת��ķ���
}


// ��������
void Left()
{
	/*
	CurrentBlock.x--;
	if (Check(CurrentBlock))		// �������һ���ķ����ܷ����ڸ�λ��
	{
	CurrentBlock.x++;
	DrawBlock(CurrentBlock, DRAW_CLEAR);	// ���ԭλ�õķ���
	CurrentBlock.x--;						// ��������һ��
	DrawBlock(CurrentBlock);				// �����ƶ���ķ���
	}
	*/
	BLOCKINFO temp = CurrentBlock;
	temp.x--;
	if (Check(temp))								// �������һ���ķ����ܷ����ڸ�λ��
	{
		DrawBlock(CurrentBlock, DRAW_CLEAR);		// ���ԭλ�õķ���
		CurrentBlock.x--;							// ��������һ��
		DrawBlock(CurrentBlock);					// �����ƶ���ķ���
	}
}


// ��������
void Right()
{
	/*
	CurrentBlock.x++;
	if (Check(CurrentBlock))		// �������һ���ķ����ܷ����ڸ�λ��
	{
	CurrentBlock.x--;
	DrawBlock(CurrentBlock, DRAW_CLEAR);	// ���ԭλ�õķ���
	CurrentBlock.x++;						// ��������һ��
	DrawBlock(CurrentBlock);				// �����ƶ���ķ���
	}
	*/
	BLOCKINFO temp = CurrentBlock;
	temp.x++;
	if (Check(temp))								// �������һ���ķ����ܷ����ڸ�λ��
	{
		DrawBlock(CurrentBlock, DRAW_CLEAR);		// ���ԭλ�õķ���
		CurrentBlock.x++;							// ��������һ��
		DrawBlock(CurrentBlock);					// �����ƶ���ķ���
	}

}


// �������¼���
void Down()
{
	/*
	CurrentBlock.y--;
	if (Check(CurrentBlock))				// �������һ���ķ����ܷ����ڸ�λ��
	{
	CurrentBlock.y++;
	DrawBlock(CurrentBlock, DRAW_CLEAR);	// ���ԭλ�õķ���
	CurrentBlock.y--;						// ��������һ��
	DrawBlock(CurrentBlock);				// �����ƶ���ķ���
	}
	*/
	BLOCKINFO temp = CurrentBlock;
	temp.y--;
	if (Check(temp))
	{
		DrawBlock(CurrentBlock, DRAW_CLEAR);
		CurrentBlock.y--;
		DrawBlock(CurrentBlock, DRAW_SHOW);
	}
	else
		Sink();									// �������Ѳ��ɼ������ƣ�ʹ��Sink()�����̶�
}


// ����ֱ�ӵ���ײ�
void Sink()
{
	int x, y, i;

	/*
	// �����ظ�����Down()�����ƹ���
	CurrentBlock.y--;
	if (Check(CurrentBlock))				// �������һ���ķ����ܷ����ڸ�λ��
	{
	CurrentBlock.y++;
	DrawBlock(CurrentBlock, DRAW_CLEAR);	// ���ԭλ�õķ���
	CurrentBlock.y--;						// ��������һ��
	DrawBlock(CurrentBlock);				// �����ƶ���ķ���
	}
	DrawBlock(CurrentBlock, DRAW_FIX);			// �̶�����
	*/

	// �������Ʒ���
	DrawBlock(CurrentBlock, DRAW_CLEAR);
	BLOCKINFO temp1 = CurrentBlock;
	temp1.y--;

	// ȷ�Ͽ�����ʱʵ����������
	while (Check(temp1))
	{
		CurrentBlock.y--;
		temp1.y--;
	}
	DrawBlock(CurrentBlock, DRAW_FIX);

	WORD temp2 = Blocks[CurrentBlock.type].ang[CurrentBlock.ang];
	for (i = 0; i < 16; i++, temp2 <<= 1)
		if (temp2 & 0x8000)
		{
			if (CurrentBlock.y - i / 4 >= HEIGHT)		// ������ӳ����ϱ�Ե����Ϸ����
			{
				GameOver();
				return;
			}
			else
				GameArea[CurrentBlock.x + i % 4][CurrentBlock.y - i / 4] = 1;	// ��Ǹ�λ���з������
		}

	Remove();
	
	NewBlock();		// �����·���
}
 
 
// ��������
void Remove()
{
	int x, y, i;
	byte remove = 0;	// remove����Ƿ���Ҫ����

	for (y = CurrentBlock.y; y >= max(CurrentBlock.y - 4, 0); y--)
	{
		i = 0;

		for (x = 0; x < WIDTH; x++)
			if (GameArea[x][y] == 1)
				i++;

		if (i == WIDTH)		//��������   
			remove |= (1 << (CurrentBlock.y - y));
	}

	if (remove)
	{
		IMAGE img;

		for (int row = 0; row < 4; row++, remove >>= 1)
		{
			if (remove & 1)
			{
				for (y = CurrentBlock.y - row + 1; y < HEIGHT; y++)
					for (x = 0; x < WIDTH; x++)
					{
						GameArea[x][y - 1] = GameArea[x][y];
						GameArea[x][y] = 0;
					}
				getimage(&img, 0, 0, WIDTH*UNIT, (HEIGHT - (CurrentBlock.y - row + 1))*UNIT);	// ����δ��������
				putimage(0, UNIT, &img);														// ����ƶ����δ��������

				if (delta_t > 200)																// ÿ����һ�������Զ�����ӿ�	
					delta_t -= 50;

				ScorePrint();

				//Face(SMILE);
			}
		}
	}
}


// ����÷�
void ScorePrint()
{
	// ���õ÷��������
	RECT ScoreArea = { 0, 0, SCRWIDTH, 20 };
	ScoreArea.top = SCRHEIGHT - 330;
	ScoreArea.bottom = SCRHEIGHT + 120;
	ScoreArea.left = -850;

	// ��ȥ�ϴε÷�
	setcolor(BLACK);
	_stprintf(sScore, _T("%d"), score);
	drawtext(sScore, &ScoreArea, DT_CENTER | DT_SINGLELINE);

	score += 1000;

	// ��ʾ�÷�
	setcolor(WHITE);
	_stprintf(sScore, _T("%d"), score);
	drawtext(sScore, &ScoreArea, DT_CENTER | DT_SINGLELINE);
}


// ��ESCʱ������ʾ��ȷ��
void Esc()
{
	HWND wnd = GetHWnd();
	if (MessageBox(wnd, _T("ȷ���˳���Ϸ��"), _T("�˳���Ϸ"), MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
		Quit();
}


// ��ͣ��Ϸ
void Pause()
{
	HWND wnd = GetHWnd();
	if (MessageBox(wnd, _T("��Ϸ����ͣ\n������Ϸ��"), _T("PAUSE"), MB_YESNO | MB_ICONQUESTION) == IDYES);
	else
		Quit();
}


// ��Ϸ����
void GameOver()
{
	HWND wnd = GetHWnd();
	if (MessageBox(wnd, _T("Hey����,�������\n������һ����"), _T("GAME OVER!"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		NewGame();		// ����ȷ������ʼ��һ����Ϸ
	else
		Quit();			// ����ȡ�����˳���Ϸ
}


// �˳���Ϸ
void Quit()
{
	closegraph();
	exit(0);
}
