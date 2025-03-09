#pragma once

#include <Windows.h>
#include <d3dx10.h>

#include "Texture.h"

class CGameObject
{
protected: 
	float x; 
	float y;

	// This should be a pointer to an object containing all graphic/sound/audio assets for rendering this object. 
	// For now, just a pointer to a single texture
	LPTEXTURE texture;			
public: 
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	float GetX() { return x; }
	float GetY() { return y; }

	CGameObject(float x = 0.0f, float y=0.0f, LPTEXTURE texture = NULL);

	virtual void Update(DWORD dt) = 0;
	virtual void Render();

	~CGameObject();
};
typedef CGameObject * LPGAMEOBJECT;

class CBullet : public CGameObject
{
protected:
	float vx;
	float vy;
	bool active;
public:
	CBullet(float x, float y, float vx, float vy, LPTEXTURE texture)
		: CGameObject(x, y, texture)
	{
		this->vx = vx;
		this->vy = vy;
		active = false;
	};

	CBullet()
		: CGameObject(0, 0, nullptr), vx(0), vy(0), active(false) {}

	bool getActive() { return active; }
	void SetActive(bool x) { active = x; }
	void SetVy(float vy) { this->vy = vy; }
	void Update(DWORD dt) override; ;
	void Fire(float x, float y, float vx, float vy)
	{
		if (!active)
		{
			this->x = x;
			this->y = y;
			this->vx = vx;
			this->vy = vy;
			active = true;
		}
	}
};
class CTank : public CGameObject
{
protected:
	float vx;
	float vy;
	CBullet *bullet;
	LPTEXTURE texUp, texDown, texLeft, texRight; // Các texture theo hướng

public:
	CBullet *GetBullet() { return bullet; }
	CTank(float x, float y, float vx, float vy,
		LPTEXTURE texUp, LPTEXTURE texDown, LPTEXTURE texLeft, LPTEXTURE texRight,CBullet* bullet)
		: CGameObject(x, y, texUp) // Ban đầu hướng lên
	{
		this->vx = vx;
		this->vy = vy;
		this->bullet = bullet;
		// Gán texture tương ứng
		this->texUp = texUp;
		this->texDown = texDown;
		this->texLeft = texLeft;
		this->texRight = texRight;
	}

	void Update(DWORD dt);
};

class CEnemy : public CTank
{
public:
	void Update(DWORD dt);

};