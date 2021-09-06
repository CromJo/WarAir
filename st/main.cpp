//main.cpp

#define WIN32_LEAN_AND_MEAN	// �����Ϸ����� �ܺ� mfc ��������� �������� ������ �����Ѵ�

#include <windows.h>	// �������� ���� ��� ����
#include <windowsx.h>

#include <ddraw.h>		// ���̷�Ʈ ��ο� ���� ��� ����
#include <dinput.h>		// ���̷�Ʈ ��ǲ ���� �������

#include "bmp.h"		// bmp���� ��� ����
#include "timer.h"		// �ð� ���� ��� ����
#include "enemy.h"		// �� ���� ��� ����
#include "sprite.h"		// ��������Ʈ �׸��� ���� �������
#include "myship.h"		// �÷��̾� ���� ��� ����
#include "mymissile.h"	//  �� �̻��� ���� ��� ����
#include "exploding.h"		// ���� ���� ��� ����
#include "enemymissile.h"	// �� �̻��� ���� �������
#include "boss.h"		// �������� �������
#include "bossmissile.h"	// ���� �̻��� ���� �������

#include "define.h"	// ���������
// �Լ� ���� ���� //
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);	// ������ ����
bool InitializeDirectX(void);		// ���̷�Ʈ x �ʱ�ȭ 
void GameMain(void);		// ���� ����
void InitGame();		// ���� �ʱ�ȭ
bool LoadBMPandInitSurface();	// ��Ʈ�� �ҷ����� �ʱ�ȭ ���ִ� �Լ�
bool InitObject();	// ������Ʈ �ʱ�ȭ

// ���� ���� ���� //
// ��� ���� ���� �տ��� g_�� �տ� �ٿ��� ����Ͽ���.
HWND			g_hwnd;		// ������ �ڵ�
HINSTANCE		g_hInstance;		// �ν��Ͻ� �ڵ�

LPDIRECTDRAW7				g_lpDirectDrawObject = NULL;	// ���̷�Ʈ ��ο� ��ü ����
LPDIRECTDRAWSURFACE7		g_lpPrimarySurface = NULL;		// 1�� ǥ��	����
LPDIRECTDRAWSURFACE7		g_lpSecondarySurface = NULL;	// 2�� ǥ�� ����

LPDIRECTINPUT8				g_lpDirectInputObject = NULL;		// �Է¹��� ��
LPDIRECTINPUTDEVICE8		g_lpDirectInputKeyboard = NULL;		// �Է� �޴� Ű���尪

CSprite					g_TitleSprite;			//�ε�ȭ�� ��������Ʈ
CSprite					g_BackgroundSprite;		// ��� ��������Ʈ
CSprite					g_EnemySprite;			// �� ��������Ʈ
CSprite					g_EnemyMissileSprite;	// �� �̻��� ��������Ʈ
CSprite					g_MyShipSprite;		// �� ����� ��������Ʈ
CSprite					g_MyMissileSprite;		// �� �̻��� ��������Ʈ
CSprite					g_ExplodingSprite;		// ���� ��������Ʈ
CSprite					g_BossSprite;		// ���� ��������Ʈ 
CSprite					g_BossMissileSprite;	// �����̻��� ��������Ʈ
CSprite					g_ClearSprite;	// ���� ���� ��������Ʈ
CSprite					g_DeadSprite;	// ���� ���� ��������Ʈ 

CTimer					g_Timer;		// �ð� Ŭ���� ��ü ����

bool					g_bActiveApp = false;	// ������ ��Ƽ�� �Ѱ�?

CEnemy					g_Enemy[MAX_XENEMYS][MAX_YENEMYS];		// �� ���� ��ü ����
CEnemyMissile			g_EnemyMissile[MAX_XENEMYS][MAX_YENEMYS];	// �� �̻��� ���� ��ü ����
CMyShip					g_MyShip;	// �� ����� ���� ��ü ����
CMyMissile				g_MyMissile[MAX_MISSILES];	// �� �̻��� ���� ��ü ����
CBoss					g_Boss[MAX_BOSS];	// �������� ��ü ����
CBossMissile			g_BossMissile[MAX_BOSS_YMISSILES][MAX_BOSS_XMISSILES];	// ���� �̻��ϰ��� ��ü ����
CExploding				g_Exploding[MAX_EXPLODES];	// ���� ���� ��ü ����

