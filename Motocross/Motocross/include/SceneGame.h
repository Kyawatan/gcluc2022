#pragma once
#include "Definition.h"
#include "CameraController.h"
#include "GameDirector.h"
#include "CourseGenerator.h"
#include "QTEController.h"
#include "ScoreController.h"

class SceneGame : public SceneBase
{
public:
	SceneGame();
	~SceneGame();

	void Update() override;
	int GetTotalScore();

private:
	CameraController	m_CameraController;
	GameDirector		m_GameDirector;
	CourseGenerator		m_CourseGenerator;
	QTEController		m_QTEController;
	ScoreController		m_ScoreController;

	TaskBase* m_pWindowEffect;
	TaskBase* m_pPlayer;
	TaskBase* m_pNumber;

	bool	m_isOnce;
	float	m_fWaitTime;
	int		m_iCountDown;
	int		m_iAfterGoalCount;
};