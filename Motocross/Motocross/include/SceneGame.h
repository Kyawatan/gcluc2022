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

private:
	CameraController	m_CameraController;
	GameDirector		m_GameDirector;
	CourseGenerator		m_CourseGenerator;
	QTEController		m_QTEController;
	ScoreController		m_ScoreController;

	TaskBase* m_pWindowEffect;
	TaskBase* m_pPlayer;

	bool m_isOnce;
};