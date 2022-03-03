#pragma once
#include "Definition.h"
#include "CameraController.h"


class SceneGame : public SceneBase
{
public:
	SceneGame();
	~SceneGame();

	void Update() override;

private:
	CameraController* m_pMainCamera;
	TaskBase* m_pPlayer;
};