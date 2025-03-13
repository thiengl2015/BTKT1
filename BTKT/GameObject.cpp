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
	int keyCount = 0;
	char moveKey = '\0';

	if (GetAsyncKeyState('A') & 0x8000) { keyCount++; moveKey = 'A'; }
	if (GetAsyncKeyState('D') & 0x8000) { keyCount++; moveKey = 'D'; }
	if (GetAsyncKeyState('W') & 0x8000) { keyCount++; moveKey = 'W'; }
	if (GetAsyncKeyState('S') & 0x8000) { keyCount++; moveKey = 'S'; }

	if (keyCount == 1)
	{
		switch (moveKey)
		{
		case 'A':
			x -= vx * dt * 0.5f;
			this->texture = texLeft;
			break;
		case 'D':
			x += vx * dt * 0.5f;
			this->texture = texRight;
			break;
		case 'W':
			y -= vy * dt * 0.5f;
			this->texture = texUp;
			break;
		case 'S':
			y += vy * dt * 0.5f;
			this->texture = texDown;
			break;
		}
	}

	if ((GetAsyncKeyState(VK_SPACE) & 0x8000))
	{
		float bulletVx = 0.0, bulletVy = 0.0;
		LPTEXTURE tx = nullptr;
		if (this->texture == texUp)
		{
			bulletVy = -0.1f;
			tx = this->GetBullet()->getUp();
		}
		else if (this->texture == texDown)
		{
			bulletVy = 0.1f;
			tx = this->GetBullet()->getDown();
		}
		else if (this->texture == texLeft)
		{
			bulletVx = -0.1f;
			tx = this->GetBullet()->getLeft();
		}
		else if (this->texture == texRight)
		{
			bulletVx = 0.1f;
			tx = this->GetBullet()->getRight();
		}

		bullet->Fire(this->x, this->y, bulletVx, bulletVy,tx);
	}

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
	const DWORD MOVE_INTERVAL = 1000;
	if (moveTimer >= MOVE_INTERVAL)
	{
		moveTimer = 0;

		// Chọn hướng ngẫu nhiên: 0 = lên, 1 = xuống, 2 = trái, 3 = phải
		int direction = rand() % 4;

		switch (direction)
		{
		case 0: vx = 0;  vy = -0.05f; texture = texUp; break;
		case 1: vx = 0;  vy = 0.05f; texture = texDown; break;  
		case 2: vx = -0.05f; vy = 0; texture = texLeft; break;  
		case 3: vx = 0.05f; vy = 0; texture = texRight; break;  
		}
	}
	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	int BackBufferHeight = CGame::GetInstance()->GetBackBufferHeight();
	if (x < 0)
	{
		x = 0;
		vx = -vx;  // Đảo ngược hướng
	}
	else if (x > BackBufferWidth)
	{
		x =BackBufferHeight ;
		vx = -vx;
	}

	// Kiểm tra va chạm cạnh trên/dưới
	if (y < 0)
	{
		y = 0;
		vy = -vy;
	}
	else if (y > BackBufferHeight)
	{
		y = BackBufferHeight ;
		vy = -vy;
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
		LPTEXTURE tx = nullptr;
		if (this->texture == texUp)
		{
			bulletVy = -0.05f;
			tx = this->GetBullet()->getUp();
		}
		else if (this->texture == texDown)
		{
			bulletVy = 0.05f;
			tx = this->GetBullet()->getDown();
		}
		else if (this->texture == texLeft)
		{
			bulletVx = -0.05f;
			tx = this->GetBullet()->getLeft();
		}
		else if (this->texture == texRight)
		{
			bulletVx = 0.05f;
			tx = this->GetBullet()->getRight();
		}

		bullet->Fire(this->x, this->y, bulletVx, bulletVy, tx);
		this->GetBullet()->SetActive(true);
	}

	// Cập nhật đạn
	bullet->Update(dt);
}
void CEnemy::TakeDamage(LPTEXTURE EWUp, LPTEXTURE EWDown, LPTEXTURE EWLeft, LPTEXTURE EWRight, LPTEXTURE EGUp, LPTEXTURE EGDown, LPTEXTURE EGLeft, LPTEXTURE EGRight)
{
	color--;
	if (color == 2)
	{
		texUp = EWUp;
		texDown = EWDown;
		texLeft = EWLeft;
		texRight = EWRight;
	}
	else if (color == 1)
	{
		texUp = EWUp;
		texDown = EWDown;
		texLeft = EWLeft;
		texRight = EWRight;
	}
	else if (color <= 0)
	{
		this->SetActive(false);
	}
}