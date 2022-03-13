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

	// �����蔻��̕⏕��������
	void DrawCollisionLine(KVector2 vStart, KVector2 vEnd);

private:
	QuadBase* m_pSprite;
	QuadBase* m_pCollisionLine;
};