#pragma once
#include "QuadBase.h"
#include "Definition.h"

enum class E_FrameName
{
	Left,
	Goal,
	Base,
};

class TaskGame : public TaskBase
{
public:
	TaskGame(KVector3 vPos, E_FrameName eFrameName);
	~TaskGame();

	void Update() override;
	void Draw() override;

	// 当たり判定の補助線を引く
	void DrawCollisionLine(KVector2 vStart, KVector2 vEnd);

private:
	QuadBase* m_pSprite;
	QuadBase* m_pCollisionLine;
};