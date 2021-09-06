//main.cpp

#define WIN32_LEAN_AND_MEAN	// 컴파일러에게 외부 mfc 오버헤더를 포함하지 말도록 지시한다

#include <windows.h>	// 윈도우즈 관련 헤더 파일
#include <windowsx.h>

#include <ddraw.h>		// 다이렉트 드로우 관련 헤더 파일
#include <dinput.h>		// 다이렉트 인풋 관련 헤더파일

#include "bmp.h"		// bmp관련 헤더 파일
#include "timer.h"		// 시간 관련 헤더 파일
#include "enemy.h"		// 적 관련 헤더 파일
#include "sprite.h"		// 스프라이트 그리기 관련 헤더파일
#include "myship.h"		// 플레이어 관련 헤더 파일
#include "mymissile.h"	//  내 미사일 관련 헤더 파일
#include "exploding.h"		// 폭파 관련 헤더 파일
#include "enemymissile.h"	// 적 미사일 관련 헤더파일
#include "boss.h"		// 보스관련 헤더파일
#include "bossmissile.h"	// 보스 미사일 관련 헤더파일

#include "define.h"	// 디파인헤더
// 함수 원형 선언 //
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);	// 윈도우 프록
bool InitializeDirectX(void);		// 다이렉트 x 초기화 
void GameMain(void);		// 게임 메인
void InitGame();		// 게임 초기화
bool LoadBMPandInitSurface();	// 비트맵 불러오고 초기화 해주는 함수
bool InitObject();	// 오브젝트 초기화

// 전역 변수 정의 //
// 모든 전역 변수 앞에는 g_를 앞에 붙여서 사용하였다.
HWND			g_hwnd;		// 윈도우 핸들
HINSTANCE		g_hInstance;		// 인스턴스 핸들

LPDIRECTDRAW7				g_lpDirectDrawObject = NULL;	// 다이렉트 드로우 객체 생성
LPDIRECTDRAWSURFACE7		g_lpPrimarySurface = NULL;		// 1차 표면	생성
LPDIRECTDRAWSURFACE7		g_lpSecondarySurface = NULL;	// 2차 표면 생성

LPDIRECTINPUT8				g_lpDirectInputObject = NULL;		// 입력받은 값
LPDIRECTINPUTDEVICE8		g_lpDirectInputKeyboard = NULL;		// 입력 받는 키보드값

CSprite					g_TitleSprite;			//로딩화면 스프라이트
CSprite					g_BackgroundSprite;		// 배경 스프라이트
CSprite					g_EnemySprite;			// 적 스프라이트
CSprite					g_EnemyMissileSprite;	// 적 미사일 스프라이트
CSprite					g_MyShipSprite;		// 내 비행기 스프라이트
CSprite					g_MyMissileSprite;		// 내 미사일 스프라이트
CSprite					g_ExplodingSprite;		// 폭파 스프라이트
CSprite					g_BossSprite;		// 보스 스프라이트 
CSprite					g_BossMissileSprite;	// 보스미사일 스프라이트
CSprite					g_ClearSprite;	// 게임 성공 스프라이트
CSprite					g_DeadSprite;	// 게임 실패 스프라이트 

CTimer					g_Timer;		// 시간 클래스 객체 생성

bool					g_bActiveApp = false;	// 게임이 엑티브 한가?

CEnemy					g_Enemy[MAX_XENEMYS][MAX_YENEMYS];		// 적 관련 객체 생성
CEnemyMissile			g_EnemyMissile[MAX_XENEMYS][MAX_YENEMYS];	// 적 미사일 관련 객체 생성
CMyShip					g_MyShip;	// 내 비행기 관련 객체 생성
CMyMissile				g_MyMissile[MAX_MISSILES];	// 내 미사일 관련 객체 생성
CBoss					g_Boss[MAX_BOSS];	// 보스관련 객체 생성
CBossMissile			g_BossMissile[MAX_BOSS_YMISSILES][MAX_BOSS_XMISSILES];	// 보스 미사일관련 객체 생성
CExploding				g_Exploding[MAX_EXPLODES];	// 폭파 관련 객체 생성

