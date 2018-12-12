#include<graphics.h>
#include<conio.h>
#include<time.h>
#include<math.h>
#include"tetris.h"	// 游戏头文件


// 主函数
int main(void)
{
	Init();

	HWND wnd = GetHWnd();
	if (MessageBox(wnd, _T("现在开始游戏吗？"), _T("START GAME!"), MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		CMD Cmd;
		while (true)
		{
			Cmd = GetCmd();
			TransCmd(Cmd);
		}
	}
}


// 初始化界面
void Init()
{
	initgraph(SCRWIDTH, SCRHEIGHT);		// 初始化图形系统
	srand((unsigned)time(NULL));		// 使用系统时间生成随机数
	setbkmode(TRANSPARENT);				// 设置填充背景色为透明

	// 游戏标题
	settextstyle(50, 0, _T("微软雅黑"));
	outtextxy(50, 35, _T("Tetris"));

	// 显示得分
	roundrect(25, 120, 170, 200, 18, 18);

	settextstyle(30, 0, _T("微软雅黑"));
	outtextxy(70, 120, _T("score"));
	outtextxy(35, 140, _T("-------------"));

	// 显示操作说明
	roundrect(25, 210, 170, 440, 18, 18);

	settextstyle(20, 0, _T("微软雅黑"));
	outtextxy(70, 220, _T("操作说明"));
	outtextxy(30, 235, _T("-----------------------"));
	outtextxy(35, 260, _T("W：顺时针旋转"));
	outtextxy(35, 280, _T("A：向左移动"));
	outtextxy(35, 300, _T("S：加速下移"));
	outtextxy(35, 320, _T("D：向右移动"));
	outtextxy(35, 340, _T("空格：直接到达底部"));
	outtextxy(35, 380, _T("Q：暂停游戏"));
	outtextxy(35, 400, _T("ESC：退出游戏"));

	//Face(SAD);

	setorigin(200, 20);		// 设定坐标原点

	//	绘制游戏区域
	rectangle(-1, -1, WIDTH * UNIT, HEIGHT * UNIT);									// 该区域为主游戏区
	roundrect((WIDTH + 1) * UNIT - 1, -1, (WIDTH + 5) * UNIT, 4 * UNIT, 3, 3);		// 该区域提示下一方块

	NewGame();	// 创建新游戏
}


// 绘制圆脸
// 参数：
//		happy：圆脸表情
/*
void Face(bool happy)
{

// 设置圆脸的圆心
int x = (WIDTH - 2)*UNIT;
int y = (HEIGHT - 10) / 2 * UNIT;

//clearcircle(x, y, 24);
setcolor(BLACK);
fillcircle(x, y, 24);

// 脸
setcolor(YELLOW);
setfillcolor(0x00FFFF);
fillcircle(x, y, 24);

// 嘴
setcolor(RED);
if (happy)		// 笑脸
arc(x - 16, y - 16, x + 16, y + 16, (210 * PI / 180), (330 * PI / 180));
else			// 哭脸
arc(x - 16, y - 16 + 24, x + 16, y + 16 + 24, (30 * PI / 180), (150 * PI / 180));

// 眼睛
setcolor(BLUE);
setfillcolor(CYAN);
fillcircle(x - 6, y - 4, 2);
fillcircle(x + 6, y - 4, 2);

graphdefaults();	// 重置绘图设置
}
*/


// 创建新游戏
void NewGame()
{
	// 清空游戏区域
	setfillcolor(BLACK);										// 设置填充颜色为黑色
	solidrectangle(0, 0, WIDTH * UNIT - 1, HEIGHT * UNIT - 1);	// 以矩形填充游戏区域
	ZeroMemory(GameArea, WIDTH * HEIGHT);						// 清空该区域内存

																// 生成下一方块的参数
	NextBlock.type = rand() % 7;	// 随机确定下一方块的形状
	NextBlock.ang = rand() % 4;		// 随机确定下一方块的旋转状态
	NextBlock.x = WIDTH + 1;		//
	NextBlock.y = HEIGHT - 1;		// 将该参数的方块显示在右侧提示区域

	delta_t = 500;	// 设置自动下落间隔时间

					// 显示初始计分为0
	score -= 1000;
	ScorePrint();

	NewBlock();		// 生成新方块
}


// 获取键盘控制命令
CMD GetCmd()
{
	while (true)
	{
		// 规定时间内未进行有效操作，自动下落一步
		DWORD newtime = GetTickCount();
		if (newtime - oldtime >= delta_t)
		{
			oldtime = newtime;
			return CMD_DOWN;
		}

		// 若收到有效操作，返回对应命令
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


// 将键盘指令传递为函数
// 参数：
//		_cmd：键盘输入对应的操作
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


// 生成新方块参数
void NewBlock()
{
	CurrentBlock.type = NextBlock.type;		// 上一个新方块形状变为正下落方块形状
	NextBlock.type = rand() % 7;			// 随机生成新的下一方块形状
	CurrentBlock.ang = NextBlock.ang;		// 上一个新方块旋转状态变为正下落方块旋转状态
	NextBlock.type = rand() % 4;			// 随机生成新的下一方块旋转状态
	CurrentBlock.x = (WIDTH - 4) / 2;
	CurrentBlock.y = HEIGHT + 3;

	// 下移新方块至游戏区域的上边界
	WORD temp = Blocks[CurrentBlock.type].ang[CurrentBlock.ang];
	while ((temp & 0xF) == 0)
	{
		CurrentBlock.y--;
		temp >>= 4;
	}

	DrawBlock(CurrentBlock, DRAW_SHOW);	// 绘制新方块

										// 绘制下一个方块
	setfillcolor(BLACK);														// 
	solidrectangle((WIDTH + 1)* UNIT, 0, (WIDTH + 5)* UNIT - 1, 4 * UNIT - 1);	// 以黑色填充右侧提示区域
	DrawBlock(NextBlock, DRAW_SHOW);

	//oldtime = GetTickCount();													// 计时器判断自动下落
}


// 绘制单位正方形
// 参数：
//		x，y：单位正方形位置横纵坐标
//		_color：单位正方形颜色
//		_draw：单位正方形绘制方法
void DrawUnitBlock(int x, int y, COLORREF _color, DRAW _draw)
{
	// 计算四个边界的坐标
	int left = x * UNIT;
	int right = (x + 1) * UNIT - 1;
	int top = (HEIGHT - 1 - y) * UNIT;
	int bottom = (HEIGHT - y) * UNIT - 1;

	// 绘制单位正方形
	switch (_draw)
	{
		// 绘制单位正方形
	case DRAW_SHOW:
		setlinecolor(0x006060);
		roundrect(left + 1, top + 1, right - 1, bottom - 1, 5, 5);	// 单位正方形外第二层线框
		setlinecolor(0x003030);
		roundrect(left, top, right, bottom, 8, 8);					// 单位正方形外第一层线框
		setfillcolor(_color);
		setlinecolor(LIGHTGRAY);
		fillrectangle(left + 2, top + 2, right - 2, bottom - 2);	// 单位正方形
		break;

		// 清除单位正方形
	case DRAW_CLEAR:
		setfillcolor(BLACK);																				// 
		solidrectangle(x * UNIT, (HEIGHT - y - 1) * UNIT, (x + 1) * UNIT - 1, (HEIGHT - y) * UNIT - 1);		// 以黑色填充作为消除
		break;

		// 固定单位正方形
	case DRAW_FIX:
		setfillcolor(RGB(GetRValue(_color) * 0.7, GetGValue(_color) * 0.7, GetBValue(_color) * 0.7));	// 固定后颜色变暗为原先的70%亮度
		setlinecolor(DARKGRAY);
		fillrectangle(left + 1, top + 1, right - 1, bottom - 1);
		break;
	}
}


// 绘制新方块
// 参数：
//		_block：方块的参数信息
//		_draw：方块绘制方法
void DrawBlock(BLOCKINFO _block, DRAW _draw)
{
	WORD temp = Blocks[_block.type].ang[_block.ang];
	int x, y, i;

	// 对4 * 4矩阵逐个元素判断该点是否需要绘制单位正方形
	for (i = 0; i < 16; i++, temp <<= 1)			
		if (temp & 0x8000)
		{
			x = _block.x + i % 4;
			y = _block.y - i / 4;
			if (y < HEIGHT)
				DrawUnitBlock(x, y, Blocks[_block.type].color, _draw);
		}
}


// 检查方块是否可以落在当前位置
// 参数：
//		_block：方块的参数信息
bool Check(BLOCKINFO _block)
{
	WORD b = Blocks[_block.type].ang[_block.ang];
	int x, y, i;

	// 对4 * 4矩阵逐个元素判断该点是否需要绘制单位正方形
	for (i = 0; i < 16; i++, b <<= 1)
		if (b & 0x8000)
		{
			x = _block.x + i % 4;
			y = _block.y - i / 4;

			if (((x < 0) || (x >= WIDTH) || (y < 0)) || ((y < HEIGHT) && (GameArea[x][y])))		// 超出左右边界或该位置已有方块存在则不可落下
				return false;		// 不可落在当前位置 
		}
	return true;					// 可以落在当前位置
}


// 方块顺时针旋转
void Revolve()
{
	int delta_x;		// x偏移量
	BLOCKINFO temp = CurrentBlock;
	temp.ang++;

	// 判断旋转后是否超出边界，若超出则移动dx
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

// 顺时针旋转操作
revolve:		
	DrawBlock(CurrentBlock, DRAW_CLEAR);	// 清除原来的方块
	CurrentBlock.ang++;						// 方块转换为下一状态
	CurrentBlock.x += delta_x;				// 方块左右移动避免超出边界
	DrawBlock(CurrentBlock, DRAW_SHOW);		// 绘制旋转后的方块
}


// 方块左移
void Left()
{
	/*
	CurrentBlock.x--;
	if (Check(CurrentBlock))		// 检查左移一步的方块能否落在该位置
	{
	CurrentBlock.x++;
	DrawBlock(CurrentBlock, DRAW_CLEAR);	// 清除原位置的方块
	CurrentBlock.x--;						// 方块左移一步
	DrawBlock(CurrentBlock);				// 绘制移动后的方块
	}
	*/
	BLOCKINFO temp = CurrentBlock;
	temp.x--;
	if (Check(temp))								// 检查左移一步的方块能否落在该位置
	{
		DrawBlock(CurrentBlock, DRAW_CLEAR);		// 清除原位置的方块
		CurrentBlock.x--;							// 方块左移一步
		DrawBlock(CurrentBlock);					// 绘制移动后的方块
	}
}


// 方块右移
void Right()
{
	/*
	CurrentBlock.x++;
	if (Check(CurrentBlock))		// 检查右移一步的方块能否落在该位置
	{
	CurrentBlock.x--;
	DrawBlock(CurrentBlock, DRAW_CLEAR);	// 清除原位置的方块
	CurrentBlock.x++;						// 方块右移一步
	DrawBlock(CurrentBlock);				// 绘制移动后的方块
	}
	*/
	BLOCKINFO temp = CurrentBlock;
	temp.x++;
	if (Check(temp))								// 检查右移一步的方块能否落在该位置
	{
		DrawBlock(CurrentBlock, DRAW_CLEAR);		// 清除原位置的方块
		CurrentBlock.x++;							// 方块右移一步
		DrawBlock(CurrentBlock);					// 绘制移动后的方块
	}

}


// 方块向下加速
void Down()
{
	/*
	CurrentBlock.y--;
	if (Check(CurrentBlock))				// 检查下移一步的方块能否落在该位置
	{
	CurrentBlock.y++;
	DrawBlock(CurrentBlock, DRAW_CLEAR);	// 清除原位置的方块
	CurrentBlock.y--;						// 方块下移一步
	DrawBlock(CurrentBlock);				// 绘制移动后的方块
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
		Sink();									// 若方块已不可继续下移，使用Sink()函数固定
}


// 方块直接到达底部
void Sink()
{
	int x, y, i;

	/*
	// 连续重复进行Down()的下移功能
	CurrentBlock.y--;
	if (Check(CurrentBlock))				// 检查下移一步的方块能否落在该位置
	{
	CurrentBlock.y++;
	DrawBlock(CurrentBlock, DRAW_CLEAR);	// 清除原位置的方块
	CurrentBlock.y--;						// 方块下移一步
	DrawBlock(CurrentBlock);				// 绘制移动后的方块
	}
	DrawBlock(CurrentBlock, DRAW_FIX);			// 固定方块
	*/

	// 连续下移方块
	DrawBlock(CurrentBlock, DRAW_CLEAR);
	BLOCKINFO temp1 = CurrentBlock;
	temp1.y--;

	// 确认可下移时实现连续下移
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
			if (CurrentBlock.y - i / 4 >= HEIGHT)		// 方块叠加超过上边缘则游戏结束
			{
				GameOver();
				return;
			}
			else
				GameArea[CurrentBlock.x + i % 4][CurrentBlock.y - i / 4] = 1;	// 标记该位置有方块存在
		}

	Remove();
	
	NewBlock();		// 生成新方块
}
 
 
// 消除满行
void Remove()
{
	int x, y, i;
	byte remove = 0;	// remove标记是否需要消除

	for (y = CurrentBlock.y; y >= max(CurrentBlock.y - 4, 0); y--)
	{
		i = 0;

		for (x = 0; x < WIDTH; x++)
			if (GameArea[x][y] == 1)
				i++;

		if (i == WIDTH)		//该行填满   
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
				getimage(&img, 0, 0, WIDTH*UNIT, (HEIGHT - (CurrentBlock.y - row + 1))*UNIT);	// 保存未消除部分
				putimage(0, UNIT, &img);														// 输出移动后的未消除部分

				if (delta_t > 200)																// 每发生一次消除自动下落加快	
					delta_t -= 50;

				ScorePrint();

				//Face(SMILE);
			}
		}
	}
}


