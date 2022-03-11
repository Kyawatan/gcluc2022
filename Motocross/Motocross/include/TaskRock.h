#pragma once
#include "QuadBase.h"
#include "Definition.h"
#include "TaskPlayer.h"


class TaskRock : public TaskBase
{
public:
	TaskRock(KVector3 vPos);
	~TaskRock();

	void Update() override;
	void Draw() override;

	// “–‚½‚è”»’è‚ð—LŒø‚É‚·‚é
	void OnCollisionDetection();

private:
	QuadBase*	m_pSprite;
	TaskPlayer* m_pPlayer;
	bool		m_onCollisionDetection;
};