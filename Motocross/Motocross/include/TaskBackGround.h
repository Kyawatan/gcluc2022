#pragma once
#include "Definition.h"
#include "QuadBase.h"


class TaskBackGround : public TaskBase
{
public:
	TaskBackGround();
	~TaskBackGround();

	void Update() override;
	void Draw() override;

private:
	QuadBase* m_pSprite;
};