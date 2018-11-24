// ͷ�ļ�
// ����������ö�١��ṹ�塢�����ȵĶ���ͺ�������


// �������
#define SCRWIDTH 640	// ����̨���
#define SCRHEIGHT 480	// ����̨�߶�
#define WIDTH  10		// ��Ϸ������
#define HEIGHT 22		// ��Ϸ����߶�
#define UNIT   20		// ��Ϸ����ʵ������
#define PI 3.14
#define SMILE true
#define SAD false


// ������Ϸ����
enum CMD
{
	CMD_REVOLVE,	// ˳ʱ����ת
	CMD_LEFT,		// ����
	CMD_RIGHT,		// ����
	CMD_DOWN,		// ���¼���
	CMD_SINK,		// ֱ�ӵ���ײ�
	CMD_PAUSE,		// ��ͣ��Ϸ
	CMD_QUIT		// �˳���Ϸ
};


// ����ͼ�λ��Ʒ���
enum DRAW
{
	DRAW_SHOW,	// ����ͼ��
	DRAW_CLEAR,	// ���ͼ��
	DRAW_FIX	// �̶�ͼ��
};


// ���干������״�ķ��鼰�䲻ͬ״̬
struct BLOCK
{
	WORD ang[4];
	COLORREF color;
} Blocks[7] = { { 0x0F00, 0x4444, 0x0F00, 0x4444, YELLOW },				// ֱ���η���
				{ 0x0660, 0x0660, 0x0660, 0x0660, MAGENTA },			// �����η���
				{ 0x0740, 0x0622, 0x02E0, 0x4460, BLUE },				// L�η���
				{ 0x0470, 0x0644, 0x0E20, 0x2260, RED },				// ��L�η���
				{ 0x0C60, 0x2640, 0x0C60, 0x2640, BROWN },				// Z�η���
				{ 0x0360, 0x4620, 0x0360, 0x4620, GREEN },				// ��Z�η���
				{ 0x4E00, 0x4640, 0x0E40, 0x4C40, CYAN } };				// T�η���


// ���嵱ǰ���顢��һ����Ĳ�����Ϣ
struct BLOCKINFO
{
	byte type;				// ��һ�������״
	byte ang : 2;			// ��һ�������ת״̬		
	char x, y;				// ��һ���������	
} CurrentBlock, NextBlock;


// ��Ǹ����귽���Ƿ��Ѵ���
byte GameArea[WIDTH][HEIGHT] = { 0 };


// ���ü�ʱ���ж��Զ�����
DWORD oldtime = GetTickCount();


// �����Զ�������ʱ��
DWORD  delta_t;


// ����÷�
int	score;				// ������Ϸ�ߵĵ÷�
TCHAR	sScore[20];			// ����÷ֵ��ַ���


// ��������
void Init();													// ��ʼ������
//void Face(bool happy);											// ����Բ��
void NewGame();													// ��������Ϸ
CMD GetCmd();													// ��ȡ���̿�������
void TransCmd(CMD _cmd);										// ������ָ���Ϊ����
void NewBlock();												// �����·������
void DrawUnitBlock(int x, int y, COLORREF _color, DRAW _draw);	// ���Ƶ�λ������
void DrawBlock(BLOCKINFO _block, DRAW _draw = DRAW_SHOW);		// �����·���
bool Check(BLOCKINFO _block);									// ��鷽���Ƿ�������ڵ�ǰλ��
void Revolve();													// ����˳ʱ����ת
void Left();													// ��������
void Right();													// ��������
void Down();													// �������¼���
void Sink();													// ����ֱ�ӵ���ײ�
void Remove();													// ��������
void ScorePrint();												// ����÷�
void Esc();														// ��ESCʱ������ʾ��ȷ��
void Pause();													// ��ͣ��Ϸ
void GameOver();												// ��Ϸ����
void Quit();													// �˳���Ϸ