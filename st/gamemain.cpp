
#include <ddraw.h>
#include <dinput.h>

#include "bmp.h"
#include "timer.h"
#include "sprite.h"
#include "enemy.h"
#include "myship.h"
#include "mymissile.h"
#include "exploding.h"
#include "enemymissile.h"
#include "boss.h"
#include "bossmissile.h"

#include "stdio.h"
#include <windows.h>

#include <dsound.h>
#include "dsutil.h"

#define MAX_MISSILES 2
#define MAX_XENEMYS 5
#define MAX_YENEMYS 5
#define MAX_EXPLODES 3
#define MAX_ENEMY_MISSILES 25
#define MAX_BOSS 1
#define MAX_BOSS_XMISSILES 3
#define MAX_BOSS_YMISSILES 5

#define MY_MISSILE_INTERVAL 240
#define BOSS_MISSILE_INTERVAL 100

//�Լ����� ����
BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 lpKeyboard, int dikcode);

//�ܺκ��� ����
extern LPDIRECTDRAWSURFACE7 g_lpPrimarySurface;
extern LPDIRECTDRAWSURFACE7 g_lpSecondarySurface;
extern LPDIRECTINPUT g_lpDirectInputObject;
extern LPDIRECTINPUTDEVICE8 g_lpDirectInputKeyboard;
extern CTimer g_Timer;

//���� ���ȭ�� ��������Ʈ ����
extern CSprite g_BackgroundSprite;

//���� �迭�� ���� ��������Ʈ ����
extern CBoss g_Boss[MAX_BOSS];
extern CSprite g_BossSprite;

//���� �̻��� �迭�� ���� �̻��� ��������Ʈ ����
extern CBossMissile g_BossMissile[MAX_BOSS_YMISSILES][MAX_BOSS_XMISSILES];
extern CSprite g_BossMissileSprite;

//�� ����� �迭�� �� ����� ��������Ʈ ����
extern CEnemy g_Enemy[MAX_XENEMYS][MAX_YENEMYS];
extern CSprite g_EnemySprite;

//�� �̻��� �迭�� �� �̻��� ��������Ʈ ����
extern CEnemyMissile g_EnemyMissile[MAX_XENEMYS][MAX_YENEMYS];
extern CSprite g_EnemyMissileSprite;

//�� ����� �迭�� �� ����� ��������Ʈ ����
extern CEnemy g_Enemy[MAX_XENEMYS][MAX_YENEMYS];
extern CSprite g_EnemyMissileSprite;

//�÷��̾� ������ ��������Ʈ ����
extern CMyShip g_MyShip;
extern CSprite g_MyShipSprite;

//�÷��̾� �̻��� �迭�� ��������Ʈ ����
extern CMyMissile g_MyMissile[MAX_MISSILES];
extern CSprite g_MyMissileSprite;

//���� �迭�� ��������Ʈ ����
extern CExploding g_Exploding[MAX_EXPLODES];
extern CSprite g_ExplodingSprite;

//���� Ŭ���� ��������Ʈ�� ���忣�� ���ĸ��� ����
extern CSprite g_ClearSprite;
extern CSprite g_DeadSprite;

extern HSNDOBJ Sound[10]; //���� �迭
extern void _Play(int num); //���� �÷��� �Լ�(���带 ����� �� ȣ��)

static BOOL m_bGameFirst = TRUE; // ���� ó�� ���� ����(�ʱ� Ʈ��)
static BOOL m_bGameDead = FALSE; //���忣�� ����(���� ����)
static BOOL m_bGameClear = FALSE; //���� Ŭ���� ����(Ŭ���� ���� ����)
static BOOL m_bBoss = FALSE; //���� ����(���� ����)

int enemyCnt = MAX_XENEMYS * MAX_YENEMYS;//�� ����� ��ü �� �ʱ�ȭ

