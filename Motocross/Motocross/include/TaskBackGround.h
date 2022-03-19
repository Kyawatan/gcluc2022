#pragma once
#include "Definition.h"
#include "QuadBase.h"


class TaskBackGround : public TaskBase
{
public:
	TaskBackGround(E_SceneName eCurrentScene);
	~TaskBackGround();

	void Update() override;
	void Draw() override;

	const float GetCourseLength();

private:
	QuadBase*	m_pSprite;
	const float m_fCourseLength;
	bool		m_onSlideIn;

	void SlideIn();
};