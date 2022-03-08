#pragma once
#include "QuadBase.h"
#include "Definition.h"
#include "CollisionDetector.h"


class TaskKobu : public TaskBase
{
public:
	TaskKobu(KVector3 vPos);
	~TaskKobu();

	void Update() override;
	void Draw() override;

	void SetCollisionDetector(KVector2 fStart, KVector2 fEnd);
	void DrawCollisionLine(); // “–‚½‚è”»’è‚Ì•â•ü‚ğˆø‚­

private:
	QuadBase* m_pSprite;

	QuadBase* m_pCollisionLine;
	CollisionDetector* m_pCollisionDetector;
	KVector2 m_vCollisionStart;
	KVector2 m_vCollisionEnd;
};