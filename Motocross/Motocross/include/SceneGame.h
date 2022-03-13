#pragma once
#include "Definition.h"
#include "CameraController.h"
#include "GameDirector.h"
#include "CourseGenerator.h"

//#include "LaneManager.h"
#include "QTEController.h"
//#include "CollisionDetector.h"

class SceneGame : public SceneBase
{
public:
	SceneGame();
	~SceneGame();

	void Update() override;

private:
	CameraController	m_CameraController;
	GameDirector		m_GameDirector;
	//LaneManager			m_LaneManager;
	CourseGenerator		m_CourseGenerator;
	QTEController		m_QTEController;
	//CollisionDetector	m_CollisionDetector;

	TaskBase* m_pPlayer;
};