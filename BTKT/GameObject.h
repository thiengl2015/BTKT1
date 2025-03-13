#pragma once

#include <Windows.h>
#include <d3dx10.h>

#include "Texture.h"

class CGameObject
{
protected: 
	float x; 
	float y;

	LPTEXTURE texture;			
public: 
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	float GetX() { return x; }
	float GetY() { return y; }

	CGameObject(float x = 0.0f, float y=0.0f, LPTEXTURE texture = NULL);

	virtual void Update(DWORD dt) = 0;
	virtual void Render();
	bool CheckCollision(CGameObject* obj1, CGameObject* obj2)
	{
		float obj1_left = obj1->GetX();
		float obj1_right = obj1->GetX() + 16;
		float obj1_top = obj1->GetY();
		float obj1_bottom = obj1->GetY() + 16;

		float obj2_left = obj2->GetX();
		float obj2_right = obj2->GetX() + 16;
		float obj2_top = obj2->GetY();
		float obj2_bottom = obj2->GetY() + 16;

		return (obj1_right > obj2_left &&
			obj1_left < obj2_right &&
			obj1_bottom > obj2_top &&
			obj1_top < obj2_bottom);
	}

	~CGameObject();
};
typedef CGameObject * LPGAMEOBJECT;

class CBullet : public CGameObject
{
protected:
	float vx;
	float vy;
	bool active;
	LPTEXTURE up, down, left, right;
public:
	CBullet(float x, float y, float vx, float vy,LPTEXTURE up,LPTEXTURE down,LPTEXTURE left,LPTEXTURE right)
		: CGameObject(x, y, up)
	{
		this->up = up;
		this->down = down;
		this->left = left;
		this->right = right;
		this->vx = vx;
		this->vy = vy;
		active = false;
	};

	CBullet()
		: CGameObject(0, 0, nullptr), vx(0), vy(0), active(false),up(nullptr),down(nullptr),left(nullptr),right(nullptr) {}

	LPTEXTURE getUp() { return up; }
	LPTEXTURE getDown() { return down; }
	LPTEXTURE getLeft() { return left; }
	LPTEXTURE getRight() { return right; }
	bool getActive() { return active; }
	void SetActive(bool x) { active = x; }
	void SetVy(float vy) { this->vy = vy; }
	void Update(DWORD dt) override; 
	void Fire(float x, float y, float vx, float vy, LPTEXTURE tex)
	{
		if (!active)
		{
			this->x = x;
			this->y = y;
			this->vx = vx;
			this->vy = vy;
			this->texture = tex;
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
	LPTEXTURE texUp, texDown, texLeft, texRight;

public:
	CBullet *GetBullet() { return bullet; }
	CTank(float x, float y, float vx, float vy,
		LPTEXTURE texUp, LPTEXTURE texDown, LPTEXTURE texLeft, LPTEXTURE texRight,CBullet* bullet)
		: CGameObject(x, y, texUp)
	{
		this->vx = vx;
		this->vy = vy;
		this->bullet = bullet;
		this->texUp = texUp;
		this->texDown = texDown;
		this->texLeft = texLeft;
		this->texRight = texRight;
	}

	void Update(DWORD dt);
};

class CEnemy : public CTank
{
private:
	int color;
	bool active;
public:
	CEnemy(float x, float y, float vx, float vy,
		LPTEXTURE texUp, LPTEXTURE texDown, LPTEXTURE texLeft, LPTEXTURE texRight, CBullet* bullet, int color,bool active)
		: CTank(x, y, vx, vy, texUp, texDown, texLeft, texRight, bullet) {
		this->color = color;
		this->active = active;
	}
	CEnemy() : CTank(0, 0, 0, 0, nullptr, nullptr, nullptr, nullptr, nullptr),color(0),active(true) {}
	bool IsActive() { return active; }
	void SetActive(bool x) { this->active = x; }
	void TakeDamage(LPTEXTURE EWUp,LPTEXTURE EWDown, LPTEXTURE EWLeft, LPTEXTURE EWRight, LPTEXTURE EGUp, LPTEXTURE EGDown, LPTEXTURE EGLeft, LPTEXTURE EGRight);
	void Update(DWORD dt);
	void Render() override
	{
		if (active) 
			CGameObject::Render();
	}
	bool CheckCollision(CGameObject* obj1, CGameObject* obj2)
	{
		float obj1_left = obj1->GetX();
		float obj1_right = obj1->GetX() + 3;
		float obj1_top = obj1->GetY();
		float obj1_bottom = obj1->GetY() + 3;

		float obj2_left = obj2->GetX();
		float obj2_right = obj2->GetX() + 3;
		float obj2_top = obj2->GetY();
		float obj2_bottom = obj2->GetY() + 3;

		return (obj1_right > obj2_left &&
			obj1_left < obj2_right &&
			obj1_bottom > obj2_top &&
			obj1_top < obj2_bottom);
	}
};
