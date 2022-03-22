#pragma once
#include "Definition.h"
#include "QuadBase.h"
#include "TaskWindowEffect.h"


class TaskMenu : public TaskBase
{
public:
	TaskMenu(TaskWindowEffect* pWindowEffect);
	~TaskMenu();

	void Update() override;
	void Draw() override;

private:
	TaskWindowEffect* m_pWindowEffect;
	QuadBase* m_pSprite;
	float m_fSpeed;
	bool m_onSlideIn;
	bool m_onSlideOut;

	float m_fWaitTime;

	void SlideIn();
	void SlideOut();
};