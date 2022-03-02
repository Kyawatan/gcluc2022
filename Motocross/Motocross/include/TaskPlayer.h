#pragma once
#include "Definition.h"
#include "QuadBase.h"


class TaskPlayer : public TaskBase
{
public:
	TaskPlayer();
	~TaskPlayer();

	void Update() override;
	void Draw() override;

private:
	QuadBase* m_pSprite;
	KawataAnimation* m_pAnim;

	void AutoRun();
	void Move();
};