static char szReplay[] = "You Dead... Replay? (y/n)"; //�÷��̾ �׾��� ��� ���÷��� �ǻ縦 ���� �ؽ�Ʈ�� ���
static char szWin[] = "You Win... Replay? (y/n)"; //�÷��̾ �¸����� ��� ���÷��� �ǻ縦 ���� �ؽ�Ʈ�� ���

void InitGame();//���� �ʱ�ȭ �Լ�
void MyShipDead();//���忣�� �Լ�
void MyShipWin();//���� Ŭ���� �Լ�

void GameMain(void)
{
	if (m_bGameFirst == TRUE)
		InitGame();

	int i, j, m, m2;

	static int time = 0;
	static int nLastMissileTime = g_Timer.time();
	static int nLastMissileTime2 = g_Timer.time();


	g_BackgroundSprite.Drawing2(0, 400, 300, g_lpSecondarySurface, false);

	if (enemyCnt == 0)
		m_bBoss = TRUE;

	if (m_bBoss == TRUE)
	{
		//����
		for (i = 0; i < MAX_BOSS; i++)
		{
			if (g_Boss[i].IsLive())
			{
				//g_Boss[i].Move()
				if ((g_MyShip.GetX() - g_Boss[i].GetX()) > 30)
					g_Boss[i].m_speedx = 2;
				if ((g_MyShip.GetX() - g_Boss[i].GetX()) < -30)
					g_Boss[i].m_speedx = -2;

				g_Boss[i].m_x += g_Boss[i].m_speedx;

				g_Boss[i].Draw(g_lpSecondarySurface);

			}
			for (m = 0; m < MAX_BOSS_YMISSILES; m++)
			{
				if (time % 51 == 0)
				{
					for (m2 = 0; m2 < MAX_BOSS_XMISSILES; m2++)
					{
						if (!g_BossMissile[m][m2].IsLive() && g_Boss[i].IsLive())
						{
							g_BossMissile[m][m2].Initialize(&g_BossMissileSprite, g_Boss[i].GetX(), g_Boss[i].GetY() + 80, &g_Timer, 0, 10, 5);
							g_BossMissile[m][0].m_speedx = -2;
							g_BossMissile[m][1].m_speedx = 0;
							g_BossMissile[m][2].m_speedx = 2;

						}
					}
				}
				time++;
			}
		}
	}

	//���� ������ ó���� �׸���
	for (i = 0; i < MAX_XENEMYS; i++)
	{
		for (j = 0; j < MAX_YENEMYS; j++)
		{
			if (g_Enemy[i][j].IsLive())
			{
				g_Enemy[i][j].Move();
				g_Enemy[i][j].Draw(g_lpSecondarySurface);

			}


			//�� �Ѿ� ���� �Ⱓ
			int nansu = rand() % 150;
			if (nansu == 1)
			{
				if (!g_EnemyMissile[i][j].IsLive() && g_Enemy[i][j].IsLive())
				{
					g_EnemyMissile[i][j].Initialize(&g_EnemyMissileSprite, g_Enemy[i][j].GetX(), g_Enemy[i][j].GetY() + 20, &g_Timer, 0, 100, 5);

				}
			}
		}
	}

	//�Ѿ� �߻�� �׸���
	if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_SPACE))
	{
		if (g_MyShip.IsLive())
		{
			if (g_Timer.elapsed(nLastMissileTime, MY_MISSILE_INTERVAL))
			{
				for (m = 0; m < MAX_MISSILES; m++)
				{
					if (!g_MyMissile[m].IsLive())
						break;
				}
				if (m == MAX_MISSILES)
					return;

				g_MyMissile[m].Initialize(&g_MyMissileSprite, g_MyShip.GetX(), g_MyShip.GetY() - 20, &g_Timer, 0, 10, 5);

			}
		}
	}

	//���� �̻��� ��ο�
	for (m = 0; m < MAX_BOSS_YMISSILES; m++)
	{
		for (m2 = 0; m2 < MAX_BOSS_XMISSILES; m2++)
		{
			if (g_BossMissile[m][m2].IsLive())
			{
				g_BossMissile[m][m2].Move();
				g_BossMissile[m][m2].Draw(g_lpSecondarySurface);

			}
		}
	}

	//�÷��̾� �̻��� �׸���
	for (m = 0; m < MAX_MISSILES; m++)
	{
		if (g_MyMissile[m].IsLive())
		{
			g_MyMissile[m].Move();
			g_MyMissile[m].Draw(g_lpSecondarySurface);
		}
	}

	//�� �̻��� �׸���
	for (i = 0; i < MAX_XENEMYS; i++)
	{
		for (j = 0; j < MAX_YENEMYS; j++)
		{
			if (g_EnemyMissile[i][j].IsLive())
			{
				g_EnemyMissile[i][j].Move();
				g_EnemyMissile[i][j].Draw(g_lpSecondarySurface);

			}
		}
	}

			//�÷��̾� ������� ������ ó���� �׸���
			int nMyShipFrame = 0; //�߰� (center)

			if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_LEFT))//����Ű ���� ��ư�� ������ ���
			{
				g_MyShip.Left();
				nMyShipFrame = 1;
			}
			if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_RIGHT))//����Ű ������ ��ư�� ������ ���
			{
				g_MyShip.Right();
				nMyShipFrame = 2;
			}

			g_MyShip.DrawFrame(g_lpSecondarySurface, nMyShipFrame);//�÷��̾� ���
		
	
	//�÷��̾� �Ѿ˰� �� �浿 ���� �� ���� ó��
	for (m = 0; m < MAX_MISSILES; m++)
	{
		for (i = 0; i < MAX_XENEMYS; i++)
		{
			for (j = 0; j < MAX_YENEMYS; j++)
			{
				if (g_MyMissile[m].IsLive() && g_Enemy[i][j].IsLive())
				{
					if (abs(g_MyMissile[m].GetX() - g_Enemy[i][j].GetX()) < 30)
					{
						if (abs(g_MyMissile[m].GetY() - g_Enemy[i][j].GetY()) < 30)
						{
							g_Exploding[m].Initialize(&g_ExplodingSprite, g_MyMissile[m].GetX(), g_MyMissile[m].GetY(), &g_Timer, 0, 10);
							g_MyMissile[m].Kill();//�÷��̾� �̻����� �������.
							g_Enemy[i][j].Kill();//�� ����Ⱑ ���ߵȴ�.
							enemyCnt--;

						}
					}
				}
			}
		}
						g_Exploding[m].Draw(g_lpSecondarySurface);
		
		
		}

	//���� �Ѿ˰� �÷��̾� ����� �浹 ���� �� ���� ó��
	for (m = 0; m < MAX_EXPLODES; m++)
	{
		for (i = 0; i < MAX_XENEMYS; i++)
		{
			for (j = 0; j < MAX_YENEMYS; j++)
			{
				if (g_EnemyMissile[i][j].IsLive() && g_MyShip.IsLive())
				{
					if (abs(g_EnemyMissile[i][j].GetX() - g_MyShip.GetX()) < 30)
					{
						if (abs(g_EnemyMissile[i][j].GetY() - g_MyShip.GetY()) < 20)
						{
							g_Exploding[m].Initialize(&g_ExplodingSprite, g_EnemyMissile[i][j].GetX(), g_EnemyMissile[i][j].GetY(), &g_Timer, 0, 10);
							g_EnemyMissile[i][j].Kill();;//�� �̻����� �������.
							g_MyShip.Kill();//�÷��̾� ����Ⱑ ���ߵȴ�.

							g_Exploding[m].Draw(g_lpSecondarySurface);
							_Play(2);//�������� ����ش�.
							MyShipDead();//�÷��̾� ����
						}

					}
				}
			}
		}
	}
	//�÷��̾� �Ѿ˰� ���� �浹 ���� �� ���� ó��
	for (m = 0; m < MAX_MISSILES; m++)
	{
		for (i = 0; i < MAX_BOSS; i++)
		{
			if (g_MyMissile[m].IsLive() && g_Boss[i].IsLive() && m_bBoss == TRUE)
			{
				if (abs(g_MyMissile[m].GetX() - g_Boss[i].GetX()) < 100)
				{
					if (abs(g_MyMissile[m].GetY() - g_Boss[i].GetY()) < 70)
					{
						g_Exploding[m].Initialize(&g_ExplodingSprite, g_MyMissile[m].GetX(), g_MyMissile[m].GetY(), &g_Timer, 0, 10);
						g_MyMissile[m].Kill();//�÷��̾� �̻����� �������.
						g_Boss[i].m_Enegy -= 1;

						g_Exploding[m].Draw(g_lpSecondarySurface);
						_Play(2);//�������� ����ش�.

						if (g_Boss[i].m_Enegy == 0)//������ �������� 0�� ���
						{
							g_Boss[i].Kill();//������ ����.
							MyShipWin();//�÷��̾� �¸�

						}

					}
				}
			}
		}
	}

	//�����Ѿ˰� �÷��̾� �浹 ���� �� ���� ó��
	for (i = 0; i<MAX_EXPLODES; i++)
	{
		for (m = 0; m<MAX_BOSS_YMISSILES; m++)	// ������ ���� �̻��ϸ�ŭ
		{
			for (m2 = 0; m2<MAX_BOSS_XMISSILES; m2++)	// ������ ���� �̻��� ��ŭ
			{
				if (g_BossMissile[m][m2].IsLive() && g_MyShip.IsLive())	// ������ �̻����� �ְ� �÷��̾ ������
				{
					if (abs(g_BossMissile[m][m2].GetX() - g_MyShip.GetX())<30)	// ���� �̻��ϰ� �÷��̾� �浹üũ
						if (abs(g_BossMissile[m][m2].GetY() - g_MyShip.GetY())<20)	// �̹��� ũ��
						{
							g_Exploding[i].Initialize(&g_ExplodingSprite, g_BossMissile[m][m2].GetX(),	// ���� �ʱ�ȭ
								g_BossMissile[m][m2].GetY(), &g_Timer, 0, 10);	// ���� ��ġ ����
							g_BossMissile[m][m2].Kill();	// �����̻��� ����
							g_MyShip.Kill();		// �÷��̾� ����

							g_Exploding[i].Draw(g_lpSecondarySurface);	// �����̹��� �׸���.
							_Play(2);	// 2�� ���� ���
							MyShipDead();	// ���ӽ��� �Լ� ȣ��
						}
				}
			}
		}
	}


	if (m_bGameClear == TRUE)//������ Ŭ������ ���
		g_ClearSprite.Drawing(0, 400, 300, g_lpSecondarySurface, true);//Ŭ���� �̹����� �׷��ش�.
	if (m_bGameDead == TRUE)//���ӿ� �й��� ���
		g_DeadSprite.Drawing(0, 400, 300, g_lpSecondarySurface, true);//�й� �̹����� �׷��ش�.

																	   //�ø���
	HRESULT hResult;
	if (FAILED(hResult = g_lpPrimarySurface->Flip(NULL, DDFLIP_WAIT)))
	{
		if (hResult == DDERR_SURFACELOST)
			g_lpPrimarySurface->Release();

	}

}
BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 lpKeyboard, int dikcode)
{
	char KeyBuffer[256];
	HRESULT hResult;

	if (lpKeyboard->GetDeviceState(256, (LPVOID)KeyBuffer) == DIERR_INPUTLOST)
	{
		while (hResult = lpKeyboard->Acquire() == DIERR_INPUTLOST)
		{
			hResult = lpKeyboard->Acquire();
		}
	}
	return (KeyBuffer[dikcode] & 0x80);
}