// 计算得分
void ScorePrint()
{
	// 设置得分区域参数
	RECT ScoreArea = { 0, 0, SCRWIDTH, 20 };
	ScoreArea.top = SCRHEIGHT - 330;
	ScoreArea.bottom = SCRHEIGHT + 120;
	ScoreArea.left = -850;

	// 擦去上次得分
	setcolor(BLACK);
	_stprintf(sScore, _T("%d"), score);
	drawtext(sScore, &ScoreArea, DT_CENTER | DT_SINGLELINE);

	score += 1000;

	// 显示得分
	setcolor(WHITE);
	_stprintf(sScore, _T("%d"), score);
	drawtext(sScore, &ScoreArea, DT_CENTER | DT_SINGLELINE);
}


// 按ESC时弹出提示框确认
void Esc()
{
	HWND wnd = GetHWnd();
	if (MessageBox(wnd, _T("确定退出游戏吗？"), _T("退出游戏"), MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
		Quit();
}


// 暂停游戏
void Pause()
{
	HWND wnd = GetHWnd();
	if (MessageBox(wnd, _T("游戏已暂停\n继续游戏吗？"), _T("PAUSE"), MB_YESNO | MB_ICONQUESTION) == IDYES);
	else
		Quit();
}


// 游戏结束
void GameOver()
{
	HWND wnd = GetHWnd();
	if (MessageBox(wnd, _T("Hey朋友,你挂啦！\n想再来一局吗？"), _T("GAME OVER!"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		NewGame();		// 按“确定”开始新一局游戏
	else
		Quit();			// 按“取消”退出游戏
}


// 退出游戏
void Quit()
{
	closegraph();
	exit(0);
}