extern BOOL _InitDirectSound();	// ���̷�Ʈ ���� �ʱ�ȭ ���� �޾ƿ´�.
// ���� ���� ���� �� //

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//WindowClass ���
	WNDCLASSEX wndclass;	// ������ Ŭ���� ��ü ����
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;	// ��Ÿ��
	wndclass.lpfnWndProc = WinProc;	// ������ ����
	wndclass.cbClsExtra = wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;	// �ν��Ͻ� �ڵ鷯
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));  //������ ���
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);	// Ŀ�� ���
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	// ��� �� 
	wndclass.lpszMenuName = NULL;	// �޴� 
	wndclass.lpszClassName = CLASS_NAME;	// Ŭ���� �̸�
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (RegisterClassEx(&wndclass) == 0)
		return 0;
	// window�� creation  // �켱����!! ȭ����ȯ�� �Ұ��� ������.
	g_hwnd = CreateWindowEx(WS_EX_TOPMOST, CLASS_NAME, CLASS_NAME, WS_POPUP | WS_VISIBLE,
		0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),	// ������� x,y ũ�⸦ ���ؿ´�.
		NULL, NULL, hInstance, NULL);

	if (g_hwnd == NULL) return 0;	// g_hwnd�� MessageBox() ȣ�� �� �ʿ��ϴ�.
	g_hInstance = hInstance;		// DirectInput�ʱ�ȭ�� �ʿ��ϴ�

	SetFocus(g_hwnd);		// ������ ��Ŀ���� �����´�.
	ShowCursor(FALSE);		// false�� ���콺 �����͸� �����.

	// DirectDraw �ʱ�ȭ
	if (!InitializeDirectX())	//���̷�Ʈ x�� �ʱ�ȭ 
		return 0;		// �ʱ�ȭ ���� �������� 0�� �����Ѵ�.

	if (!_InitDirectSound())	// ���̷�Ʈ ���带 �ʱ�ȭ
		return 0;	// �ֱ�ȭ ���� �������� 0�� ����

	g_Timer.start();	// Ÿ�̸� ����

	if (!LoadBMPandInitSurface())	// ��Ʈ�� ������ �ҷ����� �ʱ�ȭ�Ѵ�.
		return 0;

	if (!InitObject())	// ������Ʈ �ʱ�ȭ..?  true�� ��ȯ
		return 0;

	//Message loop
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))	//�޽����� ���翩�θ� Ȯ����
		{
			if (!GetMessage(&msg, NULL, 0, 0))	// �޽����� ���´�. �������ϸ�
				return (int)msg.wParam;	// Ű���� �޽������� ���������� ��ȯ�Ͽ� ������.
			TranslateMessage(&msg);		// Ű������� �޽���
			DispatchMessage(&msg);		// Window procdure ȣ��
		}
		else if (g_bActiveApp)	// ������ ���࿩�θ� �Ǵ��Ͽ�
			GameMain();	// ���� ������ ȣ��
		else
			WaitMessage();	// �޽����� ���ö����� ��ٸ���.
	}
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)		// ������ ����
{
	switch (message)
	{

	case WM_ACTIVATEAPP:		// ���α׷��� Ȱ��, ��Ȱ�� ���θ� ���´�.
		if (wParam)
			g_bActiveApp = true;	// Ȱ�����̸� Ʈ��
		else
			g_bActiveApp = false;	// �ƴϸ� �޽�
		break;

	case WM_KEYDOWN:		// Ű�� ��������
		if (wParam == 'Y') InitGame();	// y��� ���� �ʱ�ȭ
		if (wParam == 'N') DestroyWindow(hwnd);	//n�̶�� ���� ����
		if (wParam == VK_ESCAPE) DestroyWindow(hwnd);	// esc��� ���� ����
		break;

	case WM_DESTROY:		// ���α׷� �����
		if (g_lpDirectDrawObject)
		{
			if (g_lpSecondarySurface)		// 2������ �Ҹ�
				g_lpSecondarySurface->Release();
			if (g_lpPrimarySurface)				// 1�� ���� �Ҹ�
				g_lpPrimarySurface->Release();
			g_DeadSprite.ReleaseAll();		// ���� ���� ��������Ʈ ���� ��� Surface�� �޸� ����
			g_ClearSprite.ReleaseAll();		// ���� ���� ��������Ʈ ���� ��� Surface�� �޸� ���� 
			g_BossMissileSprite.ReleaseAll();	// ���� �̻��� ��������Ʈ ���� ��� Surface�� �޸� ���� 
			g_BossSprite.ReleaseAll();	// ���� ��������Ʈ ���� ��� Surface�� �޸� ���� 
			g_EnemyMissileSprite.ReleaseAll();	// ���̻��� ��������Ʈ ���� ��� Surface�� �޸� ���� 
			g_EnemySprite.ReleaseAll();	// �� ��������Ʈ ���� ��� Surface�� �޸� ���� 
			g_BackgroundSprite.ReleaseAll();	// ��� ��������Ʈ ���� ��� Surface�� �޸� ���� 
			g_TitleSprite.ReleaseAll();		// Ÿ��Ʋ(�ε�ȭ��) ��������Ʈ ���� ��� Surface�� �޸� ���� 

			g_lpDirectDrawObject->Release();	// 
		}
		if (g_lpDirectInputObject)
		{
			if (g_lpDirectInputKeyboard)
			{
				g_lpDirectInputKeyboard->Unacquire();  // ���� ������ �ش�
				g_lpDirectInputKeyboard->Release();	// Ű���尪 �Ҹ�
			}
			g_lpDirectInputObject->Release();	// �Է°� �Ҹ�
		}

		ShowCursor(TRUE);	//Ŀ���� ���̰� ����
		PostQuitMessage(0);	// ����
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
		// ���࿡ ������ ó���� ���� ���ߴٸ� �⺻�� ����
	}
	return 0;
}
bool LoadBMPandInitSurface() // ��Ʈ�� �ҷ����� �Լ�
{
	//Ÿ��Ʋ ȭ�� ���
	if (!g_TitleSprite.InitSprite(1, 800, 600, COLOR_KEY, g_lpDirectDrawObject))	//Ÿ��Ʋ ȭ�� �ʱ�ȭ
		return false;
	if (!g_TitleSprite.LoadFrame(0, "data\\title.bmp"))	// Ÿ��Ʋȭ�� ���� ������ �׳� �Ѿ�� ���н� ���� �޽�
		return false;
	g_TitleSprite.Drawing(0, 400, 300, g_lpSecondarySurface, false);	// Ÿ��Ʋ ȭ���� �׸���.

	//Flipping
	HRESULT hResult;
	if (FAILED(hResult = g_lpPrimarySurface->Flip(NULL, DDFLIP_WAIT)))	// �ø��� ó��
	{
		if (hResult == DDERR_SURFACELOST)	// �ø����� �����ؼ� Restore()�� �������ش�.
			g_lpPrimarySurface->Restore();
	}
	//��׶��� ��������Ʈ
	if (!g_BackgroundSprite.InitSprite(1, 800, 1200, COLOR_KEY, g_lpDirectDrawObject))	// 1���� �������� 800*1200 ũ���� ��潺������Ʈ�� �ʱ�ȭ
		return false;
	if (!g_BackgroundSprite.LoadFrame(0, "data\\background.bmp"))	// 0��° �����ӿ� �ش� ���ϰ�ο��� �ҷ��´�.
		return false;

	//�� ��������Ʈ					// ���� ��������Ʈ�� ũ�� ��ŭ �ʱ�ȭ ����
	if (!g_EnemySprite.InitSprite(10, 100, 75, COLOR_KEY, g_lpDirectDrawObject))
		return false;				// �� �����ӿ� �� ������ �ҷ��´�. �ҷ����� �����ϸ� ���� �޽�
	if (!g_EnemySprite.LoadFrame(0, "data\\enemy_motion0000.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(1, "data\\enemy_motion0001.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(2, "data\\enemy_motion0002.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(3, "data\\enemy_motion0003.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(4, "data\\enemy_motion0004.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(5, "data\\enemy_motion0005.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(6, "data\\enemy_motion0006.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(7, "data\\enemy_motion0007.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(8, "data\\enemy_motion0008.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(9, "data\\enemy_motion0009.bmp"))
		return false;

	//�� ���༱ ��������Ʈ			//�� ������� ��������Ʈ �ʱ�ȭ
	if (!g_MyShipSprite.InitSprite(3, 90, 82, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	// �׳� ���� ����, ������ �̹�����  �ش� �����ӿ� �ҷ��´�.
	if (!g_MyShipSprite.LoadFrame(0, "data\\myship_center.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(1, "data\\myship_left.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(2, "data\\myship_right.bmp"))
		return false;

	//�� �̻��� ��������Ʈ		// �� �̻����� �ʱ�ȭ
	if (!g_MyMissileSprite.InitSprite(8, 28, 50, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	// �̻����� ������ ���ӵ� �̹����� ������ �����ӿ� �ҷ��´�.
	if (!g_MyMissileSprite.LoadFrame(0, "data\\My_missile00.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(1, "data\\My_missile01.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(2, "data\\My_missile02.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(3, "data\\My_missile03.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(4, "data\\My_missile04.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(5, "data\\My_missile05.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(6, "data\\My_missile06.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(7, "data\\My_missile07.bmp"))
		return false;

	//������� ��������Ʈ		// ������� ��������Ʈ �ʱ�ȭ
	if (!g_ExplodingSprite.InitSprite(20, 160, 120, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	// ��������� �̹������� ������ �����ӿ� �ҷ��´�.
	if (!g_ExplodingSprite.LoadFrame(0, "data\\SimpleExplosion001.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(1, "data\\SimpleExplosion002.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(2, "data\\SimpleExplosion003.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(3, "data\\SimpleExplosion004.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(4, "data\\SimpleExplosion005.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(5, "data\\SimpleExplosion006.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(6, "data\\SimpleExplosion007.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(7, "data\\SimpleExplosion008.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(8, "data\\SimpleExplosion009.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(9, "data\\SimpleExplosion010.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(10, "data\\SimpleExplosion011.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(11, "data\\SimpleExplosion012.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(12, "data\\SimpleExplosion013.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(13, "data\\SimpleExplosion014.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(14, "data\\SimpleExplosion015.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(15, "data\\SimpleExplosion016.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(16, "data\\SimpleExplosion017.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(17, "data\\SimpleExplosion018.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(18, "data\\SimpleExplosion019.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(19, "data\\SimpleExplosion020.bmp"))
		return false;

	//Enemy �̻��� ��������Ʈ		// �� �̻����� ��������Ʈ�� �ʱ�ȭ�ϰ� �����ӿ� �̹����� �ҷ��´�.
	if (!g_EnemyMissileSprite.InitSprite(1, 5, 20, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(0, "data\\Enemy_Missile.bmp"))
		return false;

	//boss sprite		// ������ ��������Ʈ�� �ʱ�ȭ�ϰ� �����ӿ� �̹����� �ҷ��´�.
	if (!g_BossSprite.InitSprite(1, 300, 225, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_BossSprite.LoadFrame(0, "data\\Boss_0000.bmp"))
		return false;

	//boss Missile sprite		// ���� �̻����� ��������Ʈ�� �ʱ�ȭ�ϰ� �����ӿ� �̹����� �ҷ��´�.
	if (!g_BossMissileSprite.InitSprite(1, 30, 30, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_BossMissileSprite.LoadFrame(0, "data\\Boss_Missile.bmp"))
		return false;

	//Clear&Dead		// ���Ӽ����� ȭ��� ���� ���н� ȭ���� ��������Ʈ�� �ʱ�ȭ�ϰ� �ش� �����ӿ� �ش� �̹����� �ҷ��´�.
	if (!g_ClearSprite.InitSprite(1, 400, 150, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_ClearSprite.LoadFrame(0, "data\\clear.bmp"))
		return false;
	if (!g_DeadSprite.InitSprite(1, 400, 150, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_DeadSprite.LoadFrame(0, "data\\dead.bmp"))
		return false;
	return true;
}

bool InitObject()	// ȣ��� true�� ��ȯ
{
	return true;
}
