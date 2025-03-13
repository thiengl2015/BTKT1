/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 01 - SKELETON CODE 

	This sample illustrates how to:

	1/ Re-organize introductory code to an initial skeleton for better scalability
	2/ CGame is a singleton, playing a role of an "engine".
	3/ CGameObject is an abstract class for all game objects
	4/ CTexture is a wrapper class for ID3D10TEXTURE 
	
	NOTE: to create transparent background, download GIMP, then use Color to Alpha feature 
================================================================ */

#include <windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <vector>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"

#define WINDOW_CLASS_NAME L"Game Window"
#define MAIN_WINDOW_TITLE L"01 - Skeleton"
#define WINDOW_ICON_PATH L"brick.ico"



#define TEXTURE_PATH_TANK1 L"tank1.png"
#define TEXTURE_PATH_TANK2 L"tank2.png"
#define TEXTURE_PATH_TANK3 L"tank3.png"
#define TEXTURE_PATH_TANK4 L"tank4.png"
#define TEXTURE_PATH_TANK5 L"tank5.png"

#define TEXTURE_PATH_EW1 L"ewhite1.png"
#define TEXTURE_PATH_EW2 L"ewhite2.png"
#define TEXTURE_PATH_EW3 L"ewhite3.png"
#define TEXTURE_PATH_EW4 L"ewhite4.png"

#define TEXTURE_PATH_EG1 L"egreen1.png"
#define TEXTURE_PATH_EG2 L"egreen2.png"
#define TEXTURE_PATH_EG3 L"egreen3.png"
#define TEXTURE_PATH_EG4 L"egreen4.png"

#define TEXTURE_PATH_ER1 L"ered1.png"
#define TEXTURE_PATH_ER2 L"ered2.png"
#define TEXTURE_PATH_ER3 L"ered3.png"
#define TEXTURE_PATH_ER4 L"ered4.png"

#define TEXTURE_PATH_BULLET1 L"bullet1.png"
#define TEXTURE_PATH_BULLET2 L"bullet2.png"
#define TEXTURE_PATH_BULLET3 L"bullet3.png"
#define TEXTURE_PATH_BULLET4 L"bullet4.png"

#define TEXTURE_PATH_MISC L"misc.png"

#define BACKGROUND_COLOR D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240



using namespace std;

CTank *tank;
#define TANK_START_X 160.0f
#define TANK_START_Y 190.0f
#define TANK_START_VX 0.1f
#define TANK_START_VY 0.1f

#define E1_START_X 100.0f
#define E1_START_Y 100.0f
#define E1_START_VX 0.0f
#define E1_START_VY 0.0f


vector<CEnemy*> enemys;
LPTEXTURE texTank1 = NULL;
LPTEXTURE texTank2 = NULL;
LPTEXTURE texTank3 = NULL;
LPTEXTURE texTank4 = NULL;
LPTEXTURE texTank5 = NULL;

LPTEXTURE texEW1 = NULL;
LPTEXTURE texEW2 = NULL;
LPTEXTURE texEW3 = NULL;
LPTEXTURE texEW4 = NULL;

LPTEXTURE texEG1 = NULL;
LPTEXTURE texEG2 = NULL;
LPTEXTURE texEG3 = NULL;
LPTEXTURE texEG4 = NULL;

LPTEXTURE texER1 = NULL;
LPTEXTURE texER2 = NULL;
LPTEXTURE texER3 = NULL;
LPTEXTURE texER4 = NULL;


LPTEXTURE texBulletUp = NULL;
LPTEXTURE texBulletDown = NULL;
LPTEXTURE texBulletLeft = NULL;
LPTEXTURE texBulletRight = NULL;
LPTEXTURE texMisc = NULL;
CBullet* bullet;
CBullet* ew1;
vector<LPGAMEOBJECT> objects;  

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources. In this example, create a brick object and Tank object
*/

