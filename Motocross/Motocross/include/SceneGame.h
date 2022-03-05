#pragma once
#include "Definition.h"
#include "CameraController.h"
#include "LaneManager.h"
#include "CourseGenerator.h"


class SceneGame : public SceneBase
{
public:
	SceneGame();
	~SceneGame();

	void Update() override;

private:
	CameraController m_MainCamera;
	LaneManager m_LaneManager;
	CourseGenerator m_CourseGenerator;
	TaskBase* m_pPlayer;
};