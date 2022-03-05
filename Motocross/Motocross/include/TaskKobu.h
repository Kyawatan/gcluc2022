#pragma once
#include "Definition.h"
#include "QuadBase.h"


class TaskKobu : public TaskBase
{
public:
	TaskKobu(KVector3 vPos);
	~TaskKobu();

	void Update() override;
	void Draw() override;

private:
	QuadBase* m_pSprite;
};