void LoadResources()
{
	CGame * game = CGame::GetInstance();
	texTank1 = game->LoadTexture(TEXTURE_PATH_TANK1);
	texTank5 = game->LoadTexture(TEXTURE_PATH_TANK5);
	texTank3 = game->LoadTexture(TEXTURE_PATH_TANK3);
	texTank4 = game->LoadTexture(TEXTURE_PATH_TANK4);

	texMisc = game->LoadTexture(TEXTURE_PATH_MISC);

	texBulletUp = game->LoadTexture(TEXTURE_PATH_BULLET1);
	texBulletDown = game->LoadTexture(TEXTURE_PATH_BULLET3);
	texBulletLeft = game->LoadTexture(TEXTURE_PATH_BULLET2);
	texBulletRight = game->LoadTexture(TEXTURE_PATH_BULLET4);

	texEW1 = game->LoadTexture(TEXTURE_PATH_EW1);
	texEW2 = game->LoadTexture(TEXTURE_PATH_EW2);
	texEW3 = game->LoadTexture(TEXTURE_PATH_EW3);
	texEW4 = game->LoadTexture(TEXTURE_PATH_EW4);

	texEG1 = game->LoadTexture(TEXTURE_PATH_EG1);
	texEG2 = game->LoadTexture(TEXTURE_PATH_EG2);
	texEG3 = game->LoadTexture(TEXTURE_PATH_EG3);
	texEG4 = game->LoadTexture(TEXTURE_PATH_EG4);

	texER1 = game->LoadTexture(TEXTURE_PATH_ER1);
	texER2 = game->LoadTexture(TEXTURE_PATH_ER2);
	texER3 = game->LoadTexture(TEXTURE_PATH_ER3);
	texER4 = game->LoadTexture(TEXTURE_PATH_ER4);

	ew1 = new CBullet(0, 0, 0.0, 0.0, texBulletUp, texBulletDown, texBulletLeft, texBulletRight);
	bullet = new CBullet(0, 0, 0.0, 0.0, texBulletUp,texBulletDown,texBulletLeft,texBulletRight);
	tank = new CTank(TANK_START_X, TANK_START_Y, TANK_START_VX, TANK_START_VY, texTank1, texTank3,texTank5,texTank4, bullet);

	enemys.clear();

	for (int i = 0; i < 2; i++) {
		float x = 100 + i * 50; // Cách đều nhau 50px
		float y = 50;

		// Tạo đạn riêng cho mỗi enemy
		CBullet* enemyBullet = new CBullet(0, 0, 0.0, 0.0, texBulletUp, texBulletDown, texBulletLeft, texBulletRight);

		// Enemy màu trắng
		enemys.push_back(new CEnemy(x, y, 0.05f, 0, texEW1, texEW2, texEW3, texEW4, enemyBullet, 1,true));

		// Enemy màu xanh
		enemys.push_back(new CEnemy(x*1.5, y*1.5 , 0.05f, 0, texEG1, texEG2, texEG3, texEG4, enemyBullet, 2,true));

		// Enemy màu đỏ
		enemys.push_back(new CEnemy(x*2, y*2 , 0.05f, 0, texER1, texER2, texER3, texER4, enemyBullet, 3,true));
	}

	for (auto enemy : enemys) {
		objects.push_back(enemy);
	}

	//brick = new CBrick(BRICK_X, BRICK_Y, texBrick);
	objects.push_back(tank);

	
}

void Update(DWORD dt)
{
	tank->Update(dt);

	for (auto& enemy : enemys) {
		enemy->Update(dt);

		// Nếu enemy đang active thì kiểm tra va chạm với đạn của tank
		if (enemy->IsActive() && tank->GetBullet()->getActive() && tank->CheckCollision(enemy, tank->GetBullet())) {
			enemy->TakeDamage(texEW1,texEW3,texEW2,texEW4,texEG1,texEG3,texEG2,texEG4);
			tank->GetBullet()->SetActive(false); // Đạn của tank biến mất sau khi bắn trúng
		}
		if (enemy->GetBullet()->getActive() && tank->GetBullet()->getActive() && enemy->CheckCollision(enemy->GetBullet(), tank->GetBullet()))
		{
			tank->GetBullet()->SetActive(false);
			enemy->GetBullet()->SetActive(false);
		}
	}

	// Xoá các enemy không còn tồn tại
	enemys.erase(std::remove_if(enemys.begin(), enemys.end(), [](CEnemy* e) { return !e->IsActive(); }), enemys.end());
}

/*
	Render a frame 
*/
void Render()
{
	CGame* g = CGame::GetInstance();

	ID3D10Device* pD3DDevice = g->GetDirect3DDevice();
	IDXGISwapChain* pSwapChain = g->GetSwapChain();
	ID3D10RenderTargetView* pRenderTargetView = g->GetRenderTargetView();
	ID3DX10Sprite* spriteHandler = g->GetSpriteHandler();

	if (pD3DDevice != NULL)
	{
		// clear the background 
		pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);

		// Use Alpha blending for transparent sprites
		FLOAT NewBlendFactor[4] = { 0,0,0,0 };
		pD3DDevice->OMSetBlendState(g->GetAlphaBlending(), NewBlendFactor, 0xffffffff);

		tank->Render();
		if (tank->GetBullet()->getActive())
		{
			tank->GetBullet()->Render();

		}
		for (auto& enemy : enemys) {
			enemy->Render();
			if (enemy->GetBullet()->getActive()) {
				enemy->GetBullet()->Render();
			}
		}
		//DebugOutTitle(L"01 - Skeleton %0.1f, %0.1f", tank->GetBullet()->GetX(), tank->GetBullet()->GetY());

		spriteHandler->End();
		pSwapChain->Present(0, 0);
	}
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd) 
	{
		DWORD ErrCode = GetLastError();
		DebugOut(L"[ERROR] CreateWindow failed! ErrCode: %d\nAt: %s %d \n", ErrCode, _W(__FILE__), __LINE__);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	SetDebugWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	ULONGLONG frameStart = GetTickCount64();
	ULONGLONG tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ULONGLONG now = GetTickCount64();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		ULONGLONG dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			Update((DWORD)dt);
			Render();
		}
		else
			Sleep((DWORD)(tickPerFrame - dt));
	}

	return 1;
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) 
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	CGame * game = CGame::GetInstance();
	game->Init(hWnd);

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);


	LoadResources();

	Run();

	return 0;
}