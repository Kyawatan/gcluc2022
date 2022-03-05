#pragma once
#include "Definition.h"
#include "QuadBase.h"


class TaskCorn : public TaskBase
{
public:
	TaskCorn(KVector3 vPos);
	~TaskCorn();

	void Update() override;
	void Draw() override;

private:
	QuadBase* m_pSprite;
};