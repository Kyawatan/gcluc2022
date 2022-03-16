#pragma once
#include "QuadBase.h"
#include "Definition.h"



class TaskBear : public TaskBase
{
public:
	TaskBear();
	~TaskBear();

	void Update() override;
	void Draw() override;

	void SetRhythm(int iTipNum);

private:
	QuadBase* m_pSprite;
	int m_iTipNum;
	bool m_onEnable;
	bool m_isOnce;
};