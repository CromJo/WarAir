
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

//함수원형 선언
BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 lpKeyboard, int dikcode);

//외부변수 선언
extern LPDIRECTDRAWSURFACE7 g_lpPrimarySurface;
extern LPDIRECTDRAWSURFACE7 g_lpSecondarySurface;
extern LPDIRECTINPUT g_lpDirectInputObject;
extern LPDIRECTINPUTDEVICE8 g_lpDirectInputKeyboard;
extern CTimer g_Timer;

//게임 배경화면 스프라이트 변수
extern CSprite g_BackgroundSprite;

//보스 배열과 보스 스프라이트 변수
extern CBoss g_Boss[MAX_BOSS];
extern CSprite g_BossSprite;

//보스 미사일 배열과 보스 미사일 스프라이트 변수
extern CBossMissile g_BossMissile[MAX_BOSS_YMISSILES][MAX_BOSS_XMISSILES];
extern CSprite g_BossMissileSprite;

//적 비행기 배열과 적 비행기 스프라이트 변수
extern CEnemy g_Enemy[MAX_XENEMYS][MAX_YENEMYS];
extern CSprite g_EnemySprite;

//적 미사일 배열과 적 미사일 스프라이트 변수
extern CEnemyMissile g_EnemyMissile[MAX_XENEMYS][MAX_YENEMYS];
extern CSprite g_EnemyMissileSprite;

//적 비행기 배열과 적 비행기 스프라이트 변수
extern CEnemy g_Enemy[MAX_XENEMYS][MAX_YENEMYS];
extern CSprite g_EnemyMissileSprite;

//플레이어 비행기와 스프라이트 변수
extern CMyShip g_MyShip;
extern CSprite g_MyShipSprite;

//플레이어 미사일 배열과 스프라이트 변수
extern CMyMissile g_MyMissile[MAX_MISSILES];
extern CSprite g_MyMissileSprite;

//폭발 배열과 스프라이트 변수
extern CExploding g_Exploding[MAX_EXPLODES];
extern CSprite g_ExplodingSprite;

//게임 클리어 스프라이트와 데드엔드 스파리읕 변수
extern CSprite g_ClearSprite;
extern CSprite g_DeadSprite;

extern HSNDOBJ Sound[10]; //사운드 배열
extern void _Play(int num); //사운드 플레이 함수(사운드를 재생할 때 호출)

static BOOL m_bGameFirst = TRUE; // 게임 처음 시작 상태(초기 트루)
static BOOL m_bGameDead = FALSE; //데드엔드 상태(죽지 않음)
static BOOL m_bGameClear = FALSE; //게임 클리어 상태(클리어 하지 않음)
static BOOL m_bBoss = FALSE; //보스 상태(보스 없음)

int enemyCnt = MAX_XENEMYS * MAX_YENEMYS;//적 비행기 개체 수 초기화

static char szReplay[] = "You Dead... Replay? (y/n)"; //플레이어가 죽었을 경우 리플레이 의사를 묻는 텍스트를 출력
static char szWin[] = "You Win... Replay? (y/n)"; //플레이어가 승리했을 경우 리플레이 의사를 묻는 텍스트를 출력

void InitGame();//게임 초기화 함수
void MyShipDead();//데드엔드 함수
void MyShipWin();//게임 클리어 함수

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
		//보스
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

	//적의 움직임 처리와 그리기
	for (i = 0; i < MAX_XENEMYS; i++)
	{
		for (j = 0; j < MAX_YENEMYS; j++)
		{
			if (g_Enemy[i][j].IsLive())
			{
				g_Enemy[i][j].Move();
				g_Enemy[i][j].Draw(g_lpSecondarySurface);

			}


			//적 총알 생성 기간
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

	//총알 발사와 그리기
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

	//보스 미사일 드로우
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

	//플레이어 미사일 그리기
	for (m = 0; m < MAX_MISSILES; m++)
	{
		if (g_MyMissile[m].IsLive())
		{
			g_MyMissile[m].Move();
			g_MyMissile[m].Draw(g_lpSecondarySurface);
		}
	}

	//적 미사일 그리기
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

			//플레이어 비행기의 움직임 처리와 그리기
			int nMyShipFrame = 0; //중간 (center)

			if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_LEFT))//방향키 왼쪽 버튼을 눌렀을 경우
			{
				g_MyShip.Left();
				nMyShipFrame = 1;
			}
			if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_RIGHT))//방향키 오른쪽 버튼을 눌렀을 경우
			{
				g_MyShip.Right();
				nMyShipFrame = 2;
			}

			g_MyShip.DrawFrame(g_lpSecondarySurface, nMyShipFrame);//플레이어 출력
		
	
	//플레이어 총알과 적 충동 감지 및 폭파 처리
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
							g_MyMissile[m].Kill();//플레이어 미사일이 사라진다.
							g_Enemy[i][j].Kill();//적 비행기가 폭발된다.
							enemyCnt--;

						}
					}
				}
			}
		}
						g_Exploding[m].Draw(g_lpSecondarySurface);
		
		
		}

	//적의 총알과 플레이어 비행기 충돌 감지 및 폭파 처리
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
							g_EnemyMissile[i][j].Kill();;//적 미사일이 사라진다.
							g_MyShip.Kill();//플레이어 비행기가 폭발된다.

							g_Exploding[m].Draw(g_lpSecondarySurface);
							_Play(2);//폭발음을 들려준다.
							MyShipDead();//플레이어 데드
						}

					}
				}
			}
		}
	}
	//플레이어 총알과 보스 충돌 감지 및 폭파 처리
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
						g_MyMissile[m].Kill();//플레이어 미사일이 사라진다.
						g_Boss[i].m_Enegy -= 1;

						g_Exploding[m].Draw(g_lpSecondarySurface);
						_Play(2);//폭발음을 들려준다.

						if (g_Boss[i].m_Enegy == 0)//보스의 에너지가 0일 경우
						{
							g_Boss[i].Kill();//보스가 죽음.
							MyShipWin();//플레이어 승리

						}

					}
				}
			}
		}
	}

	//보스총알과 플레이어 충돌 감지 및 폭파 처리
	for (i = 0; i<MAX_EXPLODES; i++)
	{
		for (m = 0; m<MAX_BOSS_YMISSILES; m++)	// 보스의 세로 미사일만큼
		{
			for (m2 = 0; m2<MAX_BOSS_XMISSILES; m2++)	// 보스의 가로 미사일 만큼
			{
				if (g_BossMissile[m][m2].IsLive() && g_MyShip.IsLive())	// 보스의 미사일이 있고 플레이어가 있을때
				{
					if (abs(g_BossMissile[m][m2].GetX() - g_MyShip.GetX())<30)	// 보스 미사일과 플레이어 충돌체크
						if (abs(g_BossMissile[m][m2].GetY() - g_MyShip.GetY())<20)	// 이미지 크기
						{
							g_Exploding[i].Initialize(&g_ExplodingSprite, g_BossMissile[m][m2].GetX(),	// 폭파 초기화
								g_BossMissile[m][m2].GetY(), &g_Timer, 0, 10);	// 폭차 위치 설정
							g_BossMissile[m][m2].Kill();	// 보스미사일 없앰
							g_MyShip.Kill();		// 플레이어 없앰

							g_Exploding[i].Draw(g_lpSecondarySurface);	// 폭파이미지 그린다.
							_Play(2);	// 2번 사운드 출력
							MyShipDead();	// 게임실패 함수 호출
						}
				}
			}
		}
	}


	if (m_bGameClear == TRUE)//게임을 클리어할 경우
		g_ClearSprite.Drawing(0, 400, 300, g_lpSecondarySurface, true);//클리어 이미지를 그려준다.
	if (m_bGameDead == TRUE)//게임에 패배할 경우
		g_DeadSprite.Drawing(0, 400, 300, g_lpSecondarySurface, true);//패배 이미지를 그려준다.

																	   //플리핑
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