extern BOOL _InitDirectSound();	// 다이렉트 사운드 초기화 값을 받아온다.
// 전역 변수 정의 끝 //

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//WindowClass 등록
	WNDCLASSEX wndclass;	// 윈도우 클래스 객체 생성
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;	// 스타일
	wndclass.lpfnWndProc = WinProc;	// 윈도우 프록
	wndclass.cbClsExtra = wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;	// 인스턴스 핸들러
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));  //아이콘 등록
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);	// 커서 등록
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	// 배경 색 
	wndclass.lpszMenuName = NULL;	// 메뉴 
	wndclass.lpszClassName = CLASS_NAME;	// 클래스 이름
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (RegisterClassEx(&wndclass) == 0)
		return 0;
	// window를 creation  // 우선순위!! 화면전환이 불가능 해진다.
	g_hwnd = CreateWindowEx(WS_EX_TOPMOST, CLASS_NAME, CLASS_NAME, WS_POPUP | WS_VISIBLE,
		0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),	// 모니터의 x,y 크기를 구해온다.
		NULL, NULL, hInstance, NULL);

	if (g_hwnd == NULL) return 0;	// g_hwnd는 MessageBox() 호출 시 필요하다.
	g_hInstance = hInstance;		// DirectInput초기화때 필요하다

	SetFocus(g_hwnd);		// 현재의 포커스를 가져온다.
	ShowCursor(FALSE);		// false로 마우스 포인터를 숨긴다.

	// DirectDraw 초기화
	if (!InitializeDirectX())	//다이렉트 x를 초기화 
		return 0;		// 초기화 하지 못했을때 0을 리턴한다.

	if (!_InitDirectSound())	// 다이렉트 사운드를 초기화
		return 0;	// 최기화 하지 못했을때 0을 리턴

	g_Timer.start();	// 타이머 시작

	if (!LoadBMPandInitSurface())	// 비트맵 파일을 불러오고 초기화한다.
		return 0;

	if (!InitObject())	// 오브젝트 초기화..?  true를 반환
		return 0;

	//Message loop
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))	//메시지의 존재여부를 확인함
		{
			if (!GetMessage(&msg, NULL, 0, 0))	// 메시지를 얻어온다. 얻지못하면
				return (int)msg.wParam;	// 키보드 메시지값을 정수형으로 변환하여 보낸다.
			TranslateMessage(&msg);		// 키보드관련 메시지
			DispatchMessage(&msg);		// Window procdure 호출
		}
		else if (g_bActiveApp)	// 게임의 실행여부를 판단하여
			GameMain();	// 게임 메인을 호출
		else
			WaitMessage();	// 메시지가 들어올때까지 기다린다.
	}
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)		// 윈도우 프록
{
	switch (message)
	{

	case WM_ACTIVATEAPP:		// 프로그램의 활성, 비활성 여부를 얻어온다.
		if (wParam)
			g_bActiveApp = true;	// 활동중이면 트루
		else
			g_bActiveApp = false;	// 아니면 펄스
		break;

	case WM_KEYDOWN:		// 키가 눌렷을때
		if (wParam == 'Y') InitGame();	// y라면 게임 초기화
		if (wParam == 'N') DestroyWindow(hwnd);	//n이라면 게임 종료
		if (wParam == VK_ESCAPE) DestroyWindow(hwnd);	// esc라면 게임 종료
		break;

	case WM_DESTROY:		// 프로그램 종료시
		if (g_lpDirectDrawObject)
		{
			if (g_lpSecondarySurface)		// 2차버퍼 소멸
				g_lpSecondarySurface->Release();
			if (g_lpPrimarySurface)				// 1차 버퍼 소멸
				g_lpPrimarySurface->Release();
			g_DeadSprite.ReleaseAll();		// 게임 실패 스프라이트 내의 모든 Surface를 메모리 해제
			g_ClearSprite.ReleaseAll();		// 게임 성공 스프라이트 내의 모든 Surface를 메모리 해제 
			g_BossMissileSprite.ReleaseAll();	// 보스 미사일 스프라이트 내의 모든 Surface를 메모리 해제 
			g_BossSprite.ReleaseAll();	// 보스 스프라이트 내의 모든 Surface를 메모리 해제 
			g_EnemyMissileSprite.ReleaseAll();	// 적미사일 스프라이트 내의 모든 Surface를 메모리 해제 
			g_EnemySprite.ReleaseAll();	// 적 스프라이트 내의 모든 Surface를 메모리 해제 
			g_BackgroundSprite.ReleaseAll();	// 배경 스프라이트 내의 모든 Surface를 메모리 해제 
			g_TitleSprite.ReleaseAll();		// 타이틀(로딩화면) 스프라이트 내의 모든 Surface를 메모리 해제 

			g_lpDirectDrawObject->Release();	// 
		}
		if (g_lpDirectInputObject)
		{
			if (g_lpDirectInputKeyboard)
			{
				g_lpDirectInputKeyboard->Unacquire();  // 접근 권한을 준다
				g_lpDirectInputKeyboard->Release();	// 키보드값 소멸
			}
			g_lpDirectInputObject->Release();	// 입력값 소멸
		}

		ShowCursor(TRUE);	//커서를 보이게 설정
		PostQuitMessage(0);	// 종료
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
		// 만약에 위에서 처리를 하지 못했다면 기본값 설정
	}
	return 0;
}
bool LoadBMPandInitSurface() // 비트맵 불러오는 함수
{
	//타이틀 화면 출력
	if (!g_TitleSprite.InitSprite(1, 800, 600, COLOR_KEY, g_lpDirectDrawObject))	//타이틀 화면 초기화
		return false;
	if (!g_TitleSprite.LoadFrame(0, "data\\title.bmp"))	// 타이틀화면 설정 성공시 그냥 넘어가고 실패시 리턴 펄스
		return false;
	g_TitleSprite.Drawing(0, 400, 300, g_lpSecondarySurface, false);	// 타이틀 화면을 그린다.

	//Flipping
	HRESULT hResult;
	if (FAILED(hResult = g_lpPrimarySurface->Flip(NULL, DDFLIP_WAIT)))	// 플리핑 처리
	{
		if (hResult == DDERR_SURFACELOST)	// 플리핑을 참조해서 Restore()에 연결해준다.
			g_lpPrimarySurface->Restore();
	}
	//백그라운드 스프라이트
	if (!g_BackgroundSprite.InitSprite(1, 800, 1200, COLOR_KEY, g_lpDirectDrawObject))	// 1개의 프레임의 800*1200 크기의 배경스프라이트를 초기화
		return false;
	if (!g_BackgroundSprite.LoadFrame(0, "data\\background.bmp"))	// 0번째 프레임에 해당 파일경로에서 불러온다.
		return false;

	//적 스프라이트					// 적의 스프라이트의 크기 만큼 초기화 한후
	if (!g_EnemySprite.InitSprite(10, 100, 75, COLOR_KEY, g_lpDirectDrawObject))
		return false;				// 각 프레임에 각 파일을 불러온다. 불러오기 실패하면 리턴 펄스
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

	//내 비행선 스프라이트			//내 비행기의 스프라이트 초기화
	if (!g_MyShipSprite.InitSprite(3, 90, 82, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	// 그냥 갈때 왼쪽, 오른쪽 이미지를  해당 프레임에 불러온다.
	if (!g_MyShipSprite.LoadFrame(0, "data\\myship_center.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(1, "data\\myship_left.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(2, "data\\myship_right.bmp"))
		return false;

	//내 미사일 스프라이트		// 내 미사일을 초기화
	if (!g_MyMissileSprite.InitSprite(8, 28, 50, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	// 미사일이 나갈때 연속된 이미지를 각각의 프레임에 불러온다.
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

	//폭파장면 스프라이트		// 폭파장면 스프라이트 초기화
	if (!g_ExplodingSprite.InitSprite(20, 160, 120, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	// 폭파장면의 이미지들을 각각의 프레임에 불러온다.
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

	//Enemy 미사일 스프라이트		// 적 미사일의 스프라이트를 초기화하고 프레임에 이미지를 불러온다.
	if (!g_EnemyMissileSprite.InitSprite(1, 5, 20, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(0, "data\\Enemy_Missile.bmp"))
		return false;

	//boss sprite		// 보스의 스프라이트를 초기화하고 프레임에 이미지를 불러온다.
	if (!g_BossSprite.InitSprite(1, 300, 225, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_BossSprite.LoadFrame(0, "data\\Boss_0000.bmp"))
		return false;

	//boss Missile sprite		// 보스 미사일의 스프라이트를 초기화하고 프레임에 이미지를 불러온다.
	if (!g_BossMissileSprite.InitSprite(1, 30, 30, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_BossMissileSprite.LoadFrame(0, "data\\Boss_Missile.bmp"))
		return false;

	//Clear&Dead		// 게임성공시 화면과 게임 실패시 화면의 스프라이트를 초기화하고 해당 프레임에 해당 이미지를 불러온다.
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

bool InitObject()	// 호출시 true를 반환
{
	return true;
}
