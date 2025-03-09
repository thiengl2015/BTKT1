#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include <windows.h>


/*
	Initialize game object 
*/
CGameObject::CGameObject(float x, float y, LPTEXTURE tex)
{
	this->x = x;
	this->y = y;
	this->texture = tex;
}

void CGameObject::Render()
{
	CGame::GetInstance()->Draw(x, y, texture);
}

CGameObject::~CGameObject()
{
	if (texture != NULL) delete texture;
}

#define TANK_VX 0.1f
#define TANK_WIDTH 14

void CTank::Update(DWORD dt)
{
	if (GetAsyncKeyState('A') & 0x8000) // Nhấn A -> Di chuyển trái
	{
		x -= vx * dt*0.5f;
		this->texture = texLeft; // Đổi sang texture khi di chuyển trái
	}
	if (GetAsyncKeyState('D') & 0x8000) // Nhấn D -> Di chuyển phải
	{
		x += vx * dt*0.5f;
		this->texture = texRight; // Đổi sang texture khi di chuyển phải
	}
	if (GetAsyncKeyState('W') & 0x8000) // Nhấn W -> Di chuyển lên
	{
		y -= vy * dt*0.5f;
		this->texture = texUp; // Đổi sang texture khi di chuyển lên
	}
	if (GetAsyncKeyState('S') & 0x8000) // Nhấn S -> Di chuyển xuống
	{
		y += vy * dt*0.5f;
		this->texture = texDown; // Đổi sang texture khi di chuyển xuống
	}
	if ((GetAsyncKeyState(VK_SPACE) & 0x8000))
	{
		float bulletVx = 0.0, bulletVy = 0.0;

		if (this->texture == texUp) bulletVy = -0.3f;
		else if (this->texture == texDown) bulletVy = 0.3f;
		else if (this->texture == texLeft) bulletVx = -0.3f;
		else if (this->texture == texRight) bulletVx = 0.3f;

		bullet->Fire(this->x, this->y, bulletVx, bulletVy);
	}

	// Giữ tank trong giới hạn màn hình
	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 0) x = 0;
	else if (x >= BackBufferWidth - TANK_WIDTH) x = (float)(BackBufferWidth - TANK_WIDTH);

	bullet->Update(dt);
}
void CBullet::Update(DWORD dt)
{
	if (!active) return;

	x += vx * dt;
	y += vy * dt;

	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	int BackBufferHeight = CGame::GetInstance()->GetBackBufferHeight();
	if (y < 0 || y > BackBufferHeight || x < 0 || x > BackBufferWidth)
	{
		active = false;
	}
}

void CEnemy::Update(DWORD dt)
{
	static DWORD moveTimer = 0;
	static DWORD fireTimer = 0;

	moveTimer += dt;
	fireTimer += dt;

	// Thời gian đổi hướng (ví dụ: mỗi 2 giây)
	const DWORD MOVE_INTERVAL = 2000;
	if (moveTimer >= MOVE_INTERVAL)
	{
		moveTimer = 0;

		// Chọn hướng ngẫu nhiên: 0 = lên, 1 = xuống, 2 = trái, 3 = phải
		int direction = rand() % 4;

		switch (direction)
		{
		case 0: vx = 0;  vy = -0.1f; break; // Đi lên
		case 1: vx = 0;  vy = 0.1f; break;  // Đi xuống
		case 2: vx = -0.1f; vy = 0; break;  // Đi trái
		case 3: vx = 0.1f; vy = 0; break;   // Đi phải
		}
	}

	// Cập nhật vị trí enemy
	x += vx * dt;
	y += vy * dt;

	// Bắn đạn mỗi 1 giây
	const DWORD FIRE_INTERVAL = 5000;
	if (fireTimer >= FIRE_INTERVAL)
	{
		fireTimer = 0;

		float bulletVx = 0, bulletVy = 0;

		// Đạn bắn theo hướng enemy đang di chuyển
		if (vx > 0) bulletVx = 0.3f; // Bắn phải
		else if (vx < 0) bulletVx = -0.3f; // Bắn trái
		else if (vy > 0) bulletVy = 0.3f; // Bắn xuống
		else if (vy < 0) bulletVy = -0.3f; // Bắn lên

		bullet->Fire(x, y, bulletVx, bulletVy);
	}

	// Cập nhật đạn
	bullet->Update(dt);
}