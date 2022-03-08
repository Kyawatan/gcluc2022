#pragma once
#include "Definition.h"
#include "CameraController.h"
#include "LaneManager.h"
#include "CourseGenerator.h"
#include "CollisionDetector.h"

class SceneGame : public SceneBase
{
public:
	SceneGame();
	~SceneGame();

	void Update() override;

private:
	CameraController	m_MainCamera;
	LaneManager			m_LaneManager;
	CollisionDetector	m_CollisionDetector;
	CourseGenerator		m_CourseGenerator;
	TaskBase*			m_pPlayer;
};