//���� �ʱ�ȭ
void InitGame()
{
	int i, j, m, m2;

	//���� �ʱ�ȭ Explodes Init
	for (i = 0; i < MAX_EXPLODES; i++)
	{
		if (g_Exploding[i].IsLive())
		{
			g_Exploding[i].Kill();//ȭ����� ���� �̹����� ��� ����������.
		}
	}
	//�÷��̾� ����� �ʱ�ȭ MyShip Init
	if (g_MyShip.IsLive())
	{
		g_MyShip.Kill(); //ȭ����� �÷��̾� ����⸦ ����������.
	}
	g_MyShip.Initialize(&g_MyShipSprite, 400, 550, &g_Timer, 0, 25, 5);

	for (m = 0; m < MAX_MISSILES; m++)
	{
		if (g_MyMissile[m].IsLive())
		{
			g_MyMissile[m].Kill();//ȭ����� �÷��̾� �̻����� ��� ����������.
		}
	}

	//�� ����� �ʱ�ȭ Enemy Init
	for (i = 0; i < MAX_XENEMYS; i++)
	{
		for (j = 0; j < MAX_YENEMYS; j++)
		{
			if (g_Enemy[i][j].IsLive())
			{
				g_Enemy[i][j].Kill();//ȭ����� �� ����⸦ ��� ����������.
			}
			if (g_EnemyMissile[i][j].IsLive())
			{
				g_EnemyMissile[i][j].Kill();//ȭ����� �� �̻����� ��� ����������.
			}

		}
	}
	for (i = 0; i < MAX_XENEMYS; i++)
	{
		for (j = 0; j < MAX_YENEMYS; j++)
		{
			g_Enemy[i][j].Initialize(&g_EnemySprite, (i + 1) * 100, (j + 1) * 60, &g_Timer, 0, 25, 5);
			g_Enemy[i][j].m_speedx = 2;
		}
	}

	//���� �ʱ�ȭ Boss Init
	for (i = 0; i < MAX_BOSS; i++)
	{
		if (g_Boss[i].IsLive())
		{
			g_Boss[i].Kill();
		}
	}
	for (i = 0; i < MAX_BOSS; i++)
	{
		g_Boss[i].Initialize(&g_BossSprite, (i + 1) * 100, 130, &g_Timer, 0, 25, 5);
		g_Boss[i].m_speedx = 2;//���� �¿�� �����̴� �ӵ��� 2�� �ʱ�ȭ
		g_Boss[i].m_Enegy = 30;//���� �������� 30���� �ʱ�ȭ
	}

	for (m = 0; m < MAX_BOSS_YMISSILES; m++)
	{
		for (m2 = 0; m2 < MAX_BOSS_XMISSILES; m2++)
		{
			if (g_BossMissile[m][m2].IsLive())
			{
				g_BossMissile[m][m2].Kill();//���� �̻����� ��� �������.
			}

			m_bGameDead = FALSE; //���忣�� ���� �ʱ�ȭ
			m_bGameClear = FALSE; //���� Ŭ���� ���� �ʱ�ȭ
			m_bGameFirst = FALSE; //���� ó�� ���� ���� �ʱ�ȭ
			m_bBoss = FALSE;//���� ���� �ʱ�ȭ
			enemyCnt = MAX_XENEMYS*MAX_YENEMYS;//�� ����� ��ü �� �ʱ�ȭ

			SndObjPlay(Sound[0], DSBPLAY_LOOPING);
		}
	}
}

void MyShipDead()//�÷��̾� ����Ⱑ �������� ��� ȣ���ϴ� �Լ�
{
	m_bGameDead = TRUE; //���忣��
	_Play(3); // Ŭ���� �Ǵ� ���忣�� ���带 ����ش�.
}

void MyShipWin() //�÷��̾ ���ӿ��� �¸��� ��� ȣ���ϴ� �Լ�
{
	m_bGameClear = TRUE;//���� Ŭ����
	_Play(3);//Ŭ���� �Ǵ� ���忣�� ���带 ����ش�.
}

void _Play(int num) // ���带 ����� �� ȣ���ϴ� �Լ�
{
	SndObjPlay(Sound[num], NULL);//num���� �޾Ƽ� ���� �´� ���带 ȣ���Ѵ�.
}