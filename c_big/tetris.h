// 头文件
// 包含常量、枚举、结构体、变量等的定义和函数声明


// 定义参数
#define SCRWIDTH 640	// 控制台宽度
#define SCRHEIGHT 480	// 控制台高度
#define WIDTH  10		// 游戏区域宽度
#define HEIGHT 22		// 游戏区域高度
#define UNIT   20		// 游戏区域实际像素
#define PI 3.14
#define SMILE true
#define SAD false


// 定义游戏操作
enum CMD
{
	CMD_REVOLVE,	// 顺时针旋转
	CMD_LEFT,		// 左移
	CMD_RIGHT,		// 右移
	CMD_DOWN,		// 向下加速
	CMD_SINK,		// 直接到达底部
	CMD_PAUSE,		// 暂停游戏
	CMD_QUIT		// 退出游戏
};


// 定义图形绘制方法
enum DRAW
{
	DRAW_SHOW,	// 绘制图形
	DRAW_CLEAR,	// 清除图形
	DRAW_FIX	// 固定图形
};


// 定义共七种形状的方块及其不同状态
struct BLOCK
{
	WORD ang[4];
	COLORREF color;
} Blocks[7] = { { 0x0F00, 0x4444, 0x0F00, 0x4444, YELLOW },				// 直线形方快
				{ 0x0660, 0x0660, 0x0660, 0x0660, MAGENTA },			// 正方形方块
				{ 0x0740, 0x0622, 0x02E0, 0x4460, BLUE },				// L形方块
				{ 0x0470, 0x0644, 0x0E20, 0x2260, RED },				// 反L形方块
				{ 0x0C60, 0x2640, 0x0C60, 0x2640, BROWN },				// Z形方块
				{ 0x0360, 0x4620, 0x0360, 0x4620, GREEN },				// 反Z形方块
				{ 0x4E00, 0x4640, 0x0E40, 0x4C40, CYAN } };				// T形方块


// 定义当前方块、下一方块的参数信息
struct BLOCKINFO
{
	byte type;				// 下一方块的形状
	byte ang : 2;			// 下一方块的旋转状态		
	char x, y;				// 下一方块的坐标	
} CurrentBlock, NextBlock;


// 标记该坐标方块是否已存在
byte GameArea[WIDTH][HEIGHT] = { 0 };


// 设置计时器判断自动下落
DWORD oldtime = GetTickCount();


// 定义自动下落间隔时间
DWORD  delta_t;


// 定义得分
int	score;				// 两个游戏者的得分
TCHAR	sScore[20];			// 保存得分的字符串


// 函数声明
void Init();													// 初始化界面
//void Face(bool happy);											// 绘制圆脸
void NewGame();													// 创建新游戏
CMD GetCmd();													// 获取键盘控制命令
void TransCmd(CMD _cmd);										// 将键盘指令传递为函数
void NewBlock();												// 生成新方块参数
void DrawUnitBlock(int x, int y, COLORREF _color, DRAW _draw);	// 绘制单位正方形
void DrawBlock(BLOCKINFO _block, DRAW _draw = DRAW_SHOW);		// 绘制新方块
bool Check(BLOCKINFO _block);									// 检查方块是否可以落在当前位置
void Revolve();													// 方块顺时针旋转
void Left();													// 方块左移
void Right();													// 方块右移
void Down();													// 方块向下加速
void Sink();													// 方块直接到达底部
void Remove();													// 消除满行
void ScorePrint();												// 计算得分
void Esc();														// 按ESC时弹出提示框确认
void Pause();													// 暂停游戏
void GameOver();												// 游戏结束
void Quit();													// 退出游戏