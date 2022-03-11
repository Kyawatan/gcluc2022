#pragma once
#include "QuadBase.h"
#include "Definition.h"

enum class E_FrameName
{
	Left,
	Goal,
	Base,
};

class TaskStealFrame : public TaskBase
{
public:
	TaskStealFrame(KVector3 vPos, E_FrameName eFrameName);
	~TaskStealFrame();

	void Update() override;
	void Draw() override;

	// �����蔻��̕⏕��������
	void DrawCollisionLine(KVector2 vStart, KVector2 vEnd);

private:
	QuadBase* m_pSprite;
	QuadBase* m_pCollisionLine;
};