//게임 초기화
void InitGame()
{
	int i, j, m, m2;

	//폭발 초기화 Explodes Init
	for (i = 0; i < MAX_EXPLODES; i++)
	{
		if (g_Exploding[i].IsLive())
		{
			g_Exploding[i].Kill();//화면상의 폭발 이미지를 모두 지워버린다.
		}
	}
	//플레이어 비행기 초기화 MyShip Init
	if (g_MyShip.IsLive())
	{
		g_MyShip.Kill(); //화면상의 플레이어 비행기를 지워버린다.
	}
	g_MyShip.Initialize(&g_MyShipSprite, 400, 550, &g_Timer, 0, 25, 5);

	for (m = 0; m < MAX_MISSILES; m++)
	{
		if (g_MyMissile[m].IsLive())
		{
			g_MyMissile[m].Kill();//화면상의 플레이어 미사일을 모두 지워버린다.
		}
	}

	//적 비행기 초기화 Enemy Init
	for (i = 0; i < MAX_XENEMYS; i++)
	{
		for (j = 0; j < MAX_YENEMYS; j++)
		{
			if (g_Enemy[i][j].IsLive())
			{
				g_Enemy[i][j].Kill();//화면상의 적 비행기를 모두 지워버린다.
			}
			if (g_EnemyMissile[i][j].IsLive())
			{
				g_EnemyMissile[i][j].Kill();//화면상의 적 미사일을 모두 지워버린다.
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

	//보스 초기화 Boss Init
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
		g_Boss[i].m_speedx = 2;//보스 좌우로 움직이는 속도를 2로 초기화
		g_Boss[i].m_Enegy = 30;//보스 에너지를 30으로 초기화
	}

	for (m = 0; m < MAX_BOSS_YMISSILES; m++)
	{
		for (m2 = 0; m2 < MAX_BOSS_XMISSILES; m2++)
		{
			if (g_BossMissile[m][m2].IsLive())
			{
				g_BossMissile[m][m2].Kill();//보스 미사일이 모두 사라진다.
			}

			m_bGameDead = FALSE; //데드엔드 상태 초기화
			m_bGameClear = FALSE; //게임 클리어 상태 초기화
			m_bGameFirst = FALSE; //게임 처음 시작 상태 초기화
			m_bBoss = FALSE;//보스 상태 초기화
			enemyCnt = MAX_XENEMYS*MAX_YENEMYS;//적 비행기 개체 수 초기화

			SndObjPlay(Sound[0], DSBPLAY_LOOPING);
		}
	}
}

void MyShipDead()//플레이어 비행기가 폭발했을 경우 호출하는 함수
{
	m_bGameDead = TRUE; //데드엔드
	_Play(3); // 클리어 또는 데드엔드 사운드를 들려준다.
}

void MyShipWin() //플레이어가 게임에서 승리할 경우 호출하는 함수
{
	m_bGameClear = TRUE;//게임 클리어
	_Play(3);//클리어 또는 데드엔드 사운드를 들려준다.
}

void _Play(int num) // 사운드를 들려줄 때 호출하는 함수
{
	SndObjPlay(Sound[num], NULL);//num값을 받아서 값에 맞는 사운드를 호출한다.
}