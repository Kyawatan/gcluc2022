#pragma once
#include "QuadBase.h"
#include "Definition.h"


class TaskFlag : public TaskBase
{
public:
	TaskFlag(KVector3 vPos);
	~TaskFlag();

	void Update() override;
	void Draw() override;

	// “–‚½‚è”»’è‚Ì•â•ü‚ğˆø‚­
	void DrawCollisionLine(KVector2 vStart, KVector2 vEnd);

private:
	QuadBase* m_pSprite;
	QuadBase* m_pCollisionLine;
};